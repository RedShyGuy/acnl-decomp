#pragma once

#include "decomp.h"

namespace nw {
namespace font {
class Font;

namespace internal {
// A sheet of a font as a texture (ResFont::SetDrawBuffer: one per sheet). Member names are ours.
class TextureObject
{
public:
    // the GPU address is the physical address / 8 (0: no image)
    void Set(u32 name, const nw::font::Font* pFont, const void* pImage, u16 format, u16 width, u16 height); // 0x00141B5C | nintendogs:bytes [tier A]

    u32 m_Name;              // 0x00 the texture name (0: none)
    const Font* m_pFont;     // 0x04
    u32 m_ImageAddress;      // 0x08
    u16 m_Height;            // 0x0C
    u16 m_Width;             // 0x0E
    u8 m_Format;             // 0x10 (FontSheetFormat)
};
ASSERT_SIZE(TextureObject, 0x14);
} // namespace internal
} // namespace font
} // namespace nw
