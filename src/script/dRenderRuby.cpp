#include "script/dRenderRuby.h"
#include "script/dRenderMain.h"
#include <cmath>
#include <float.h>

namespace script {
// 0x005D7C14
RenderBase::Operation RenderRuby::ProcessTag(Context* ctx)
{
    Tag tag = GetContextTag(ctx);
    Operation operation = OPERATION_DEFAULT;
    if (tag.group == 0xFF) {
        if (tag.index == 0) {
            operation = ProcessRubyPosition(ctx, &tag);
            return operation;
        }
    } else if (tag.group == 0) {
        if (tag.index == 3) {
            ProcessColorTag(ctx, tag);
        }
    }
    SkipTag(ctx, tag);
    return operation;
}

// 0x005D7D74
RenderBase::Operation RenderRuby::ProcessRubyPosition(Context* ctx, const Tag* tag)
{
    f32 width = static_cast<s16>(tag->args[0]);
    width = (width >= 0.0f ? width + 0.5f : width - 0.5f) * (1.0f / 32);
    const RenderMain* main = m_pMain;
    f32 baseX = 0.0f;
    f32 baseWidth = 0.0f;
    if (main->m_Offset < main->m_Size && m_Index < main->m_Size && main->m_Offset >= m_Index && main->m_pStore != NULL) {
        baseX = main->m_pStore[m_Index].x;
        baseWidth = main->m_pStore[m_Index].width;
    }
    ctx->writer->m_CursorX = std::floor(baseX + (baseWidth - width) * 0.5f - 0.5f + 0.5f);
    ctx->writer->m_WidthLimit = FLT_MAX;
    m_Index++;
    SkipTag(ctx, *tag);
    return OPERATION_DEFAULT;
}

// 0x005D7E68 slot 0x08
RenderBase::Operation RenderRuby::Process(u16 code, Context* ctx)
{
    if (code == Tag::TAG_BEGIN) {
        return ProcessTag(ctx);
    }
    switch (code) {
    case '\t':
        ProcessTab(ctx);
        return OPERATION_NO_CHAR_SPACE;
    case '\n':
        ProcessLinefeed(ctx);
        return OPERATION_NEXT_LINE;
    default:
        return OPERATION_DEFAULT;
    }
}

// 0x005D7F24 slot 0x0C
RenderBase::Operation RenderRuby::CalcRect(nw::ut::Rect* pRect, u16 code, Context* ctx)
{
    if (code == Tag::TAG_BEGIN) {
        Tag tag = GetContextTag(ctx);
        SkipTag(ctx, tag);
        return OPERATION_DEFAULT;
    }
    return TagProcessorBase<wchar_t>::CalcRect(pRect, code, ctx);
}

// 0x005D7FC4 slot 0x00
// 0x005D7F84 (deleting dtor)
RenderRuby::~RenderRuby()
{
    // (only the base is destroyed)
}
} // namespace script
