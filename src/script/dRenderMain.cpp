#include "script/dRenderMain.h"
#include <new>

namespace script {
namespace {
// the s16 parameter of a position tag in pixels (1/32 pixels, rounded away from 0)
inline f32 ToPixelsRounded(s16 value)
{
    f32 f = value;
    return (f >= 0.0f ? f + 0.5f : f - 0.5f) * (1.0f / 32);
}
} // namespace

// 0x005D6D6C (name is ours)
RenderBase::Operation RenderMain::ProcessTag(Context* ctx)
{
    static const s32 GROUP_SYSTEM = 0;
    static const s32 GROUP_RENDER = 0xFF;

    Tag tag = GetContextTag(ctx);
    Operation operation = OPERATION_DEFAULT;
    if (tag.group == GROUP_RENDER) {
        switch (tag.index) {
        case 0:
            // a ruby base: the cursor and the width
            if (m_Offset < m_Size && m_pStore != NULL) {
                Store* store = &m_pStore[m_Offset];
                store->x = ctx->writer->m_CursorX;
                store->width = ToPixelsRounded(tag.args[0]);
                m_Offset++;
            }
            break;
        case 1:
            // the cursor
            ctx->writer->m_CursorX = static_cast<s16>(tag.args[0]) * (1.0f / 32);
            SkipTag(ctx, tag);
            return operation;
        case 6:
            operation = ProcessRgbaTag(ctx, &tag);
            return operation;
        }
    } else if (tag.group == GROUP_SYSTEM) {
        switch (tag.index) {
        case 2: {
            // the font scale in percent
            f32 scale = tag.args[0] * 0.01f;
            ctx->writer->m_ScaleH = m_FontSize.x * scale;
            ctx->writer->m_ScaleV = m_FontSize.y * scale;
            break;
        }
        case 3:
            ProcessColorTag(ctx, tag);
            break;
        }
    }
    SkipTag(ctx, tag);
    return operation;
}

// 0x005D6FE0 (name is ours)
RenderBase::Operation RenderMain::ProcessRgbaTag(Context* ctx, const Tag* tag)
{
    u32 rgba = (tag->args[0] << 16) | tag->args[1];
    ctx->writer->m_TextColors[0] = nw::ut::Color8(rgba >> 24, rgba >> 16, rgba >> 8, rgba);
    ctx->writer->UpdateVertexColors();
    SkipTag(ctx, *tag);
    return OPERATION_DEFAULT;
}

inline void RenderMain::BeginLineScale(u16 code, Context* ctx)
{
    m_SavedScale.x = -1.0f;
    m_SavedScale.y = -1.0f;
    if (code == '\n' && m_FontSize.x >= 0.0f && m_FontSize.y >= 0.0f) {
        m_SavedScale.x = ctx->writer->m_ScaleH;
        m_SavedScale.y = ctx->writer->m_ScaleV;
        ctx->writer->m_ScaleH = m_FontSize.x;
        ctx->writer->m_ScaleV = m_FontSize.y;
    }
}

inline void RenderMain::EndLineScale(Context* ctx)
{
    if (m_SavedScale.x >= 0.0f) {
        ctx->writer->m_ScaleH = m_SavedScale.x;
        ctx->writer->m_ScaleV = m_SavedScale.y;
    }
}

// 0x005D704C slot 0x08
RenderBase::Operation RenderMain::Process(u16 code, Context* ctx)
{
    if (code == Tag::TAG_BEGIN) {
        return ProcessTag(ctx);
    }
    if (m_IsEndAtLineFeed && code == '\n') {
        return OPERATION_END_DRAW;
    }
    BeginLineScale(code, ctx);
    Operation operation;
    switch (code) {
    case '\t':
        ProcessTab(ctx);
        operation = OPERATION_NO_CHAR_SPACE;
        break;
    case '\n':
        ProcessLinefeed(ctx);
        operation = OPERATION_NEXT_LINE;
        break;
    default:
        operation = OPERATION_DEFAULT;
        break;
    }
    EndLineScale(ctx);
    return operation;
}

// 0x005D71C8 slot 0x0C
RenderBase::Operation RenderMain::CalcRect(nw::ut::Rect* pRect, u16 code, Context* ctx)
{
    if (code == Tag::TAG_BEGIN) {
        Tag tag = GetContextTag(ctx);
        if (tag.group == 0 && tag.index == 2) {
            f32 scale = tag.args[0] * 0.01f;
            ctx->writer->m_ScaleH = m_FontSize.x * scale;
            ctx->writer->m_ScaleV = m_FontSize.y * scale;
        }
        SkipTag(ctx, tag);
        return OPERATION_DEFAULT;
    }
    if (m_IsEndAtLineFeed && code == '\n') {
        return OPERATION_END_DRAW;
    }
    BeginLineScale(code, ctx);
    Operation operation = TagProcessorBase<wchar_t>::CalcRect(pRect, code, ctx);
    EndLineScale(ctx);
    return operation;
}

// 0x005D7338 | libgarden [tier A]
RenderMain::RenderMain(u32 textLength, u32 storeNum, u32 tagNum)
    : RenderBase(textLength, tagNum), m_Offset(0), m_pStore(NULL), m_Size(storeNum * 2), m_IsEndAtLineFeed(0),
      m_SavedScale(-1.0f, -1.0f)
{
    if (m_Size != 0) {
        m_pStore = new (std::nothrow) Store[m_Size];
    }
    ResetStore();
}

// 0x005D7434 slot 0x00
// 0x005D73D8 (deleting dtor)
RenderMain::~RenderMain()
{
    delete[] m_pStore;
    m_pStore = NULL;
}
} // namespace script
