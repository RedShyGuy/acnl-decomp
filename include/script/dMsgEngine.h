#pragma once

#include "decomp.h"

namespace script {
// a character of a text, UTF-16 decoded (name is ours)
struct CharInfo
{
    explicit CharInfo(const char16* text); // 0x005ED460

    const char16* pText;  // 0x0
    u32 units;            // 0x4 the code units (a surrogate pair: high << 16 | low)
    u32 code;             // 0x8 the code point
    u32 size;             // 0xC in bytes (2 or 4)
};
ASSERT_SIZE(CharInfo, 0x10);

// a tag of a message (LibMessageStudio: 0x0E, group, index, size of the parameters in bytes, parameters; names after
// libgarden)
struct Tag
{
    static const char16 TAG_BEGIN = 0xE;
    static const char16 TAG_END = 0xF;  // 0x0F, group, index (name is ours)

    static Tag GetTag(const char16* const* text); // 0x005D855C (name after libgarden)

    const char16* data;   // 0x00
    s32 group;            // 0x04 (none: 0x7FFFFFFF)
    s32 index;            // 0x08
    u32 paramSize;        // 0x0C in bytes
    const char16* args;   // 0x10
    u32 length;           // 0x14 in bytes, with the header
};
ASSERT_SIZE(Tag, 0x18);

// Walks through a text and calls its slots for every character and tag. Texts of inserted words are pushed on a stack
// of ranges and continued when they end. libgarden names the class RenderTrace (with SetText/SetText2); the RTTI of
// the binary calls this base MsgEngine. Slot names are ours.
// RTTI N6script9MsgEngineE @ 0x008D3E8C
// vtable 0x0090BBB0 (vptr 0x0090BBB8), offset_to_top 0, 8 entries
class MsgEngine
{
public:
    // a part of a text (name is ours)
    struct Range
    {
        DECOMP_NOINLINE Range(); // 0x0060230C

        const char16* pCur;  // 0x0
        const char16* pEnd;  // 0x4
    };

    DECOMP_NOINLINE MsgEngine(); // 0x006024F0
    virtual ~MsgEngine(); // 0x00602558 slot 0x00
    // 0x00602548 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x006024E4 slot 0x08
    virtual void End(); // 0x00602478 slot 0x0C
    virtual bool IsContinue(); // 0x006024E8 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x00602304 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x0060247C slot 0x18
    virtual void OnTagEnd(); // 0x00602308 slot 0x1C

    // starts a text at *begin (end: NULL = up to the terminating 0) and calls Begin (libgarden: RenderTrace::SetText)
    void SetText(char16_t const* const* begin, char16_t const* const* end); // 0x00602480 | libgarden [tier A]
    // runs until the text reaches *stop or IsContinue stops it (libgarden: RenderTrace::SetText2)
    void SetText2(char16_t const* const* stop); // 0x0060231C | libgarden [tier A]

    // interrupts the text with another one (the current range goes on the stack; nothing if it is full)
    void PushText(char16_t const* const* begin, char16_t const* const* end); // 0x0060224C (name is ours)
    void PushWord(const class IWord* word); // 0x006022A0 (name is ours)

    // (inline) no text, the interrupted texts are dropped (name is ours)
    void ResetText()
    {
        m_pCur = s_pNoStop;
        m_pEnd = s_pNoStop;
        while (m_StackNum != 0) {
            m_StackNum--;
            m_Stack[m_StackNum].pCur = NULL;
            m_Stack[m_StackNum].pEnd = NULL;
        }
    }

    // (inline) the whole text of a buffer
    void Run(const char16* text)
    {
        SetText(&text, &s_pNoStop);
        SetText2(&s_pNoStop);
    }

    const char16* m_pCur;     // 0x04
    const char16* m_pEnd;     // 0x08 (NULL: up to the terminating 0)
    Range m_Stack[10];        // 0x0C the interrupted texts
    s32 m_StackNum;           // 0x5C

    // the end and stop that is never reached (name is ours)
    static const char16* s_pNoStop;
};
ASSERT_SIZE(MsgEngine, 0x60);

// a pointer into a text (the element of the arrays of texts in tag parameters; name is ours)
struct TextPtr
{
    DECOMP_EMIT_COPY TextPtr() : pText(NULL) {} // 0x005EA928 (name is ours)

    const char16* pText; // 0x0
};

// write a character / a whole tag at *dst and advance it, if it fits before *last (names are ours)
bool PutChar(char16** dst, char16* const* last, const CharInfo* info); // 0x005EACEC
bool PutTag(char16** dst, char16* const* last, const Tag* tag); // 0x005EACA0
} // namespace script
