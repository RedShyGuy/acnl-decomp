#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector4.h"
#include "nw/lyt/internal/lyt_TexCoordAry.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// the frame sizes of a window (names are ours)
struct WindowFrameSize
{
    f32 l;  // 0x0
    f32 r;  // 0x4
    f32 t;  // 0x8
    f32 b;  // 0xC
};
ASSERT_SIZE(WindowFrameSize, 0x10);

// RTTI N2nw3lyt6WindowE @ 0x008D0890
// vtable 0x00902C90 (vptr 0x00902C98), offset_to_top 0, 32 entries
//
// A pane with a content (a quad inside the frames) and one, four or eight frames, each with a
// material (one frame: its texture is flipped to all corners). The names of the functions
// beside the reference ones and of the members are ours.
class Window : public ::nw::lyt::Pane
{
public:
    // a frame
    struct Frame
    {
        // (inline)
        Frame() : textureFlip(0), pMaterial(nullptr) {}

        u8 textureFlip;         // 0x0 the flip of the texture (TEXTUREFLIP_*)
        Material* pMaterial;    // 0x4
    };

    // the ways the texture of a frame is flipped (the rows of s_FlipInfos)
    enum TextureFlip
    {
        TEXTUREFLIP_NONE,
        TEXTUREFLIP_FLIPH,
        TEXTUREFLIP_FLIPV,
        TEXTUREFLIP_ROTATE_90,
        TEXTUREFLIP_ROTATE_180,
        TEXTUREFLIP_ROTATE_270,
        TEXTUREFLIP_MAX
    };

    // the texture coordinates for Drawer: two vectors per texture
    static const int TEXCOORD_VEC_MAX = 6;

    Window(const res::Window* pBlock, const ResBlockSet& resBlockSet); // 0x004BB690 | nintendogs:callseq [tier A]
    virtual ~Window(); // 0x004BB9AC slot 0x00 (name is ours)
    // 0x004BB8D8 (deleting dtor)
    virtual const PaneTypeInfo* GetRuntimeTypeInfo() const; // 0x0073D4F8 slot 0x08 (name is ours)
    virtual ut::Color8 GetVtxColor(u32 idx) const; // 0x0073D394 slot 0x0C
    virtual void SetVtxColor(u32 idx, ut::Color8 value); // 0x004BB22C slot 0x10
    virtual u8 GetVtxColorElement(u32 idx) const; // 0x0073D504 slot 0x1C | mk7dlp:bytes
    virtual void SetVtxColorElement(u32 idx, u8 value); // 0x004BB314 slot 0x20 | nintendogs:bytes
    // the content material and the frame materials
    virtual u8 GetMaterialNum() const; // 0x0073D4E8 slot 0x24
    virtual Material* GetMaterial(u32 idx); // 0x0073D35C slot 0x28
    virtual Material* FindMaterialByName(const char* findName, bool bRecursive); // 0x004BB23C slot 0x30
    virtual void DrawSelf(DrawInfo& drawInfo); // 0x004BB32C slot 0x64 (name is ours)
    virtual u32* MakeUniformDataSelf(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer); // 0x0073D51C slot 0x68
    virtual void DrawContent(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const WindowFrameSize& frameSize, u8 alpha); // 0x004BB150 slot 0x70 (name is ours)
    // one frame, four frames, eight frames
    virtual void DrawFrame(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame& frame, const WindowFrameSize& frameSize, u8 alpha); // 0x004BB43C slot 0x74 (name is ours)
    virtual void DrawFrame4(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha); // 0x004BAD98 slot 0x78 (name is ours)
    virtual void DrawFrame8(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha); // 0x004BAF8C slot 0x7C (name is ours)

    // the sizes of the frames (from the texture sizes of the frame materials)
    WindowFrameSize GetFrameSize(u8 frameNum, const Frame* frames) const; // 0x0073D3A4 (name is ours)

    static const PaneTypeInfo s_TypeInfo; // (name is ours)

    s32 m_ContentTexCoordNum;                               // 0x0D4
    nn::math::VEC4 m_ContentTexCoords[TEXCOORD_VEC_MAX];    // 0x0D8
    res::InflationLRTB m_ContentInflation;                  // 0x138
    ut::Color8 m_ContentVtxColors[4];                       // 0x148
    internal::TexCoordAry m_ContentTexCoordAry;             // 0x158
    Frame* m_pFrames;                                       // 0x160
    Material* m_pContentMaterial;                           // 0x164
    u8 m_FrameNum;                                          // 0x168
    bool m_IsContentTexCoordCached;                         // 0x169
};
ASSERT_OFFSET(Window, m_ContentInflation, 0x138);
ASSERT_OFFSET(Window, m_pFrames, 0x160);
ASSERT_SIZE(Window, 0x16C);
} // namespace lyt
} // namespace nw
