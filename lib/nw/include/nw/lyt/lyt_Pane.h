#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/internal/lyt_PaneBase.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_Color8.h"
#include "nw/ut/ut_LinkList.h"
#include "nw/ut/ut_Rect.h"

namespace nw {
namespace lyt {
class AnimResource;
class AnimTransform;
class DrawInfo;
class Drawer;
class Material;

namespace res {
struct Pane;
} // namespace res

// The runtime type of a pane: GetRuntimeTypeInfo returns the address of a static object per
// class, which points to the one of the base class (Pane: NULL). Struct and member names are ours.
struct PaneTypeInfo
{
    explicit PaneTypeInfo(const PaneTypeInfo* parent) : pParent(parent) {}

    const PaneTypeInfo* pParent; // 0x0
};

// RTTI N2nw3lyt4PaneE @ 0x008D0868
// vtable 0x00902B98 (vptr 0x00902BA0), offset_to_top 0, 28 entries
//
// A pane of a layout: a node of the pane tree with its transformation, size, alpha and the
// animations bound to it. The class name and the names of the functions from the reference
// symbols; the other names are ours.
class Pane : public ::nw::lyt::internal::PaneBase
{
public:
    typedef ut::LinkList<Pane, 4> PaneList;

    // the bits of m_Flag (also in res::Pane)
    enum Flag
    {
        FLAG_VISIBLE = 1 << 0,
        FLAG_INFLUENCED_ALPHA = 1 << 1,        // the alpha of the children is multiplied with it
        FLAG_LOCATION_ADJUST = 1 << 2,         // the size is scaled with DrawInfo::m_LocationAdjustScale
        FLAG_USER_ALLOCATED = 1 << 3,          // not deleted with its parent / layout
        FLAG_MTX_CONDITION_MASK = 3 << 4,      // the last form of the matrix (MTX_CONDITION_*)
        FLAG_USER_MTX = 1 << 6,                // m_Mtx is not calculated
        FLAG_USER_GLOBAL_MTX = 1 << 7          // m_GlbMtx is not calculated
    };
    // the scale m_Mtx was made with (it is kept while the condition stays)
    enum MtxCondition
    {
        MTX_CONDITION_DIRTY,
        MTX_CONDITION_SCALE,            // m_Scale
        MTX_CONDITION_LOCATION_ADJUST   // m_Scale with DrawInfo::m_LocationAdjustScale
    };
    // the options of Animate
    static const u32 ANIMATE_OPTION_SKIP_INVISIBLE = 1 << 0;
    // the bits of m_BasePosition beside the position (0-3: h + v * 3)
    static const u8 BASEPOSITION_IGNORE_PARTS_ALPHA_ZERO = 1 << 4; // not drawn with alpha 0

    // the targets of the animations (SetColorElement / GetColorElement)
    static const u32 ANIMTARGET_PANE_ALPHA = 16;
    // the floats of the SRT animation (from m_Translate; the size does not change m_Mtx)
    static const u32 ANIMTARGET_PANE_SIZE_X = 8;
    static const u32 ANIMTARGET_PANE_SRT_MAX = 10;

    explicit Pane(const res::Pane* pBlock); // 0x004B7E78 | nintendogs:callseq-callee [tier A]
    virtual ~Pane(); // 0x004B7FC4 slot 0x00 | nintendogs:bytes
    // 0x004B7FB0 (deleting dtor)

    virtual const PaneTypeInfo* GetRuntimeTypeInfo() const; // 0x0073CE08 slot 0x08 (name is ours)
    virtual ut::Color8 GetVtxColor(u32 idx) const; // 0x0073CD54 slot 0x0C
    virtual void SetVtxColor(u32 idx, ut::Color8 value); // 0x004B7470 slot 0x10
    // the alpha (ANIMTARGET_PANE_ALPHA), else the vertex color element
    virtual u8 GetColorElement(u32 idx) const; // 0x0073CD68 slot 0x14 | mk7dlp:bytes
    virtual void SetColorElement(u32 idx, u8 value); // 0x004B7904 slot 0x18 | nintendogs:bytes
    virtual u8 GetVtxColorElement(u32 idx) const; // 0x0073CE14 slot 0x1C
    virtual void SetVtxColorElement(u32 idx, u8 value); // 0x004B7BE8 slot 0x20
    virtual u8 GetMaterialNum() const; // 0x0073CD60 slot 0x24
    virtual Material* GetMaterial(u32 idx); // 0x0073CC58 slot 0x28
    virtual Pane* FindPaneByName(const char* findName, bool bRecursive); // 0x004B7890 slot 0x2C | nintendogs:bytes
    virtual Material* FindMaterialByName(const char* findName, bool bRecursive); // 0x004B7994 slot 0x30 | nintendogs:bytes
    virtual void Animate(u32 option); // 0x004B7D84 slot 0x34 | nintendogs:bytes
    virtual void AnimateSelf(u32 option); // 0x004B7334 slot 0x38 | nintendogs:bytes
    virtual void BindAnimation(AnimTransform* pAnimTrans, bool bRecursive, bool bDisable); // 0x004B7870 slot 0x3C | nintendogs:bytes
    virtual void UnbindAnimation(AnimTransform* pAnimTrans, bool bRecursive); // 0x004B7920 slot 0x40 | nintendogs:bytes
    virtual void UnbindAllAnimation(bool bRecursive); // 0x004B7BEC slot 0x44 (name is ours)
    virtual void UnbindAnimationSelf(AnimTransform* pAnimTrans); // 0x004BEF64 slot 0x48 | nintendogs:bytes
    virtual AnimationLink* FindAnimationLinkSelf(AnimTransform* pAnimTrans); // 0x004BEEC0 slot 0x4C (name is ours)
    virtual AnimationLink* FindAnimationLinkSelf(const AnimResource& animRes); // 0x004BEF00 slot 0x50 (name is ours)
    virtual void SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable, bool bRecursive); // 0x004B7A58 slot 0x54 (name is ours)
    virtual void SetAnimationEnable(const AnimResource& animRes, bool bEnable, bool bRecursive); // 0x004B7B20 slot 0x58 (name is ours)
    virtual void CalculateMtx(const DrawInfo& drawInfo); // 0x004B7474 slot 0x5C | libgarden
    // the pane (if visible) and its children are drawn with GL
    virtual void Draw(DrawInfo& drawInfo); // 0x004B7C00 slot 0x60 (name is ours)
    virtual void DrawSelf(DrawInfo& drawInfo); // 0x004B7E74 slot 0x64 (name is ours)
    // the GPU commands of the pane for Drawer; the end
    virtual u32* MakeUniformDataSelf(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer); // 0x0073CE1C slot 0x68
    // the global matrix becomes the view matrix of the graphics resource
    virtual void LoadMtx(DrawInfo& drawInfo); // 0x004B7DF0 slot 0x6C (name is ours)

    void AppendChild(Pane* pChild); // 0x004B73FC | libgarden [tier A]
    // pChild before pNext
    void InsertChild(Pane* pNext, Pane* pChild); // 0x004B7420 | mk7dlp:bytes [tier B]
    void RemoveChild(Pane* pChild); // 0x004B7450 | libgarden [tier A]
    void AddAnimationLink(AnimationLink* pAnimationLink); // 0x004B7984 | nintendogs:callgraph [tier A]

    // (inline, copies out of line for the game; names are ours)
    void PrependChild(Pane* pChild); // 0x004B7828 (name is ours)
    void SetTranslate(const nn::math::VEC3& value); // 0x004B7850 (name is ours)
    void SetName(const char* name); // 0x004B7E34 (name is ours)
    // the commands of the pane and its visible children
    u32* MakeUniformData(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer); // 0x0073CD84 (name is ours)

    // the first material (NULL: none)
    // (mk7dlp names it const, but it calls the non-const GetMaterial(u32), see Picture::GetMaterial
    // in libgarden)
    Material* GetMaterial(); // 0x0073CC60 | mk7dlp:bytes [tier B]
    // the rectangle of the pane around its base position
    ut::Rect GetPaneRect() const; // 0x0073CC9C | nintendogs:callgraph [tier A]
    // the left top corner relative to the base position
    nn::math::VEC2 GetVtxPos() const; // 0x0073CE24 | nintendogs:callgraph [tier A]

    // (inline)
    bool IsVisible() const { return (m_Flag & FLAG_VISIBLE) != 0; }
    bool IsUserAllocated() const { return (m_Flag & FLAG_USER_ALLOCATED) != 0; }
    u8 GetBasePositionH() const { return (m_BasePosition & 0xF) % 3; }
    u8 GetBasePositionV() const { return (m_BasePosition & 0xF) / 3; }
    static Pane* FromLink(ut::LinkListNode* pNode) { return PaneList::GetPointerFromNode(pNode); }

protected:
    void Init(); // 0x004B7C78 | nintendogs:bytes-fuzzy [tier A]

public:
    static const PaneTypeInfo s_TypeInfo; // (name is ours)

    Pane* m_pParent;                    // 0x0C
    PaneList m_ChildList;               // 0x10
    AnimationList m_AnimList;           // 0x1C
    nn::math::VEC3 m_Translate;         // 0x28
    nn::math::VEC3 m_Rotate;            // 0x34 in degrees
    nn::math::VEC2 m_Scale;             // 0x40
    Size m_Size;                        // 0x48
    nn::math::MTX34 m_Mtx;              // 0x50
    nn::math::MTX34 m_GlbMtx;           // 0x80
    const void* m_pExtUserDataList;     // 0xB0 the 'usd1' block behind the pane
    u8 m_Alpha;                         // 0xB4
    u8 m_GlbAlpha;                      // 0xB5
    u8 m_BasePosition;                  // 0xB6
    u8 m_Flag;                          // 0xB7
    char m_Name[17];                    // 0xB8
    char m_UserData[9];                 // 0xC9
    // never accessed, but the members of the derived classes start at 0xD4 (ARMCC uses the tail
    // padding of a base class otherwise, see nex::Ticket)
    u8 m_Unknown0xD2[2];                // 0xD2
};
ASSERT_OFFSET(Pane, m_pParent, 0x0C);
ASSERT_OFFSET(Pane, m_GlbMtx, 0x80);
ASSERT_OFFSET(Pane, m_Flag, 0xB7);
ASSERT_SIZE(Pane, 0xD4);

// (inline) the pane as a T if its runtime type is T or derived from it, else NULL (name is ours)
template <typename T>
inline T* DynamicCast(Pane* pPane)
{
    if (pPane != nullptr) {
        for (const PaneTypeInfo* pTypeInfo = pPane->GetRuntimeTypeInfo(); pTypeInfo != nullptr; pTypeInfo = pTypeInfo->pParent) {
            if (pTypeInfo == &T::s_TypeInfo) {
                return static_cast<T*>(pPane);
            }
        }
    }
    return nullptr;
}

namespace internal {
// the base positions of new panes ignore alpha 0 (BASEPOSITION_IGNORE_PARTS_ALPHA_ZERO; name is
// ours)
extern bool g_IsIgnorePartsAlphaZero;
} // namespace internal
} // namespace lyt
} // namespace nw
