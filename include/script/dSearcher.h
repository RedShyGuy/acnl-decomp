#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// finds the first position in a text that matches its mode (member names are ours)
// RTTI N6script8SearcherE @ 0x008D3DF0
// vtable 0x0090B6CC (vptr 0x0090B6D4), offset_to_top 0, 8 entries
class Searcher : public ::script::MsgEngine
{
public:
    enum Mode : u8
    {
        MODE_CHAR = 0,        // the character m_Char
        MODE_RETURN = 1,      // a new line (else the end of the text)
        MODE_NOT_RETURN = 2,  // any character but a new line (found without a position) or a tag of the groups 1, 4, 5, 17
        MODE_TAG = 3,         // the tag m_Group, m_Index
        MODE_NONE = 4,
    };
    typedef void (Searcher::*CharFunc)(const CharInfo* info);
    typedef void (Searcher::*TagFunc)(const Tag* tag);

    Searcher(); // 0x005FDDE0 (name after the RTTI)
    virtual ~Searcher(); // 0x005FDE34 slot 0x00
    // 0x005FDE24 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x005FDDC8 slot 0x08
    virtual void End(); // 0x005FDD54 slot 0x0C
    virtual bool IsContinue(); // 0x005FDDCC slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005FDC80 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005FDD7C slot 0x18

    // the handlers of the modes
    void OnCharFind(const CharInfo* info); // 0x005FDCCC
    void OnCharReturn(const CharInfo* info); // 0x005FDD34
    void OnCharNotReturn(const CharInfo* info); // 0x005FDD20
    void OnTagGroup(const Tag* tag); // 0x005FDCF0
    void OnTagFind(const Tag* tag); // 0x005FDC50

    u8 m_Mode;                // 0x60 Mode
    u32 m_Unknown0x64;        // 0x64
    char16 m_Char;            // 0x68
    s32 m_Group;              // 0x6C
    s32 m_Index;              // 0x70
    bool m_IsFound;           // 0x74
    const char16* m_pFound;   // 0x78

    static const CharFunc s_CharFuncs[4];
    static const TagFunc s_TagFuncs[4];
};
ASSERT_SIZE(Searcher, 0x7C);

// (inline) the length in characters of the lines of text behind a tag (the tags that skip lines; name is ours)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline s32 GetLinesLength(const Tag* tag, s32 lines) // 0x0075B2B8
{
    Searcher searcher;
    const char16* begin = tag->data + tag->length / sizeof(char16);
    const char16* cur = begin;
    for (s32 i = 0; i < lines; i++) {
        CharInfo info(cur);
        cur++;
        searcher.m_Mode = Searcher::MODE_NONE;
        searcher.m_Unknown0x64 = 0;
        searcher.m_Char = 0;
        searcher.m_Group = 0x7FFFFFFF;
        searcher.m_Index = -1;
        searcher.m_IsFound = false;
        searcher.m_pFound = NULL;
        searcher.SetText(&cur, &MsgEngine::s_pNoStop);
        searcher.m_Mode = Searcher::MODE_RETURN;
        searcher.m_Unknown0x64 = reinterpret_cast<uptr>(cur);
        searcher.SetText2(&MsgEngine::s_pNoStop);
        s32 length = 0;
        if (searcher.m_IsFound) {
            length = searcher.m_pFound - reinterpret_cast<const char16*>(searcher.m_Unknown0x64);
        }
        cur += length;
    }
    return cur - begin;
}

// (inline) the length of the two lines behind a tag (a copy of GetLinesLength for two lines; name is ours)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline s32 GetLineLength(const Tag* tag) // 0x0075B834
{
    return GetLinesLength(tag, 2);
}
} // namespace script
