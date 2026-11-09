#include "script/dChip_MeasureProcessor.h"
#include "script/dMsgEngine.h"

// 0x005EA9F0 slot 0x0C
nw::font::TagProcessorBase<wchar_t>::Operation script::Chip::MeasureProcessor::CalcRect(nw::ut::Rect* pRect, u16 code,
                                                                                        nw::font::PrintContext<wchar_t>* ctx)
{
    if (code != Tag::TAG_BEGIN) {
        return TagProcessorBase<wchar_t>::CalcRect(pRect, code, ctx);
    }
    const char16* text = reinterpret_cast<const char16*>(ctx->str) - 1;
    Tag tag = Tag::GetTag(&text);
    if (tag.group == 0 && tag.index == 2) {
        f32 scale = tag.args[0] * 0.01f;
        ctx->writer->m_ScaleH = m_Scale.x * scale;
        ctx->writer->m_ScaleV = m_Scale.y * scale;
    }
    ctx->str = reinterpret_cast<const wchar_t*>(reinterpret_cast<const u8*>(ctx->str) + (tag.length & ~1u) - sizeof(char16));
    return OPERATION_DEFAULT;
}

// 0x005EAAA8 slot 0x00
// 0x005EAAA4 (deleting dtor)
script::Chip::MeasureProcessor::~MeasureProcessor()
{
    // empty (in the original too)
}
