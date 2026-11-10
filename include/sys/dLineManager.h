#pragma once

#include "decomp.h"
#include "nw/font/font_TextWriterBase.h"

namespace nw {
namespace lyt {
class TextBox;
}
} // namespace nw

namespace sys {
class TextManager;

// The lines of the text being typed: where each line starts (wrapped at the width of the text box, at a number of
// characters, or at a line feed), calculated on demand. Only the class name is from the binary (RTTI); member and
// function names are ours.
// RTTI N3sys11LineManagerE @ 0x008D1030
// vtable 0x009047B0 (vptr 0x009047B8), offset_to_top 0, 2 entries
class LineManager
{
public:
    // (inline)
    LineManager(s32 lineNum, TextManager* text)
        : m_LineNum(lineNum), m_pText(text), m_LineStarts(NULL), m_CalculatedNum(0), m_Width(0.0f), m_StartX(0.0f), m_IsSetup(false),
          m_MaxLineLength(0)
    {
    }
    virtual ~LineManager(); // 0x00522D68 slot 0x00
    // 0x00522D28 slot 0x04 (deleting dtor)

    // the writer gets the font and spaces of the text box (with a fixed width: no character space); the lines are at
    // most width wide (and the text box) and maxLineLength characters long (0: any)
    void Setup(nw::lyt::TextBox* textBox, u32 maxLineLength, bool isFixedWidth, f32 width); // 0x00522688
    // the line of the character (the last line if it is behind); its column in column (if not NULL)
    DECOMP_NOINLINE s32 GetLineOfIndex(s32 index, s32* column); // 0x00522720
    // the number of characters of the line (-1: no such line)
    s32 GetLineLength(s32 line); // 0x00522814
    // the start of the line (-1: no such line)
    DECOMP_NOINLINE s32 GetLineStartChecked(s32 line); // 0x00522884
    // the character c still fits into the lines when it is put at pos (isReplace: in place of the character there;
    // isCounted: its width counts); the characters behind it may move down to the next lines
    bool CanInsert(s32 pos, u16 c, bool isReplace, bool isCounted); // 0x005228E0
    // the start of the line (calculated if needed)
    u16 GetLineStart(s32 line); // 0x00522B68
    // the starts of the lines are calculated up to the line
    void Calc(s32 lastLine); // 0x00522B94

    // (inline) the lines from the one of the character on are calculated again
    void InvalidateFrom(s32 index) { m_CalculatedNum = GetLineOfIndex(index, NULL); }
    // (inline) the width of the character with the writer
    f32 GetCharWidth(u16 c) const
    {
        if (c < ' ') {
            return 0.0f;
        }
        if (m_Writer.m_IsWidthFixed) {
            return m_Writer.m_FixedWidth;
        }
        return m_Writer.m_pFont->GetCharWidth(c) * m_Writer.m_ScaleH;
    }

    // (inline) the last character of the line (the end of the text for the last line; -1: no such line)
    s32 GetLineEnd(s32 line); // (defined in dTextManager.h)

    s32 m_LineNum;                                // 0x04
    TextManager* m_pText;                         // 0x08
    u16* m_LineStarts;                            // 0x0C
    s32 m_CalculatedNum;                          // 0x10 the lines whose start is known
    nw::font::TextWriterBase<wchar_t> m_Writer;   // 0x14
    f32 m_Width;                                  // 0x78
    f32 m_StartX;                                 // 0x7C the x of the text in the first line
    bool m_IsSetup;                               // 0x80
    u32 m_MaxLineLength;                          // 0x84 (0: any)
};
ASSERT_OFFSET(LineManager, m_Writer, 0x14);
ASSERT_OFFSET(LineManager, m_Width, 0x78);
ASSERT_SIZE(LineManager, 0x88);
} // namespace sys
