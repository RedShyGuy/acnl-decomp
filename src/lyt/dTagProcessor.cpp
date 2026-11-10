#include "lyt/dTagProcessor.h"

#include "nw/font/font_TextWriterBase.h"

namespace {
// the scale of the writer before the scale tag of group 1 (ProcessScaleTag / CalcRectScaleTag)
// 0x0095F1C0
f32 s_SavedScaleProcess[2];
// 0x0095F1C8
f32 s_SavedScaleCalcRect[2];

// the codes of the tags in the texts
const u16 CODE_TAG_BEGIN = 0x0E;
const u16 CODE_TAG_END = 0x0F;

// (inline) the scale of the writer is changed by the percent behind the tag, or restored at its end
DECOMP_ALWAYS_INLINE void ApplyScaleTag(nw::font::PrintContext<wchar_t>* pContext, bool scaleH, bool scaleV, bool isBegin, f32* saved)
{
    nw::font::TextWriterBase<wchar_t>* pWriter = pContext->writer;
    if (isBegin) {
        f32 h = pWriter->m_ScaleH;
        f32 v = pWriter->m_ScaleV;
        saved[0] = h;
        saved[1] = v;
        const u16 percent = *pContext->str;
        pContext->str++;
        if (scaleH) {
            h *= percent * 0.01f;
        }
        if (scaleV) {
            v *= percent * 0.01f;
        }
        pWriter->m_ScaleH = h;
        pWriter->m_ScaleV = v;
    } else {
        pWriter->m_ScaleH = saved[0];
        pWriter->m_ScaleV = saved[1];
    }
}
} // namespace

namespace lyt {
// 0x005019FC slot 0x14
void lyt::TagProcessor::CalcRectTag(nw::ut::Rect* pRect, PrintContext* pContext, bool isBegin)
{
    const wchar_t* str = pContext->str;
    pContext->str = str + (isBegin ? 3 : 2);
    const u16 group = str[0];
    if (group == 0) {
        const u16 index = str[1];
        if (index == 2) {
            pContext->str += 1;
        } else if (index == 3) {
            pContext->str += 2;
        }
    } else if (group == 1) {
        CalcRectScaleTag(pRect, pContext, str[1], isBegin);
    }
}

// 0x00501A64
void lyt::TagProcessor::CalcRectScaleTag(nw::ut::Rect*, PrintContext* pContext, u16 index, bool isBegin)
{
    bool scaleH = false;
    bool scaleV = false;
    if (index == 0) {
        scaleH = true;
        scaleV = true;
    } else if (index == 1) {
        scaleH = true;
    } else if (index == 2) {
        scaleV = true;
    }
    ApplyScaleTag(pContext, scaleH, scaleV, isBegin, s_SavedScaleCalcRect);
}

// 0x00501B20
void lyt::TagProcessor::ProcessColorTag(PrintContext* pContext)
{
    const u8* rgba = reinterpret_cast<const u8*>(pContext->str);
    nw::ut::Color8 color;
    if (rgba[0] == 0 && rgba[1] == 0 && rgba[2] == 0 && rgba[3] == 0xFF) {
        color = m_SavedColor;
    } else {
        m_SavedColor = pContext->writer->m_TextColors[0];
        color = nw::ut::Color8(rgba[0], rgba[1], rgba[2], rgba[3]);
    }
    nw::font::TextWriterBase<wchar_t>* pWriter = pContext->writer;
    pWriter->m_TextColors[0] = color;
    pWriter->m_TextColors[1] = color;
    pWriter->UpdateVertexColors();
    pContext->str += 2;
}

// 0x00501C08
void lyt::TagProcessor::ProcessScaleTag(PrintContext* pContext, bool scaleH, bool scaleV, bool isBegin)
{
    ApplyScaleTag(pContext, scaleH, scaleV, isBegin, s_SavedScaleProcess);
}

// 0x00501C94 slot 0x10
void lyt::TagProcessor::ProcessTag(PrintContext* pContext, bool isBegin)
{
    const wchar_t* str = pContext->str;
    pContext->str = str + (isBegin ? 3 : 2);
    const u16 group = str[0];
    if (group == 0) {
        const u16 index = str[1];
        if (index == 2) {
            nw::font::TextWriterBase<wchar_t>* pWriter;
            const u16 percent = *pContext->str;
            f32 scale;
            if (percent == 100) {
                scale = m_SavedScaleH;
            } else if (percent == 101) {
                m_SavedScaleH = pContext->writer->m_ScaleH;
                scale = m_SavedScaleH;
            } else {
                m_SavedScaleH = pContext->writer->m_ScaleH;
                scale = m_SavedScaleH * (percent * 0.01f);
            }
            pWriter = pContext->writer;
            pWriter->m_ScaleH = scale;
            pContext->str += 1;
        } else if (index == 3) {
            ProcessColorTag(pContext);
        }
    } else if (group == 1) {
        const u16 index = str[1];
        if (index == 0) {
            ProcessScaleTag(pContext, true, true, isBegin);
        } else if (index == 1) {
            ProcessScaleTag(pContext, true, false, isBegin);
        } else if (index == 2) {
            ProcessScaleTag(pContext, false, true, isBegin);
        }
    }
}

// 0x00501D9C slot 0x08
lyt::TagProcessor::Operation lyt::TagProcessor::Process(u16 code, PrintContext* pContext)
{
    if (code == CODE_TAG_BEGIN) {
        ProcessTag(pContext, true);
        return OPERATION_DEFAULT;
    }
    if (code == CODE_TAG_END) {
        ProcessTag(pContext, false);
        return OPERATION_DEFAULT;
    }
    if (code == '\t') {
        ProcessTab(pContext);
        return OPERATION_NO_CHAR_SPACE;
    }
    if (code == '\n') {
        ProcessLinefeed(pContext);
        return OPERATION_NEXT_LINE;
    }
    return OPERATION_DEFAULT;
}

// 0x00501E8C slot 0x0C
lyt::TagProcessor::Operation lyt::TagProcessor::CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext* pContext)
{
    if (code == CODE_TAG_BEGIN) {
        CalcRectTag(pRect, pContext, true);
        return OPERATION_DEFAULT;
    }
    if (code == CODE_TAG_END) {
        CalcRectTag(pRect, pContext, false);
        return OPERATION_DEFAULT;
    }
    return nw::font::TagProcessorBase<wchar_t>::CalcRect(pRect, code, pContext);
}

// 0x00501EF4 slot 0x00
// 0x00501EE4 (deleting dtor)
lyt::TagProcessor::~TagProcessor()
{
    // empty (in the original too)
}

} // namespace lyt
