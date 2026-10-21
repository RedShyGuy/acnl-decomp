#pragma once

#include "decomp.h"
#include "nw/font/font_DispStringBuffer.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace font {
class Font;
class Glyph;
class RectDrawer;
class TextWriterResource;

// Draws characters of a font: with GL through a TextWriterResource, or into a DispStringBuffer
// for RectDrawer. Member names are ours.
class CharWriter
{
public:
    // the kinds of color gradation (m_GradationMode)
    enum GradationMode
    {
        GRADATION_MODE_NONE,
        GRADATION_MODE_H,
        GRADATION_MODE_V
    };
    // the vertices of a character (m_VertexColors)
    enum Vertex
    {
        VERTEX_RT,
        VERTEX_LT,
        VERTEX_LB,
        VERTEX_RB,
        VERTEX_COUNT
    };

    CharWriter(); // 0x004D6E38 | nintendogs:callgraph [tier A]

    // the scale of the font for the size
    void SetFontSize(f32 width, f32 height); // 0x004D64D0 | libgarden [tier A]
    f32 GetFontWidth() const; // 0x00744E34 | nintendogs:bytes [tier A]
    f32 GetFontHeight() const; // 0x00744E8C | nintendogs:bytes [tier A]
    f32 GetFontAscent() const; // 0x00744E60 | nintendogs:bytes [tier A]

    // the GL state for the drawing (name is ours)
    void SetupGX(); // 0x004D6B78 (name is ours)
    // the vertex colors from the text colors and the gradation mode
    void UpdateVertexColors(); // 0x004D6738 | libgarden [tier A]
    // the characters of the display string buffer are reset
    void StartPrint(); // 0x004D64B4 | libgarden [tier A]
    // the character at the cursor; the advance
    f32 Print(u16 code); // 0x004D6AA8 | nintendogs:bytes [tier A]
    // the commands of the display string buffer are put into the command list; the end
    u32* UseCommandBuffer(u32* pCommand, nw::font::RectDrawer* pDrawer); // 0x004D652C | libgarden [tier A]

    // the size of a display string buffer for the characters
    static u32 GetDispStringBufferSize(u32 charCount); // 0x004D67A0 | libgarden [tier A]
    // the display string buffer in the memory (NULL: no memory)
    static DispStringBuffer* InitDispStringBuffer(void* pBuffer, u32 charCount); // 0x004D6790 | libgarden [tier A]

private:
    // the texture combiners for the color mapping (name is ours)
    void SetupGXWithColorMapping(bool isAlphaTexture); // 0x004D67D4 (name is ours)
    void PrintGlyph(f32 x, const nw::font::Glyph& glyph); // 0x004D61CC | nintendogs:bytes-fuzzy [tier A]

public:
    ut::Color8 m_ColorMappingMin;            // 0x00 black maps to it
    ut::Color8 m_ColorMappingMax;            // 0x04 white maps to it
    ut::Color8 m_VertexColors[VERTEX_COUNT]; // 0x08
    ut::Color8 m_TextColors[2];              // 0x18
    u8 m_GradationMode;                      // 0x20
    f32 m_ScaleH;                            // 0x24
    f32 m_ScaleV;                            // 0x28
    f32 m_CursorX;                           // 0x2C
    f32 m_CursorY;                           // 0x30
    f32 m_CursorZ;                           // 0x34
    f32 m_FixedWidth;                        // 0x38
    const Font* m_pFont;                     // 0x3C
    TextWriterResource* m_pTextWriterResource; // 0x40
    DispStringBuffer* m_pDispStringBuffer;   // 0x44
    bool m_IsWidthFixed;                     // 0x48
    u8 m_Alpha;                              // 0x49
};
ASSERT_OFFSET(CharWriter, m_pFont, 0x3C);
ASSERT_SIZE(CharWriter, 0x4C);
} // namespace font
} // namespace nw
