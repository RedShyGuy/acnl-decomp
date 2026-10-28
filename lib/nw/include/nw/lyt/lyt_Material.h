#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_TexMap.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
class AnimResource;
class AnimTransform;
class DrawInfo;

namespace res {
struct Material;
} // namespace res

// RTTI N2nw3lyt8MaterialE @ 0x008D08C8
// vtable 0x00902EA8 (vptr 0x00902EB0), offset_to_top 0, 11 entries
//
// The colors, textures and texture combiner of a pane. The parts live in one block of the
// layout memory: the texture maps, texture SRTs, texture coordinate generators, the alpha
// compare, the blend mode and the combiner stages (the counts in m_MemCap / m_MemNum). The names
// of the functions beside the reference ones and of the members are ours.
class Material
{
public:
    // the counts of the parts
    struct MemSize
    {
        u32 texMap : 2;
        u32 texSRT : 2;
        u32 texCoordGen : 2;
        u32 tevStage : 3;
        u32 alpCmp : 1;
        u32 blendMode : 1;
        u32 reserve : 21;
    };
    // the bits of m_Flag
    enum Flag
    {
        FLAG_USER_ALLOCATED = 1 << 0,   // not deleted with its pane
        FLAG_TEXTURE_ONLY = 1 << 1,     // drawn without a texture combiner of its own (res bit 11)
        FLAG_CACHE_VALID = 1 << 2       // the texture coordinates of the pane are up to date
    };

    // the parts
    static const int TEXMAP_MAX = 3;
    static const int TEVSTAGE_MAX = 6;
    // the colors (MATCOLOR_*)
    enum MaterialColor
    {
        MATCOLOR_BLACK,     // the color black of the texture maps to (the combiner buffer color)
        MATCOLOR_WHITE,     // the color white maps to
        MATCOLOR_C0,        // the constant colors
        MATCOLOR_C1,
        MATCOLOR_C2,
        MATCOLOR_C3,
        MATCOLOR_C4,
        MATCOLOR_MAX
    };
    // the target of the color animations (SetColorElement: color * 4 + element)
    static const u32 ANIMTARGET_MATCOLOR_MAX = MATCOLOR_MAX * 4;

    Material(const res::Material* pRes, const ResBlockSet& resBlockSet); // 0x004BE1C8 | nintendogs:bytes-fuzzy [tier A]
    virtual ~Material(); // 0x004BE8A8 slot 0x00 | libgarden
    // 0x004BE864 (deleting dtor)
    virtual void BindAnimation(AnimTransform* pAnimTrans); // 0x004BCC18 slot 0x08 (name is ours)
    virtual void UnbindAnimation(AnimTransform* pAnimTrans); // 0x004BDFDC slot 0x0C
    virtual void UnbindAllAnimation(); // 0x004BE104 slot 0x10 (name is ours)
    virtual void Animate(); // 0x004BE14C slot 0x14 | nintendogs:bytes
    virtual AnimationLink* FindAnimationLink(AnimTransform* pAnimTrans); // 0x004BDFF4 slot 0x18 (name is ours)
    virtual AnimationLink* FindAnimationLink(const AnimResource& animRes); // 0x004BDFFC slot 0x1C (name is ours)
    virtual void SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable); // 0x004BE0BC slot 0x20 (name is ours)
    virtual void SetAnimationEnable(const AnimResource& animRes, bool bEnable); // 0x004BE0E0 slot 0x24 (name is ours)
    // the GL state of the material (the uniforms of the pane shader, the textures)
    virtual void SetupGL(const DrawInfo& drawInfo, u8 alpha, bool bInitFrameSpec); // 0x004BCC34 slot 0x28 (name is ours)

    // the memory of the parts (with the defaults of the SRTs, the alpha compare and the blend
    // mode); the parts are lost
    void ReserveMem(u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocAlpCmp, bool allocBlendMode); // 0x004BC900 | libgarden [tier A]
    void SetTexMapNum(u8 num); // 0x004BCBBC | nintendogs:bytes [tier A]
    void SetTexCoordGenNum(u8 num); // 0x004BE02C | nintendogs:bytes [tier A]
    void SetTevStageNum(u8 num); // 0x004BDE54 | nintendogs:bytes [tier A]
    // color idx / 4, element idx % 4
    void SetColorElement(u32 idx, u8 value); // 0x004BDF9C | nintendogs:bytes [tier A]
    void AddAnimationLink(AnimationLink* pAnimationLink); // 0x004BDFE4 | nintendogs:callgraph [tier A]

    TexSRT* GetTexSRTAry(); // 0x004BCBA8 | libgarden [tier A]
    TexCoordGen* GetTexCoordGenAry(); // 0x004BE004 | nintendogs:callseq-callee [tier A]
    // (inline, copies out of line)
    DECOMP_EMIT_COPY TexMap* GetTexMapAry() { return static_cast<TexMap*>(m_pMem); } // 0x004BCBA0 (name is ours)
    DECOMP_EMIT_COPY const TexMap* GetTexMapAry() const { return static_cast<const TexMap*>(m_pMem); } // 0x0073E858 (name is ours)
    const TexSRT* GetTexSRTAry() const; // 0x0073E860 (name is ours)
    const TexCoordGen* GetTexCoordGenAry() const; // 0x0073E900 (name is ours)
    const TevStage* GetTevStageAry() const; // 0x0073E874 | nintendogs:bytes [tier A]
    const BlendMode* GetBlendModePtr() const; // 0x0073E8C0 | nintendogs:bytes [tier A]
    const AlphaCompare* GetAlphaComparePtr() const; // 0x0073E928 | nintendogs:bytes [tier A]
    // (inline, a copy out of line for the game)
    void SetTexMap(u32 idx, const TexMap* pTexMap); // 0x004BE198 (name is ours)

    // (inline)
    const char* GetName() const { return m_Name; }
    u8 GetTexMapNum() const { return m_MemNum.texMap; }
    u8 GetTexSRTNum() const { return m_MemNum.texSRT; }
    u8 GetTexCoordGenNum() const { return m_MemNum.texCoordGen; }
    u8 GetTevStageNum() const { return m_MemNum.tevStage; }
    bool IsAlphaCompareCap() const { return m_MemNum.alpCmp != 0; }
    bool IsBlendModeCap() const { return m_MemNum.blendMode != 0; }
    bool IsUserAllocated() const { return (m_Flag & FLAG_USER_ALLOCATED) != 0; }
    bool IsTextureOnly() const { return (m_Flag & FLAG_TEXTURE_ONLY) != 0; }
    bool IsCacheValid() const { return (m_Flag & FLAG_CACHE_VALID) != 0; }
    void SetCacheDirty() { m_Flag &= ~FLAG_CACHE_VALID; }
    void SetCacheValid() { m_Flag |= FLAG_CACHE_VALID; }

protected:
    DECOMP_NOINLINE void Init(); // 0x004BE114 | nintendogs:bytes [tier A]

public:
    AnimationList m_AnimList;               // 0x04
    ut::Color8 m_Colors[MATCOLOR_MAX];      // 0x10
    MemSize m_MemCap;                       // 0x2C
    MemSize m_MemNum;                       // 0x30
    void* m_pMem;                           // 0x34
    char m_Name[21];                        // 0x38
    u8 m_Flag;                              // 0x4D
};
ASSERT_OFFSET(Material, m_MemCap, 0x2C);
ASSERT_OFFSET(Material, m_Flag, 0x4D);
ASSERT_SIZE(Material, 0x50);
} // namespace lyt
} // namespace nw
