#pragma once

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GroupContainer.h"
#include "ssys/ma/lyt/dBase2D.h"
#include "ssys/ma/lyt/dDummyLayout.h"

namespace nw {
namespace lyt {
class Bounding;
class Group;
class Pane;
class Picture;
class ResourceAccessor;
class TextBox;
} // namespace lyt
} // namespace nw

namespace ssys {
namespace ma {
namespace lyt {
class Animation;
class ArcResAccReader;

// a layout file (BCLYT) of the game: the nw layout, its draw info and camera, and a saved command list of its drawing
// that is only made again after Animate (names after libgarden where it has them, the rest are ours)
// RTTI N4ssys2ma3lyt6LayoutE @ 0x008D249C
// vtable 0x00907310 (vptr 0x00907318), offset_to_top 0, 7 entries
class Layout : public ::ssys::ma::lyt::Base2D
{
public:
    // the values of m_DrawMode: the screen the layout is drawn on (names are ours)
    enum DrawMode
    {
        DRAW_MODE_TOP_LEFT = 0x400,   // the left eye of the upper screen (the parallax is subtracted)
        DRAW_MODE_TOP_RIGHT = 0x410,  // the right eye of the upper screen (the parallax is added)
        DRAW_MODE_BOTTOM = 0x401,     // the lower screen
    };

    DECOMP_EMIT_COPY Layout() // 0x0012D2D0 | libgarden
        : m_pArcReader(NULL), m_CameraPos(0.0f, 0.0f, 1.0f), m_AnimateOption(0), m_Cmdlist(0), m_SavedBufferOffset(0),
          m_SavedBufferSize(0), m_SavedRequestOffset(0), m_SavedRequestSize(0), m_SavedStateMask(0), m_CmdlistStorageSize(0),
          m_IsCopyCmd(false), m_AnimateDone(false), m_IsViewUpdated(false), m_IsProjUpdated(false), m_IsStereo(false),
          m_IsPerspective(false), m_IsAnimationChanged(0), m_Parallax(0.0f), m_PrevParallax(0.0f), m_StereoDistance(0.0f),
          m_CameraDistance(0.0f), m_DrawMode(0), m_IsScissorEnable(false), m_ScissorWidth(0.0f), m_ScissorHeight(0.0f)
    {
        m_DrawInfo.m_pGraphicsResource = GetGraphicsResource();
    }
    virtual ~Layout(); // 0x00569F68 slot 0x00 | libgarden
    // 0x00569F1C slot 0x04 (deleting dtor)
    // draws the saved command list
    void DrawCached() override; // 0x005699A4 slot 0x08 (name after libgarden)
    // animates the panes and calculates the view and the matrices
    void Animate() override; // 0x005696F8 slot 0x0C | libgarden
    // the layout name of arcReader (NULL: the reader of the layout); true if it was built already
    bool Build(const char* name, const ArcResAccReader* arcReader, u32 cmdCacheSize) override; // 0x00569C6C slot 0x10 | libgarden
    // saves the drawing of the panes into the command list after Animate
    void DrawSave() override; // 0x00569D9C slot 0x14 (name after libgarden)
    // the projection matrix of the layout rectangle
    void UpdateProj() override; // 0x00569288 slot 0x18 (name after libgarden)

    DECOMP_EMIT_COPY void UnbindAllAnimation() { m_Layout.nw::lyt::Layout::UnbindAllAnimation(); } // 0x00133A70 | libgarden [tier A]
    DECOMP_EMIT_COPY nw::lyt::Group* FindGroup(const char* name) // 0x004B5648 | libgarden [tier A]
    {
        return m_Layout.GetGroupContainer()->FindGroupByName(name);
    }
    nw::lyt::Pane* FindPane(const char* name); // 0x00569274 | libgarden [tier A]
    nw::lyt::Picture* FindPicture(const char* name); // 0x0056950C | libgarden [tier A]
    nw::lyt::TextBox* FindTextBox(const char* name); // 0x00569568 | libgarden [tier A]
    nw::lyt::Bounding* FindBounding(const char* name); // 0x005695C4 (name is ours)
    void BindAnimation(Animation* anim, const nw::lyt::Group* group, bool recursive); // 0x00569118 | libgarden [tier A]
    void UnbindAnimation(Animation* anim, const nw::lyt::Group* group, bool recursive); // 0x0056946C | libgarden [tier A]
    // (names are ours)
    void BindAnimation(Animation* anim, nw::lyt::Pane* pane, bool recursive); // 0x00569D68
    void UnbindAnimation(Animation* anim); // 0x00569F08
    // (its code lies in front of nw::lyt::Layout::BindAnimation and falls into it)
    void BindAnimation(Animation* anim); // 0x004BA1EC
    // shows or hides the panes of a group
    void SetGroupVisible(const char* groupName, bool visible); // 0x005694C0
    // a transform of the animation that is not in the list of the layout (the caller deletes it)
    nw::lyt::AnimTransform* CreateAnimTransform(const void* animResBuf, nw::lyt::ResourceAccessor* resAccessor); // 0x00569620
    // the parallax of the 3D slider; the matrices are calculated again when it changed
    void UpdateParallax(); // 0x00569178
    // the body of DrawCached (the draw info is that of the layout)
    void Draw(const nw::lyt::DrawInfo& drawInfo); // 0x005699AC

    // the position in the layout of a position on the screen (y down, origin top left; name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void ScreenToLayout(const nn::math::VEC2& screen, nn::math::VEC2* pos) // 0x00750C5C
    {
        nw::ut::Rect rect = m_Layout.GetLayoutRect();
        f32 centerX = (rect.left + rect.right) * 0.5f;
        f32 centerY = (rect.top + rect.bottom) * 0.5f;
        pos->x = screen.x - centerX + rect.left;
        pos->y = rect.top - (screen.y - centerY);
        nn::math::VEC3 vec(pos->x, pos->y, 0.0f);
        nn::math::ARMv6::VEC3TransformAsm(&vec, &m_DrawInfo.m_ViewMtx, &vec);
        pos->x = vec.x;
        pos->y = vec.y;
    }

    void SetArcResAccReader(ArcResAccReader* reader) { m_pArcReader = reader; }
    DummyLayout& GetLayout() { return m_Layout; }
    nw::lyt::Pane* GetRootPane() { return m_Layout.GetRootPane(); }

    // (inline) the graphics resource of the LayoutMgr
    static nw::lyt::GraphicsResource* GetGraphicsResource();

    DummyLayout m_Layout;                // 0x10
    nw::lyt::DrawInfo m_DrawInfo;        // 0x30
    nn::math::MTX34 m_InvViewMtx;        // 0xBC the inverse of the view matrix (without 3D)
    ArcResAccReader* m_pArcReader;       // 0xEC
    nn::math::VEC3 m_CameraPos;          // 0xF0 the camera looks at the origin (z: without 3D)
    u32 m_AnimateOption;                 // 0xFC
    u32 m_Cmdlist;                       // 0x100 the command list of the saved drawing
    u32 m_SavedBufferOffset;             // 0x104 the saved part of it (nngxStopCmdlistSave)
    s32 m_SavedBufferSize;               // 0x108
    s32 m_SavedRequestOffset;            // 0x10C
    s32 m_SavedRequestSize;              // 0x110
    u32 m_SavedStateMask;                // 0x114
    u32 m_CmdlistStorageSize;            // 0x118
    bool m_IsCopyCmd;                    // 0x11C the saved commands are copied when drawn
    bool m_AnimateDone;                  // 0x11D animated since the last DrawSave
    bool m_IsViewUpdated;                // 0x11E
    bool m_IsProjUpdated;                // 0x11F
    bool m_IsStereo;                     // 0x120 with the 3D effect
    bool m_IsPerspective;                // 0x121 else orthographic
    u8 m_IsAnimationChanged;             // 0x122 set by the buttons when they change an animation; cleared by Animate
    f32 m_Parallax;                      // 0x124 in pixels of the upper screen
    f32 m_PrevParallax;                  // 0x128
    f32 m_StereoDistance;                // 0x12C the camera z with 3D
    f32 m_CameraDistance;                // 0x130 perspective: the distance at which the layout fills the view
    u32 m_DrawMode;                      // 0x134 DrawMode
    bool m_IsScissorEnable;              // 0x138
    f32 m_ScissorX;                      // 0x13C the scissor rectangle in the layout
    f32 m_ScissorY;                      // 0x140
    f32 m_ScissorWidth;                  // 0x144
    f32 m_ScissorHeight;                 // 0x148
};
ASSERT_OFFSET(Layout, m_DrawInfo, 0x30);
ASSERT_OFFSET(Layout, m_pArcReader, 0xEC);
ASSERT_OFFSET(Layout, m_Cmdlist, 0x100);
ASSERT_OFFSET(Layout, m_Parallax, 0x124);
ASSERT_SIZE(Layout, 0x14C);
} // namespace lyt
} // namespace ma
} // namespace ssys

#include "ssys/ma/lyt/dLayoutMgr.h"

inline nw::lyt::GraphicsResource* ssys::ma::lyt::Layout::GetGraphicsResource()
{
    return &LayoutMgr::Get()->m_Resource;
}

// the projection of the layouts (also in the Layout of the game; names are ours)
namespace ssys {
namespace ma {
namespace lyt {
namespace detail {
// the vertical field of view of the perspective projection: half of it as an angle index (256 = a full turn), 45 degrees
static const f32 HALF_FOVY_IDX = 16.0f;
// the depth range of the projections
static const f32 ORTHO_NEAR = -500.0f;
static const f32 ORTHO_FAR = 500.0f;
static const f32 PERSPECTIVE_NEAR = 0.05f;
static const f32 PERSPECTIVE_FAR = 500.0f;

// (inline) a perspective projection like the nw::gfx camera makes it (name is ours)
inline void MakePerspective(nn::math::MTX44* pOut, f32 halfFovyIdx, f32 aspect, f32 near, f32 far)
{
    f32 sinValue, cosValue;
    nn::math::SinCosFIdx(&sinValue, &cosValue, halfFovyIdx);
    const f32 cotangent = cosValue / sinValue;
    const f32 depth = 1.0f / (far - near);
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = 0.0f;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][2] = 0.0f;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][2] = -1.0f;
    pOut->m[3][3] = 0.0f;
    pOut->m[0][0] = cotangent / aspect;
    pOut->m[1][1] = cotangent;
    pOut->m[2][2] = far * depth;
    pOut->m[2][3] = far * near * depth;
}

// (inline) the projection of a layout: perspective or orthographic, turned for the screen
inline void MakeProjection(nw::lyt::DrawInfo* drawInfo, const nw::ut::Rect& rect, bool isPerspective)
{
    nn::math::MTX44 proj;
    if (isPerspective) {
        MakePerspective(&proj, HALF_FOVY_IDX, (rect.right - rect.left) / (rect.top - rect.bottom), PERSPECTIVE_NEAR,
                        PERSPECTIVE_FAR);
    } else {
        nn::math::ARMv6::MTX44OrthoC_FAST(&proj, rect.left, rect.right, rect.bottom, rect.top, ORTHO_NEAR, ORTHO_FAR);
    }
    nn::math::ARMv6::MTX44PivotC_FAST(&proj, nn::math::PIVOT_DIRECTION_1);
    drawInfo->SetProjectionMtx(proj);
}
} // namespace detail
} // namespace lyt
} // namespace ma
} // namespace ssys
