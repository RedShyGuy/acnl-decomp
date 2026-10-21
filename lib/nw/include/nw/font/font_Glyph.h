#pragma once

#include "decomp.h"
#include "nw/font/font_ResourceFormat.h"

namespace nw {
namespace font {
namespace internal {
class TextureObject;
} // namespace internal

// A character of a font: where its image is on the sheet and its widths. Member names are ours.
class Glyph
{
public:
    Glyph(); // 0x004D935C | nintendogs:callgraph [tier A]

    const void* m_pTexture;                    // 0x00 the sheet
    CharWidths m_Widths;                       // 0x04
    u8 m_Height;                               // 0x07 the cell height
    u16 m_TexWidth;                            // 0x08
    u16 m_TexHeight;                           // 0x0A
    u16 m_CellX;                               // 0x0C
    u16 m_CellY;                               // 0x0E
    u16 m_TexFormat;                           // 0x10
    bool m_IsSheetUpdated;                     // 0x12
    internal::TextureObject* m_pTextureObject; // 0x14
};
ASSERT_SIZE(Glyph, 0x18);
} // namespace font
} // namespace nw
