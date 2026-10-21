#include <cmath>

#include "nw/font/font_TextWriterBase.h"

namespace nw {
namespace font {
namespace {
// no width limit (the largest float)
const f32 WIDTH_LIMIT_NONE = 3.402823466e+38f;

// the rectangle is extended to the other one (inline; name is ours)
inline void MergeRect(ut::Rect* pRect, const ut::Rect& rect)
{
    pRect->left = pRect->left <= rect.left ? pRect->left : rect.left;
    pRect->top = pRect->top <= rect.top ? pRect->top : rect.top;
    pRect->right = pRect->right >= rect.right ? pRect->right : rect.right;
    pRect->bottom = pRect->bottom >= rect.bottom ? pRect->bottom : rect.bottom;
}

// the next character of the reader (inline; name is ours)
inline u16 ReadNextChar(CharStrmReader* pReader)
{
    return (pReader->*pReader->m_ReadFunc)();
}
} // namespace

// TagProcessorBase

template <typename T>
TagProcessorBase<T>::~TagProcessorBase()
{
}

template <typename T>
DECOMP_ALWAYS_INLINE void TagProcessorBase<T>::ProcessTab(PrintContext<T>* pContext)
{
    TextWriterBase<T>* pWriter = pContext->writer;
    const int tabWidth = pWriter->m_TabWidth;
    if (tabWidth > 0) {
        const f32 charWidth = pWriter->m_IsWidthFixed ? pWriter->m_FixedWidth : pWriter->GetFontWidth();
        const f32 dx = pWriter->m_CursorX - pContext->xOrigin;
        const f32 tabPixel = tabWidth * charWidth;
        const int numTab = static_cast<int>(dx / tabPixel) + 1;
        pWriter->m_CursorX = pContext->xOrigin + tabPixel * numTab;
    }
}

template <typename T>
DECOMP_ALWAYS_INLINE void TagProcessorBase<T>::ProcessLinefeed(PrintContext<T>* pContext)
{
    TextWriterBase<T>* pWriter = pContext->writer;
    const f32 x = pContext->xOrigin;
    const f32 y = pWriter->m_CursorY + pWriter->GetLineHeight();
    pWriter->m_CursorX = x;
    pWriter->m_CursorY = y;
}

template <typename T>
typename TagProcessorBase<T>::Operation TagProcessorBase<T>::Process(u16 code, PrintContext<T>* pContext)
{
    switch (code) {
    case '\n':
        ProcessLinefeed(pContext);
        return OPERATION_NEXT_LINE;
    case '\t':
        ProcessTab(pContext);
        return OPERATION_NO_CHAR_SPACE;
    default:
        return OPERATION_DEFAULT;
    }
}

template <typename T>
typename TagProcessorBase<T>::Operation TagProcessorBase<T>::CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext<T>* pContext)
{
    switch (code) {
    case '\n': {
        const TextWriterBase<T>* pWriter = pContext->writer;
        pRect->right = pWriter->m_CursorX;
        pRect->top = pWriter->m_CursorY;
        ProcessLinefeed(pContext);
        pRect->left = pWriter->m_CursorX;
        pRect->bottom = pContext->writer->GetFontHeight() + pWriter->m_CursorY;
        pRect->Normalize();
        return OPERATION_NEXT_LINE;
    }
    case '\t': {
        const TextWriterBase<T>* pWriter = pContext->writer;
        pRect->left = pWriter->m_CursorX;
        ProcessTab(pContext);
        pRect->right = pWriter->m_CursorX;
        pRect->top = pWriter->m_CursorY;
        pRect->bottom = pWriter->GetFontHeight() + pRect->top;
        pRect->Normalize();
        return OPERATION_NO_CHAR_SPACE;
    }
    default:
        return OPERATION_DEFAULT;
    }
}

// TextWriterBase

template <typename T>
TagProcessorBase<T> TextWriterBase<T>::s_DefaultTagProcessor;

template <typename T>
TextWriterBase<T>::TextWriterBase()
    : m_WidthLimit(WIDTH_LIMIT_NONE), m_CharSpace(0.0f), m_LineSpace(0.0f), m_TabWidth(DEFAULT_TAB_WIDTH), m_DrawFlag(0),
      m_pTagProcessor(&s_DefaultTagProcessor)
{
}

template <typename T>
TextWriterBase<T>::~TextWriterBase()
{
}

template <typename T>
f32 TextWriterBase<T>::Print(const T* str, int length) const
{
    // a copy draws: the cursor is the only state that changes
    TextWriterBase<T> writer(*this);
    const f32 width = writer.PrintImpl(str, length);
    TextWriterBase<T>* pThis = const_cast<TextWriterBase<T>*>(this);
    pThis->m_CursorX = writer.m_CursorX;
    pThis->m_CursorY = writer.m_CursorY;
    return width;
}

template <typename T>
f32 TextWriterBase<T>::AdjustCursor(f32* pX, f32* pY, const T* str, int length)
{
    f32 textWidth = 0.0f;
    f32 textHeight = 0.0f;
    const u32 flags = m_DrawFlag & (HORIZONTAL_ALIGN_MASK | HORIZONTAL_ORIGIN_MASK | VERTICAL_ORIGIN_MASK);
    if (flags != (HORIZONTAL_ALIGN_LEFT | HORIZONTAL_ORIGIN_LEFT | VERTICAL_ORIGIN_BASELINE) && flags != 0) {
        ut::Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
        TextWriterBase<T> writer(*this);
        writer.CalcStringRectImpl(&rect, str, length);
        textWidth = rect.left + rect.right;
        textHeight = rect.top + rect.bottom;
    }

    switch (m_DrawFlag & HORIZONTAL_ORIGIN_MASK) {
    case HORIZONTAL_ORIGIN_CENTER:
        *pX -= std::ceil(textWidth * 0.5f);
        break;
    case HORIZONTAL_ORIGIN_RIGHT:
        *pX -= textWidth;
        break;
    default:
        break;
    }
    switch (m_DrawFlag & VERTICAL_ORIGIN_MASK) {
    case VERTICAL_ORIGIN_MIDDLE:
        *pY -= std::ceil(textHeight * 0.5f);
        break;
    case VERTICAL_ORIGIN_BOTTOM:
        *pY -= textHeight;
        break;
    default:
        break;
    }

    switch (m_DrawFlag & HORIZONTAL_ALIGN_MASK) {
    case HORIZONTAL_ALIGN_CENTER: {
        const f32 lineWidth = CalcLineWidth(str, length);
        m_CursorX = *pX + (std::ceil(textWidth * 0.5f) - std::ceil(lineWidth * 0.5f));
        break;
    }
    case HORIZONTAL_ALIGN_RIGHT: {
        const f32 lineWidth = CalcLineWidth(str, length);
        m_CursorX = *pX + (textWidth - lineWidth);
        break;
    }
    default:
        m_CursorX = *pX;
        break;
    }
    if ((m_DrawFlag & VERTICAL_ORIGIN_MASK) == VERTICAL_ORIGIN_BASELINE) {
        m_CursorY = *pY;
    } else {
        m_CursorY = *pY + GetFontAscent();
    }
    return textWidth;
}

template <typename T>
f32 TextWriterBase<T>::CalcLineWidth(const T* str, int length)
{
    ut::Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
    TextWriterBase<T> writer(*this);
    writer.m_CursorX = 0.0f;
    writer.m_CursorY = 0.0f;
    writer.CalcLineRectImpl(&rect, &str, length);
    return rect.GetWidth();
}

template <typename T>
bool TextWriterBase<T>::CalcLineRectImpl(nw::ut::Rect* pRect, const T** pStr, int length)
{
    const T* str = *pStr;
    const T* end = str + length;
    const bool isUseLimit = m_WidthLimit < WIDTH_LIMIT_NONE;
    PrintContext<T> context = {this, str, 0.0f, 0.0f, 0};
    // (the width of a character is checked from the line start)
    f32 limitOrigin = 0.0f;
    bool isCharSpace = false;
    bool isOver = false;
    const T* prevStreamPos = nullptr;
    f32 x = 0.0f;
    CharStrmReader reader = m_pFont->GetCharStrmReader(T());

    const f32 height = GetLineHeight() - m_LineSpace;
    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = height >= 0.0f ? 0.0f : height;
    pRect->bottom = height > 0.0f ? height : 0.0f;

    reader.m_pCharStrm = str;
    u16 code = ReadNextChar(&reader);
    while (static_cast<const T*>(reader.m_pCharStrm) <= end) {
        if (code < ' ') {
            ut::Rect rect(x, 0.0f, 0.0f, 0.0f);
            context.str = static_cast<const T*>(reader.m_pCharStrm);
            context.flags = isCharSpace ? 0 : TagProcessorBase<T>::PRINT_FLAG_CHAR_SPACE_NOT_NEEDED;
            m_CursorX = x;
            if (isUseLimit && code != '\n' && prevStreamPos != nullptr) {
                // a tag that goes beyond the limit starts a new line
                PrintContext<T> context2 = context;
                TextWriterBase<T> writer(*this);
                ut::Rect rect2(0.0f, 0.0f, 0.0f, 0.0f);
                context2.writer = &writer;
                m_pTagProcessor->CalcRect(&rect2, code, &context2);
                if (rect2.GetWidth() > 0.0f && writer.m_CursorX - context.xOrigin > m_WidthLimit) {
                    isOver = true;
                    code = '\n';
                    reader.m_pCharStrm = prevStreamPos;
                    continue;
                }
            }
            const typename TagProcessorBase<T>::Operation operation = m_pTagProcessor->CalcRect(&rect, code, &context);
            reader.m_pCharStrm = context.str;
            MergeRect(pRect, rect);
            x = m_CursorX;
            if (operation == TagProcessorBase<T>::OPERATION_END_DRAW) {
                *pStr += length;
                return false;
            } else if (operation == TagProcessorBase<T>::OPERATION_NO_CHAR_SPACE) {
                isCharSpace = false;
            } else if (operation == TagProcessorBase<T>::OPERATION_CHAR_SPACE) {
                isCharSpace = true;
            } else if (operation == TagProcessorBase<T>::OPERATION_NEXT_LINE) {
                break;
            }
        } else {
            f32 newX = x;
            if (isCharSpace) {
                newX += m_CharSpace;
            }
            if (m_IsWidthFixed) {
                newX += m_FixedWidth;
            } else {
                newX += m_pFont->GetCharWidth(code) * m_ScaleH;
            }
            if (isUseLimit && prevStreamPos != nullptr && newX - limitOrigin > m_WidthLimit) {
                isOver = true;
                code = '\n';
                reader.m_pCharStrm = prevStreamPos;
                continue;
            }
            x = newX;
            isCharSpace = true;
            pRect->left = pRect->left > x ? x : pRect->left;
            pRect->right = pRect->right < x ? x : pRect->right;
        }
        if (isUseLimit) {
            prevStreamPos = static_cast<const T*>(reader.m_pCharStrm);
        }
        code = ReadNextChar(&reader);
    }
    *pStr = static_cast<const T*>(reader.m_pCharStrm);
    return isOver;
}

template <typename T>
void TextWriterBase<T>::CalcStringRectImpl(nw::ut::Rect* pRect, const T* str, int length)
{
    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;
    m_CursorX = 0.0f;
    m_CursorY = 0.0f;
    const T* end = str + length;
    int remain = length;
    do {
        ut::Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
        CalcLineRectImpl(&rect, &str, remain);
        pRect->left = pRect->left > rect.left ? rect.left : pRect->left;
        pRect->top = pRect->top <= rect.top ? pRect->top : rect.top;
        pRect->right = pRect->right >= rect.right ? pRect->right : rect.right;
        pRect->bottom = pRect->bottom >= rect.bottom ? pRect->bottom : rect.bottom;
        remain = end - str;
    } while (remain > 0);
}

template <typename T>
f32 TextWriterBase<T>::PrintImpl(const T* str, int length)
{
    f32 xOrigin = m_CursorX;
    const f32 orgCursorY = m_CursorY;
    f32 yOrigin = orgCursorY;
    const T* prevStreamPos = str;
    const T* prevNewLinePos = str;
    bool isCharSpace = false;
    const bool isUseLimit = m_WidthLimit < WIDTH_LIMIT_NONE;
    f32 textWidth = AdjustCursor(&xOrigin, &yOrigin, str, length);
    const f32 yCursorAdjust = orgCursorY - m_CursorY;
    PrintContext<T> context = {this, str, xOrigin, yOrigin, 0};
    // (the width of a character is checked from the line start)
    f32 limitOrigin = 0.0f;
    f32 x = 0.0f;
    CharStrmReader reader = m_pFont->GetCharStrmReader(T());

    reader.m_pCharStrm = str;
    u16 code = ReadNextChar(&reader);
    while (static_cast<const T*>(reader.m_pCharStrm) - str <= length) {
        if (code < ' ') {
            context.str = static_cast<const T*>(reader.m_pCharStrm);
            context.flags = isCharSpace ? 0 : TagProcessorBase<T>::PRINT_FLAG_CHAR_SPACE_NOT_NEEDED;
            if (isUseLimit && code != '\n' && prevStreamPos != prevNewLinePos) {
                // a tag that goes beyond the limit starts a new line
                PrintContext<T> context2 = context;
                TextWriterBase<T> writer(*this);
                ut::Rect rect(0.0f, 0.0f, 0.0f, 0.0f);
                context2.writer = &writer;
                m_pTagProcessor->CalcRect(&rect, code, &context2);
                if (rect.GetWidth() > 0.0f && writer.m_CursorX - context.xOrigin > m_WidthLimit) {
                    code = '\n';
                    reader.m_pCharStrm = prevStreamPos;
                    continue;
                }
            }
            const typename TagProcessorBase<T>::Operation operation = m_pTagProcessor->Process(code, &context);
            x = m_CursorX - xOrigin;
            if (operation == TagProcessorBase<T>::OPERATION_NEXT_LINE) {
                // the next line is aligned
                switch (m_DrawFlag & HORIZONTAL_ALIGN_MASK) {
                case HORIZONTAL_ALIGN_CENTER: {
                    const int remain = length - (context.str - str);
                    const f32 lineWidth = CalcLineWidth(context.str, remain);
                    m_CursorX = context.xOrigin + (std::ceil(textWidth * 0.5f) - std::ceil(lineWidth * 0.5f));
                    break;
                }
                case HORIZONTAL_ALIGN_RIGHT: {
                    const int remain = length - (context.str - str);
                    const f32 lineWidth = CalcLineWidth(context.str, remain);
                    m_CursorX = context.xOrigin + (textWidth - lineWidth);
                    break;
                }
                default: {
                    const f32 width = m_CursorX - context.xOrigin;
                    m_CursorX = context.xOrigin;
                    textWidth = width <= textWidth ? textWidth : width;
                    break;
                }
                }
                if (isUseLimit) {
                    prevNewLinePos = static_cast<const T*>(reader.m_pCharStrm);
                }
                isCharSpace = false;
            } else if (operation == TagProcessorBase<T>::OPERATION_NO_CHAR_SPACE) {
                isCharSpace = false;
            } else if (operation == TagProcessorBase<T>::OPERATION_CHAR_SPACE) {
                isCharSpace = true;
            } else if (operation == TagProcessorBase<T>::OPERATION_END_DRAW) {
                break;
            }
            reader.m_pCharStrm = context.str;
        } else {
            f32 newX = x;
            if (isCharSpace) {
                newX += m_CharSpace;
            }
            const f32 baseY = m_CursorY;
            if (m_IsWidthFixed) {
                newX += m_FixedWidth;
            } else {
                newX += m_pFont->GetCharWidth(code) * m_ScaleH;
            }
            if (isUseLimit && prevStreamPos != prevNewLinePos && newX - limitOrigin > m_WidthLimit) {
                code = '\n';
                reader.m_pCharStrm = prevStreamPos;
                continue;
            }
            x = newX;
            if (isCharSpace) {
                m_CursorX += m_CharSpace;
            }
            isCharSpace = true;
            // the characters stand on the baseline
            m_CursorY += -m_pFont->GetBaselinePos() * m_ScaleV;
            CharWriter::Print(code);
            m_CursorY = baseY;
        }
        if (isUseLimit) {
            prevStreamPos = static_cast<const T*>(reader.m_pCharStrm);
        }
        code = ReadNextChar(&reader);
    }

    const f32 width = m_CursorX - context.xOrigin;
    if ((m_DrawFlag & VERTICAL_ORIGIN_MASK) == VERTICAL_ORIGIN_MIDDLE || (m_DrawFlag & VERTICAL_ORIGIN_MASK) == VERTICAL_ORIGIN_BOTTOM) {
        m_CursorY = orgCursorY;
    } else {
        m_CursorY += yCursorAdjust;
    }
    return textWidth >= width ? textWidth : width;
}

// 0x007E9D2C slot 0x08
template TagProcessorBase<char>::Operation TagProcessorBase<char>::Process(u16 code, PrintContext<char>* pContext);
// 0x007E9E04 slot 0x0C
template TagProcessorBase<char>::Operation TagProcessorBase<char>::CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext<char>* pContext);
// 0x007E9FD8 slot 0x00
// 0x007E9FD4 (deleting dtor)
template TagProcessorBase<char>::~TagProcessorBase();
// 0x007E9FDC slot 0x08
template TagProcessorBase<wchar_t>::Operation TagProcessorBase<wchar_t>::Process(u16 code, PrintContext<wchar_t>* pContext);
// 0x007EA0B4 slot 0x0C
template TagProcessorBase<wchar_t>::Operation TagProcessorBase<wchar_t>::CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext<wchar_t>* pContext);
// 0x007EA288 slot 0x00
// 0x007EA284 (deleting dtor)
template TagProcessorBase<wchar_t>::~TagProcessorBase();

// 0x007E9CE8 | libgarden [tier A]
template TextWriterBase<wchar_t>::TextWriterBase();
// 0x004D6EEC | nintendogs:callseq-callee [tier C]
template TextWriterBase<wchar_t>::~TextWriterBase();
// 0x007E97A8 | libgarden [tier A]
template f32 TextWriterBase<wchar_t>::Print(const wchar_t* str, int length) const;
// 0x007E9804 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
template f32 TextWriterBase<wchar_t>::PrintImpl(const wchar_t* str, int length);
// 0x007E908C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template f32 TextWriterBase<wchar_t>::AdjustCursor(f32* pX, f32* pY, const wchar_t* str, int length);
// 0x007E925C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template f32 TextWriterBase<wchar_t>::CalcLineWidth(const wchar_t* str, int length);
// 0x007E92D8 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
template bool TextWriterBase<wchar_t>::CalcLineRectImpl(nw::ut::Rect* pRect, const wchar_t** pStr, int length);
// 0x007E96C0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template void TextWriterBase<wchar_t>::CalcStringRectImpl(nw::ut::Rect* pRect, const wchar_t* str, int length);

} // namespace font
} // namespace nw
