#include "sys/dLineManager.h"

#include <cstring>

#include "nw/lyt/lyt_TextBox.h"
#include "sys/dTextManager.h"

namespace sys {
// 0x00522688
void sys::LineManager::Setup(nw::lyt::TextBox* textBox, u32 maxLineLength, bool isFixedWidth, f32 width)
{
    m_Writer.m_pFont = textBox->m_pFont;
    m_Writer.SetFontSize(textBox->m_FontSize.width, textBox->m_FontSize.height);
    m_Writer.m_LineSpace = textBox->m_LineSpace;
    m_Writer.m_CharSpace = isFixedWidth ? 0.0f : textBox->m_CharSpace;
    m_Writer.m_IsWidthFixed = isFixedWidth;
    // (the width of the text box goes to the fixed width, which is only used with isFixedWidth)
    m_Writer.m_FixedWidth = textBox->m_Size.width;
    if (isFixedWidth) {
        m_Width = width;
    } else {
        f32 limit = textBox->m_Size.width;
        if (limit > width) {
            limit = width;
        }
        m_Width = limit;
    }
    m_IsSetup = true;
    m_CalculatedNum = 0;
    m_MaxLineLength = maxLineLength;
}

// 0x00522720
s32 sys::LineManager::GetLineOfIndex(s32 index, s32* column)
{
    for (s32 line = 0; line < m_LineNum - 1; line++) {
        const u16 next = GetLineStart(line + 1);
        if (next == 0 || next > index) {
            if (column != NULL) {
                *column = index - GetLineStart(line);
            }
            return line;
        }
    }
    if (column != NULL) {
        *column = index - GetLineStart(m_LineNum - 1);
    }
    return m_LineNum - 1;
}

// (inline)
inline s32 sys::LineManager::GetLineEnd(s32 line)
{
    const s32 next = GetLineStartChecked(line + 1);
    if (next >= 0) {
        return next - 1;
    }
    if (GetLineStartChecked(line) >= 0) {
        return m_pText->m_Length;
    }
    return -1;
}

// 0x00522814
s32 sys::LineManager::GetLineLength(s32 line)
{
    const s32 start = GetLineStartChecked(line);
    const s32 end = GetLineEnd(line);
    if (start == -1) {
        return -1;
    }
    s32 length = end - start + 1;
    if (m_pText->m_Length == end) {
        length--;
    }
    return length;
}

// 0x00522884
s32 sys::LineManager::GetLineStartChecked(s32 line)
{
    if (line < 0 || line > m_LineNum - 1) {
        return -1;
    }
    const u16 start = GetLineStart(line);
    if (line != 0 && start <= 0) {
        return -1;
    }
    return start;
}

// 0x005228E0
bool sys::LineManager::CanInsert(s32 pos, u16 c, bool isReplace, bool isCounted)
{
    s32 next = pos + 1;
    if (next < 0) {
        next = 0;
    }
    if (pos < 0) {
        pos = 0;
    }
    s32 line = GetLineOfIndex(next, NULL);
    f32 width = GetCharWidth(c);
    if (!isCounted) {
        width = 0.0f;
    }
    f32 overflow = 0.0f;
    f32 x = m_StartX;
    while (line < m_LineNum) {
        const s32 start = GetLineStart(line);
        if (start < 0) {
            break;
        }
        const s32 last = GetLineEnd(line);
        const bool isNewline = last != -1 && m_pText->GetDisplayChar(last) == '\n';
        s32 end = GetLineEnd(line);
        if (m_pText->m_Length == end) {
            end--;
        }
        overflow = 0.0f;
        for (s32 i = start; i <= end; i++) {
            const f32 charWidth = GetCharWidth(m_pText->GetDisplayChar(i));
            if (!isReplace && i == pos) {
                x += width;
                if (m_Width < x) {
                    overflow += width;
                }
            }
            x += charWidth;
            if (m_Width < x) {
                overflow += charWidth;
            }
        }
        if (pos > end && !isReplace) {
            if (m_Width < x + width) {
                overflow += width;
            }
        }
        if (overflow == 0.0f) {
            break;
        }
        if (GetLineStartChecked(m_LineNum - 1) >= 0 && isNewline) {
            break;
        }
        x = overflow;
        line++;
    }
    return overflow == 0.0f;
}

// 0x00522B68
u16 sys::LineManager::GetLineStart(s32 line)
{
    if (m_CalculatedNum <= line) {
        Calc(line);
    }
    return m_LineStarts[line];
}

// 0x00522B94
void sys::LineManager::Calc(s32 lastLine)
{
    for (s32 line = m_CalculatedNum; line <= lastLine; line++) {
        if (line == 0) {
            m_LineStarts[0] = 0;
            m_CalculatedNum = 1;
            continue;
        }
        const s32 start = GetLineStart(line - 1);
        s32 i = start;
        s32 breakPos = start;
        u16 c = m_pText->GetDisplayChar(i);
        f32 x = 0.0f;
        u32 count = 0;
        while (c != '\n' && c != 0) {
            const f32 width = GetCharWidth(c);
            count++;
            if (x + width > m_Width || (m_MaxLineLength != 0 && count > m_MaxLineLength)) {
                if (breakPos == start) {
                    breakPos = i;
                }
                break;
            }
            x += m_Writer.m_CharSpace + width;
            c = m_pText->GetDisplayChar(++i);
        }
        if (c == 0) {
            std::memset(&m_LineStarts[line], 0, (m_LineNum - line) * sizeof(u16));
            m_CalculatedNum = m_LineNum;
            return;
        }
        if (c == '\n') {
            m_LineStarts[line] = i + 1;
        } else {
            m_LineStarts[line] = breakPos;
        }
        m_CalculatedNum = line + 1;
    }
}

// 0x00522D68 slot 0x00
// 0x00522D28 (deleting dtor)
sys::LineManager::~LineManager()
{
    if (m_LineStarts != NULL) {
        delete[] m_LineStarts;
        m_LineStarts = NULL;
    }
}

} // namespace sys
