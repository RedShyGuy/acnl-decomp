#pragma once

#include "decomp.h"
#include "nw/font/font_CharWriter.h"
#include "nw/font/font_Font.h"
#include "nw/font/font_TagProcessorBase.h"
#include "nw/ut/ut_Rect.h"

namespace nw {
namespace font {
// A CharWriter for strings: line feeds and tabs through the tag processor, alignment of the
// lines, a line width limit with wrapping. The class name and the names of the functions are
// from the reference symbols; member names are ours.
template <typename T>
class TextWriterBase : public CharWriter
{
public:
    // the bits of m_DrawFlag (names are ours)
    enum DrawFlag
    {
        HORIZONTAL_ALIGN_LEFT = 0x0,
        HORIZONTAL_ALIGN_CENTER = 0x1,
        HORIZONTAL_ALIGN_RIGHT = 0x2,
        HORIZONTAL_ALIGN_MASK = 0x3,
        HORIZONTAL_ORIGIN_LEFT = 0x00,
        HORIZONTAL_ORIGIN_CENTER = 0x10,
        HORIZONTAL_ORIGIN_RIGHT = 0x20,
        HORIZONTAL_ORIGIN_MASK = 0x30,
        VERTICAL_ORIGIN_TOP = 0x000,
        VERTICAL_ORIGIN_MIDDLE = 0x100,
        VERTICAL_ORIGIN_BOTTOM = 0x200,
        VERTICAL_ORIGIN_BASELINE = 0x300,
        VERTICAL_ORIGIN_MASK = 0x300
    };
    static const int DEFAULT_TAB_WIDTH = 4;

    TextWriterBase();
    ~TextWriterBase();

    // the string at the cursor; the width of the text
    f32 Print(const T* str, int length) const;

    // the height of a line (the line feed of the font and the line space)
    f32 GetLineHeight() const
    {
        const int linefeed = m_pFont != nullptr ? m_pFont->GetLineFeed() : 0;
        return m_LineSpace + linefeed * m_ScaleV;
    }

    f32 PrintImpl(const T* str, int length);
    // the cursor is moved for the origin and the alignment; the width of the text
    f32 AdjustCursor(f32* pX, f32* pY, const T* str, int length);
    f32 CalcLineWidth(const T* str, int length);
    // the rectangle of a line; the string is set behind the line; true: the line was wrapped
    bool CalcLineRectImpl(nw::ut::Rect* pRect, const T** pStr, int length);
    void CalcStringRectImpl(nw::ut::Rect* pRect, const T* str, int length);

    // the processor of strings without one of their own (name is ours)
    static TagProcessorBase<T> s_DefaultTagProcessor;

    f32 m_WidthLimit;                    // 0x4C
    f32 m_CharSpace;                     // 0x50
    f32 m_LineSpace;                     // 0x54
    int m_TabWidth;                      // 0x58 in characters
    u32 m_DrawFlag;                      // 0x5C
    TagProcessorBase<T>* m_pTagProcessor; // 0x60
};
} // namespace font
} // namespace nw
