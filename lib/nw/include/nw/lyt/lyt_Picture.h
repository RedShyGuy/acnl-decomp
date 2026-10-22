#pragma once

#include "decomp.h"
#include "nn/math/math_Vector4.h"
#include "nw/lyt/internal/lyt_TexCoordAry.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
class TexMap;

namespace res {
struct Picture;
} // namespace res

// RTTI N2nw3lyt7PictureE @ 0x008D089C
// vtable 0x00902D18 (vptr 0x00902D20), offset_to_top 0, 30 entries
//
// A pane that draws a quad with a material.
class Picture : public ::nw::lyt::Pane
{
public:
    // the texture coordinates for Drawer: two vectors per texture
    static const int TEXCOORD_VEC_MAX = 6;

    Picture(const res::Picture* pResPic, const ResBlockSet& resBlockSet); // 0x004BBD88 | nintendogs:bytes-fuzzy [tier A]
    virtual ~Picture(); // 0x004BBF38 slot 0x00 (name is ours)
    // 0x004BBECC (deleting dtor)
    virtual const PaneTypeInfo* GetRuntimeTypeInfo() const; // 0x0073E394 slot 0x08 (name is ours)
    virtual ut::Color8 GetVtxColor(u32 idx) const; // 0x0073E374 slot 0x0C
    virtual void SetVtxColor(u32 idx, ut::Color8 value); // 0x004BBB70 slot 0x10
    virtual u8 GetVtxColorElement(u32 idx) const; // 0x0073E3A0 slot 0x1C
    virtual void SetVtxColorElement(u32 idx, u8 value); // 0x004BBB80 slot 0x20
    virtual u8 GetMaterialNum() const; // 0x0073E384 slot 0x24
    virtual Material* GetMaterial(u32 idx); // 0x0073E364 slot 0x28 | libgarden
    virtual void DrawSelf(DrawInfo& drawInfo); // 0x004BBD08 slot 0x64 (name is ours)
    virtual u32* MakeUniformDataSelf(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer); // 0x0073E3B8 slot 0x68 | nintendogs:callseq
    // a texture more (as long as the material has room)
    virtual void Append(const TexMap& texMap); // 0x004BBBC8 slot 0x70 | libgarden
    virtual void Append(const TextureInfo& textureInfo); // 0x004BBB98 slot 0x74 (name is ours)

    // (inline, a copy out of line for the game; name is ours)
    void SetTexCoord(u32 idx, const nn::math::VEC2* coords); // 0x004BBB44 (name is ours)

    static const PaneTypeInfo s_TypeInfo; // (name is ours)

    bool m_IsTexCoordCached;                        // 0x0D4 m_TexCoords match the material
    s32 m_TexCoordNum;                              // 0x0D8
    nn::math::VEC4 m_TexCoords[TEXCOORD_VEC_MAX];   // 0x0DC
    Material* m_pMaterial;                          // 0x13C
    ut::Color8 m_VtxColors[4];                      // 0x140
    internal::TexCoordAry m_TexCoordAry;            // 0x150
};
ASSERT_OFFSET(Picture, m_pMaterial, 0x13C);
ASSERT_OFFSET(Picture, m_TexCoordAry, 0x150);
ASSERT_SIZE(Picture, 0x158);
} // namespace lyt
} // namespace nw
