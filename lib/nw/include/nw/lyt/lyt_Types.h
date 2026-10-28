#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nw/ut/ut_Color8.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace lyt {
class AnimTransform;
class ResourceAccessor;

namespace res {
struct TextureList;
struct FontList;
struct MaterialList;
} // namespace res

// The types the layout classes share. Type names that appear in the reference symbols (Size,
// TextureInfo, ResBlockSet, AnimationLink) are from there; everything else is ours.

namespace internal {
// (inline) at most dstSize - 1 characters of src and the terminating zero (the first character
// is always copied; name is ours)
inline void CopyString(char* dst, u32 dstSize, const char* src)
{
    u32 i = 0;
    do {
        *dst++ = *src;
        ++i;
    } while (*++src != '\0' && dstSize - 1 > i);
    *dst = '\0';
}
} // namespace internal

// the size of a pane
struct Size
{
    // (inline)
    DECOMP_EMIT_COPY Size() : width(0.0f), height(0.0f) {} // 0x004B80A0 (name is ours)
    Size(f32 w, f32 h) : width(w), height(h) {}

    f32 width;  // 0x0
    f32 height; // 0x4
};
ASSERT_SIZE(Size, 0x8);

// the size of a texture in texels
struct TexSize
{
    // (inline)
    TexSize() : width(0), height(0) {}
    TexSize(u16 w, u16 h) : width(w), height(h) {}

    u16 width;  // 0x0
    u16 height; // 0x2
};
ASSERT_SIZE(TexSize, 0x4);

// the texture formats of a BCLIM image (the order of the format table 0x008A3B20)
enum TexFormat : u8 {
    TEXFORMAT_L8,
    TEXFORMAT_A8,
    TEXFORMAT_LA4,
    TEXFORMAT_LA8,
    TEXFORMAT_HILO8,
    TEXFORMAT_RGB565,
    TEXFORMAT_RGB8,
    TEXFORMAT_RGB5A1,
    TEXFORMAT_RGBA4,
    TEXFORMAT_RGBA8,
    TEXFORMAT_ETC1,
    TEXFORMAT_ETC1A4,
    TEXFORMAT_L4,
    TEXFORMAT_A4,
    TEXFORMAT_MAX
};

// A loaded texture: the GL texture name (or, without GL textures, the VRAM address with the
// area in the low bits), its physical address and size.
struct TextureInfo
{
    // (inline)
    TextureInfo() : texObject(0), physicalAddress(0), size(), realSize() {}

    u32 texObject;          // 0x00
    u32 physicalAddress;    // 0x04
    TexSize size;           // 0x08 the size of the image
    TexSize realSize;       // 0x0C the size of the texture (powers of two)
    u8 format;              // 0x10 TexFormat
};
ASSERT_SIZE(TextureInfo, 0x14);

// The blocks of a layout file that the panes take their resources from (in the order of
// Layout::Build) and the accessor of the files.
struct ResBlockSet
{
    const res::TextureList* pTextureList;   // 0x0
    const res::FontList* pFontList;         // 0x4
    const res::MaterialList* pMaterialList; // 0x8
    ResourceAccessor* pResAccessor;         // 0xC
};
ASSERT_SIZE(ResBlockSet, 0x10);

// The binding of an animation to a pane or a material: an entry of their animation lists.
class AnimationLink
{
public:
    // (inline)
    AnimationLink() : m_pAnimTransform(nullptr), m_Index(0), m_IsDisable(false) {}

    // (inline)
    void Reset()
    {
        m_pAnimTransform = nullptr;
        m_Index = 0;
        m_IsDisable = false;
    }
    // (inline)
    void Set(AnimTransform* pAnimTransform, u16 index, bool isDisable)
    {
        m_pAnimTransform = pAnimTransform;
        m_Index = index;
        m_IsDisable = isDisable;
    }

    ut::LinkListNode m_Link;            // 0x0
    AnimTransform* m_pAnimTransform;    // 0x8
    u16 m_Index;                        // 0xC the animation content of the resource
    bool m_IsDisable;                   // 0xE
};
ASSERT_SIZE(AnimationLink, 0x10);

typedef ut::LinkList<AnimationLink, 0> AnimationList;

// the texture coordinates of the four corners of a quad (left top, right top, left bottom,
// right bottom)
typedef nn::math::VEC2 TexCoordQuad[4];

// the transformation of texture coordinates
struct TexSRT
{
    // (inline)
    TexSRT() : translate(0.0f, 0.0f), rotate(0.0f), scale(0.0f, 0.0f) {}

    nn::math::VEC2 translate;   // 0x00
    f32 rotate;                 // 0x08 in degrees
    nn::math::VEC2 scale;       // 0x0C
};
ASSERT_SIZE(TexSRT, 0x14);

// where the texture coordinates of a texture come from
struct TexCoordGen
{
    // (inline)
    TexCoordGen() : genType(0), texGenSrc(0) {}

    u8 genType;     // 0x0
    u8 texGenSrc;   // 0x1 the set of texture coordinates of the pane
    u8 reserve[2];  // 0x2
};
ASSERT_SIZE(TexCoordGen, 0x4);

// The sources, operands, combine mode and scale of a part (RGB or alpha) of a stage of the
// texture combiner (4 bit values: the indices of the tables of GraphicsResource).
struct TevStageInOp
{
    u32 src0 : 4;           // bits 0-11 the sources
    u32 src1 : 4;
    u32 src2 : 4;
    u32 op0 : 4;            // bits 12-23 the operands
    u32 op1 : 4;
    u32 op2 : 4;
    u32 combine : 4;        // bits 24-27
    u32 scale : 2;          // bits 28-29
    u32 isSavePrevReg : 1;  // bit 30 the result goes to the combiner buffer
    u32 reserve : 1;
};
ASSERT_SIZE(TevStageInOp, 0x4);

// a stage of the texture combiner
struct TevStage
{
    // (inline) modulate the previous stage with nothing: the previous result (source 5) as is
    TevStage()
    {
        rgb.combine = 0;
        alpha.combine = 0;
        rgb.src0 = 5;
        rgb.src1 = 5;
        rgb.src2 = 5;
        alpha.src0 = 5;
        alpha.src1 = 5;
        alpha.src2 = 5;
        rgb.op0 = 0;
        rgb.op1 = 0;
        rgb.op2 = 0;
        alpha.op0 = 0;
        alpha.op1 = 0;
        alpha.op2 = 0;
        rgb.scale = 0;
        alpha.scale = 0;
        konstColor.rgb = 1;
        konstColor.alpha = 1;
        rgb.isSavePrevReg = 0;
        alpha.isSavePrevReg = 0;
    }

    struct KonstColor
    {
        u32 rgb : 4;    // the material color of the constant color
        u32 alpha : 4;
        u32 reserve : 24;
    };

    TevStageInOp rgb;       // 0x0
    TevStageInOp alpha;     // 0x4
    KonstColor konstColor;  // 0x8
};
ASSERT_SIZE(TevStage, 0xC);

// the alpha test
struct AlphaCompare
{
    // (inline)
    AlphaCompare(u8 compFunc, f32 refValue) : func(compFunc), ref(refValue)
    {
        reserve[0] = 0;
        reserve[1] = 0;
        reserve[2] = 0;
    }

    u8 func;        // 0x0
    u8 reserve[3];  // 0x1
    f32 ref;        // 0x4
};
ASSERT_SIZE(AlphaCompare, 0x8);

// the blending: a blend equation, else a logical operation (both 0: the default blending)
struct BlendMode
{
    // (inline)
    BlendMode(u8 op, u8 src, u8 dst, u8 logic) : blendOp(op), srcFactor(src), dstFactor(dst), logicOp(logic) {}

    u8 blendOp;     // 0x0 0: no blend equation
    u8 srcFactor;   // 0x1
    u8 dstFactor;   // 0x2
    u8 logicOp;     // 0x3 0: no logical operation
};
ASSERT_SIZE(BlendMode, 0x4);
} // namespace lyt
} // namespace nw
