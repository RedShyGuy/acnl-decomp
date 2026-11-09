#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"

namespace script {
class IWord;

// Builds a nickname from a message: the tags of group 6 insert single characters of a word (the first three or the
// last two), the text must start with a letter (kana, Latin, Hangul by region). Member and function names are ours.
// RTTI N6script17StrNicknameEngineE @ 0x008D3998
// vtable 0x0090A3B8 (vptr 0x0090A3C0), offset_to_top 0, 8 entries
class StrNicknameEngine : public ::script::SeqEngine
{
public:
    // a handler of the tags of group 6
    typedef void (StrNicknameEngine::*InsertFunc)();

    // (inline)
    explicit StrNicknameEngine(const IWord* pWord) : m_pWord(pWord), m_pDst(NULL), m_pLast(NULL), m_IsError(0), m_IsValid(0) {}
    virtual ~StrNicknameEngine();
    virtual void Begin(); // 0x005E8F28 slot 0x08
    virtual bool IsContinue(); // 0x005E8F34 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E880C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E8D94 slot 0x18

    // the nickname from the message into the word; true: success
    bool Make(IWord* pDst, const IWord* pMessage); // 0x005E8E18

    // the characters of the word (group 6, index 0..4)
    void InsertFirstChar(); // 0x005E89B0
    void InsertSecondChar(); // 0x005E8A94
    void InsertThirdChar(); // 0x005E8B7C
    void InsertLastChar(); // 0x005E8C64
    void InsertSecondLastChar(); // 0x005E8CFC

private:
    // the leading characters must be letters; the first letter makes the text valid
    DECOMP_NOINLINE void CheckLeadingChar(char16 c); // 0x005E8894
    // (inline) a found character of the word with the insert frame (nothing found: an error)
    void InsertChar(const char16* pChar);

public:
    const IWord* m_pWord;  // 0x8C the word of the characters
    char16* m_pDst;        // 0x90
    char16* m_pLast;       // 0x94
    u8 m_IsError;          // 0x98
    u8 m_IsValid;          // 0x99 a letter came

    static const InsertFunc s_InsertFuncs[5];
};
ASSERT_SIZE(StrNicknameEngine, 0x9C);
} // namespace script
