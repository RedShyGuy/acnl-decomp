#include "lyt/dTextAreaTagProcessor.h"

#include "lyt/dTextArea.h"
#include "nw/font/font_TextWriterBase.h"

// 0x00507C7C
s32 lyt::TextArea::WriteColorTag(wchar_t* dst, s32 space, nw::ut::Color8* color)
{
    if (space - 1 < 3) {
        return 0;
    }
    // (black with alpha 0 is the end of a string; it is written as an invisible near-black)
    if (color->ToU32() == 0) {
        color->r = 1;
        color->g = 1;
        color->b = 1;
        color->a = 0;
    }
    dst[0] = CHAR_COLOR;
    dst[1] = static_cast<u16>(color->ToU32());
    dst[2] = static_cast<u16>(color->ToU32() >> 16);
    return 3;
}

// 0x00507D30 slot 0x08
lyt::TextAreaTagProcessor::Operation lyt::TextAreaTagProcessor::Process(u16 code, nw::font::PrintContext<wchar_t>* pContext)
{
    switch (code) {
    case TextArea::CHAR_COLOR: {
        const u32 value = (static_cast<u32>(pContext->str[1]) << 16) | pContext->str[0];
        const nw::ut::Color8 color(value >> 24, value >> 16, value >> 8, value);
        pContext->writer->m_GradationMode = 0;
        pContext->writer->UpdateVertexColors();
        pContext->writer->m_TextColors[0] = color;
        pContext->writer->UpdateVertexColors();
        pContext->str += 2;
        return OPERATION_DEFAULT;
    }
    case '\t':
        ProcessTab(pContext);
        return OPERATION_NO_CHAR_SPACE;
    case '\n':
        ProcessLinefeed(pContext);
        return OPERATION_NEXT_LINE;
    default:
        return OPERATION_DEFAULT;
    }
}

// 0x00507E38 slot 0x0C
lyt::TextAreaTagProcessor::Operation lyt::TextAreaTagProcessor::CalcRect(nw::ut::Rect* pRect, u16 code, nw::font::PrintContext<wchar_t>* pContext)
{
    if (code != TextArea::CHAR_COLOR) {
        return TagProcessorBase<wchar_t>::CalcRect(pRect, code, pContext);
    }
    pContext->str += 2;
    return OPERATION_DEFAULT;
}

// 0x00507E64 slot 0x00
// 0x00507E60 (deleting dtor)
lyt::TextAreaTagProcessor::~TextAreaTagProcessor()
{
    // empty (in the original too)
}
