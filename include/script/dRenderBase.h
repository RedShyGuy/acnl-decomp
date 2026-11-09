#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/ut/ut_Color8.h"
#include "script/dMsgEngine.h"
#include "script/dProject.h"

namespace script {
// The text of a text box with the processor of its tags: the text buffer (written by RenderTrace), positions in it and
// a flag per position, the default colors and the font scale of the box. Member names after libgarden where it names
// them.
// RTTI N6script10RenderBaseE @ 0x008D372C
// vtable 0x00909960 (vptr 0x00909968), offset_to_top 0, 4 entries
class RenderBase : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    typedef nw::font::PrintContext<wchar_t> Context;

    // a position in the text; the first one is where the text is written (name is ours)
    struct Position
    {
        DECOMP_EMIT_COPY Position() : pText(NULL) {} // 0x005EA81C (name is ours)
        DECOMP_EMIT_COPY ~Position() {} // 0x005EA828 (name is ours)

        char16* pText; // 0x0
    };

    RenderBase(u32 length, u32 tagNum); // 0x005D6C38 (name is ours)
    virtual ~RenderBase();
    // the text is emptied, every position is the start (name is ours)
    void Clear(); // 0x005D6BA4 (name is ours)

    // (inline, name after libgarden)
    void ClearBuf2() { *m_pBuf2 = 0; }
    // (inline) every position moves one back, the first one stays (name is ours)
    void ShiftPositions()
    {
        for (s32 i = m_Count - 1; i > 0; i--) {
            m_pBuf[i] = m_pBuf[i - 1];
            m_pBuf2[i] = m_pBuf2[i - 1];
        }
    }
    // (inline, name after libgarden)
    void SetDefaultColors(const nw::ut::Color8& top, const nw::ut::Color8& bottom)
    {
        m_TextColors[0] = top;
        m_TextColors[1] = bottom;
    }

    // (inline) a character / a tag at the end of the text (names are ours)
    void Put(char16 c)
    {
        char16* next = m_pBuf->pText + 1;
        if (next <= m_pEnd) {
            *m_pBuf->pText = c;
            m_pBuf->pText = next;
        }
        *m_pBuf2 = 1;
    }
    void Put(const CharInfo* info)
    {
        PutChar(&m_pBuf->pText, &m_pEnd, info);
        *m_pBuf2 = 1;
    }
    void Put(const Tag* tag)
    {
        PutTag(&m_pBuf->pText, &m_pEnd, tag);
        *m_pBuf2 = 1;
    }

    // (inline) the length of the written text, at most the size of the buffer (name is ours)
    u32 GetTextLength() const
    {
        s32 length = m_pBuf[m_Count - 1].pText - m_pText;
        if (length < 0) {
            return 0;
        }
        if (m_Len < static_cast<u32>(length)) {
            return m_Len;
        }
        return length;
    }

protected:
    // (inline) the tag at the control character before the string of the context (name is ours)
    static Tag GetContextTag(const Context* ctx)
    {
        const char16* text = reinterpret_cast<const char16*>(ctx->str) - 1;
        return Tag::GetTag(&text);
    }
    // (inline) the string of the context behind the tag (name is ours)
    static void SkipTag(Context* ctx, const Tag& tag)
    {
        ctx->str = reinterpret_cast<const wchar_t*>(reinterpret_cast<const u8*>(ctx->str) + (tag.length & ~1u) - sizeof(char16));
    }
    // (inline) the text colors from the color tag: an index into the colors of the project, otherwise the default
    // colors (name is ours)
    void ProcessColorTag(Context* ctx, const Tag& tag);

public:
    nw::ut::Color8 m_TextColors[2]; // 0x04 the default colors (top, bottom)
    nn::math::VEC2 m_FontSize;      // 0x0C the scale of the font for the size of the text box (none: -1)
    char16* m_pText;                // 0x14
    u32 m_Len;                      // 0x18 the size of the text buffer in characters
    u32 m_Count;                    // 0x1C the number of positions
    Position* m_pBuf;               // 0x20
    u8* m_pBuf2;                    // 0x24 a flag per position (set: something was written)
    char16* m_pEnd;                 // 0x28 the end of the text buffer
};
ASSERT_SIZE(RenderBase, 0x2C);

inline void RenderBase::ProcessColorTag(Context* ctx, const Tag& tag)
{
    static const u32 COLOR_NUM = 7;
    static const u32 COLOR_NONE = 0xFFFF;

    nw::font::TextWriterBase<wchar_t>* writer = ctx->writer;
    nw::ut::Color8 top = m_TextColors[0];
    nw::ut::Color8 bottom = m_TextColors[1];
    u32 index = tag.args[0];
    if (index >= COLOR_NUM) {
        index = COLOR_NONE;
    }
    u8 rgba[4];
    if (index != COLOR_NONE && Project::s_pInstance->m_Project.GetColor(rgba, index)) {
        top.r = rgba[0];
        top.g = rgba[1];
        top.b = rgba[2];
        top.a = rgba[3];
        bottom = top;
    }
    writer->m_TextColors[0] = top;
    writer->m_TextColors[1] = bottom;
    writer->UpdateVertexColors();
}
} // namespace script
