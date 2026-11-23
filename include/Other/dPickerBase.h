#pragma once

#include "decomp.h"
#include "Other/dLayout.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace lyt {
class Material;
class Pane;
class Picture;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// the marker layout of the color pickers of the palette of the design editor (edit_pick.bclyt; a member of Palette, names
// are ours)
struct PickerCursor
{
    PickerCursor() : m_pAll(NULL), m_pPickMaterial(NULL) {}

    ::Layout m_Layout;                 // 0x000
    nw::lyt::Pane* m_pAll;             // 0x14C N_All (moved to the picked color)
    nw::lyt::Material* m_pPickMaterial; // 0x150 of P_pick (its color is the picked one)
};
ASSERT_SIZE(PickerCursor, 0x154);

// The base of the color pickers of the palette of the design editor (class name from the RTTI; all other names are
// ours): a picture P_pick_NN of the palette layout shows some colors of the color table (SvMyDesign::s_Colors) from a
// texture of 16x16 pixels; the cursor (0..1 in x and y) picks one of them.
// RTTI 10PickerBase @ 0x008CB114 (no vtable of its own: abstract)
class PickerBase
{
public:
    // the arguments of Setup
    struct SetupArg
    {
        ssys::ma::lyt::ArcResAccReader* pReader; // 0x00 (not used)
        ssys::ma::lyt::Layout* pLayout;          // 0x04 the layout with the pictures P_pick_NN
        PickerCursor* pCursor;                   // 0x08
        const void* pTexture;                    // 0x0C the texture of the colors (16x16, RGBA4)
        s32 index;                               // 0x10 the picker (P_pick_<index>)
    };
    ASSERT_SIZE(SetupArg, 0x14);

    // the picker of each color of the color table and its index in the picker (Setup)
    struct ColorEntry
    {
        PickerBase* pPicker; // 0x0
        u32 index;           // 0x4
    };

    // (inline in the constructor of Palette)
    PickerBase() : m_pCursor(NULL), m_pLayout(NULL), m_pPicture(NULL), m_Index(0) {}

    // the picture, the colors (into the texture and s_ColorEntries) and the material of the picture
    virtual void Setup(const SetupArg& arg) = 0; // slot 0x00
    // the cursor goes to the color under it (the marker moves, its color becomes the color); the color
    virtual u8 Pick() = 0; // slot 0x04
    // the cursor at the color of index
    virtual void SetCursor(u32 index) = 0; // slot 0x08

    // the texture coordinates of the picture: the pixels x0..x1, y0..y1 of the texture of 16x16
    void SetTexCoordRect(s32 x0, s32 y0, s32 x1, s32 y1); // 0x001AF39C

    // 0x0098CB28
    static ColorEntry s_ColorEntries[256];

protected:
    // (inline) the texture of the colors for the material of the picture (nearest filters)
    void SetTexture(const void* resTexture);
    // (inline) the marker becomes a child of the picture; its color becomes the color of color
    void AttachCursor();
    void SetCursorColor(u8 color);

public:
    PickerCursor* m_pCursor;        // 0x04
    ssys::ma::lyt::Layout* m_pLayout; // 0x08
    nw::lyt::Picture* m_pPicture;   // 0x0C
    s32 m_Index;                    // 0x10
    // the cursor: x and y 0..1 (y only for PickerParts); Palette::UpdatePickTouch transforms the touch position into the
    // picture here (z is not used)
    nn::math::VEC3 m_Cursor;        // 0x14
};
ASSERT_SIZE(PickerBase, 0x20);

#include "Sv/dSvMyDesign.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TexMap.h"
#include "nw/ut/ut_Color8.h"

inline void PickerBase::SetTexture(const void* resTexture)
{
    const nw::gfx::res::ResImageTextureData* texture =
        static_cast<const nw::gfx::res::ResImageTextureData*>(resTexture);
    const nw::gfx::res::ResImageData* image = texture->image.To<nw::gfx::res::ResImageData>();
    const void* address = image->pLocationAddress != NULL ? image->pLocationAddress : image->data.ToPtr();
    nw::lyt::TextureInfo info;
    info.physicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(address));
    info.size = nw::lyt::TexSize(16, 16);
    info.realSize = nw::lyt::TexSize(16, 16);
    info.format = nw::lyt::TEXFORMAT_RGBA4;
    nw::lyt::TexMap texMap(info);
    texMap.m_Bits.minFilter = nw::lyt::TexMap::FILTER_NEAREST;
    texMap.m_Bits.magFilter = nw::lyt::TexMap::FILTER_NEAREST;
    texMap.Update();
    m_pPicture->GetMaterial(0)->SetTexMap(0, &texMap);
}

inline void PickerBase::AttachCursor()
{
    ssys::ma::lyt::Layout* cursor = &m_pCursor->m_Layout;
    cursor->UnbindAllAnimation();
    nw::lyt::Pane* root = cursor->GetRootPane();
    if (root->m_pParent != NULL) {
        root->m_pParent->RemoveChild(root);
    }
    m_pPicture->AppendChild(cursor->GetRootPane());
}

inline void PickerBase::SetCursorColor(u8 color)
{
    const u16 value = SvMyDesign::GetColorValue(color, true);
    const u8 r = ((value >> 8) & 0xF0) | (((value >> 8) & 0xF0) >> 4);
    const u8 g = ((value >> 4) & 0xF0) | (((value >> 4) & 0xF0) >> 4);
    const u8 b = (value & 0xF0) | ((value & 0xF0) >> 4);
    m_pCursor->m_pPickMaterial->m_Colors[0] = nw::ut::Color8(r, g, b, 255);
}
