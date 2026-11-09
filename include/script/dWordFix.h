#pragma once

#include "decomp.h"
#include "script/dWordPtr.h"
#include <cstring>

namespace script {
// the additional data of the fixed words (libgarden: WordFix::Info with color, begin, buf; values as the code sets them)
struct WordInfo
{
    DECOMP_NOINLINE WordInfo(); // 0x005F56CC
    ~WordInfo() {}

    DECOMP_NOINLINE WordInfo& operator=(const WordInfo& info); // 0x005F565C
    DECOMP_NOINLINE void Reset(); // 0x005F56AC (name is ours)
    DECOMP_NOINLINE void SetColor(u8 color); // 0x005F56C4 (name is ours)

    // the inflection of the word from its tag 14/0 (InflectEngine); the grammar tags of SeqEngine use it
    u8 color;   // 0x0 the grammatical gender (GENDER tags: 1, 2, 3 select the 1st, 2nd, 3rd form; 4 by default = unknown)
    s32 begin;  // 0x4 the article of the ARTICLE tags of kind 1 (index of STR_Article; -1 by default)
    s32 buf;    // 0x8 the article of kind 2 (-1 by default)
};
ASSERT_SIZE(WordInfo, 0xC);

// a word with a buffer of N characters (names after libgarden; the constructor has copies in the template area
// for some sizes, the other users have it inline). Instances found in the binary: 1, 2, 4, 8-22, 24, 25, 29, 30,
// 32, 33, 36, 38, 40, 41, 45, 50, 57, 60, 64, 65, 69, 70, 75, 85, 94, 100, 109, 111, 120, 128, 188, 193, 197, 256,
// 350, 400, 450, 512, 1536.
template <u32 N>
class WordFix : public WordPtr
{
public:
    WordFix() : WordPtr(m_Buffer, N)
    {
        std::memset(m_Buffer, 0, sizeof(m_Buffer));
        for (u32 i = 0; i < N; i++) {
            m_Buffer[i] = WORD_NULL_CHAR;
        }
    }
    virtual ~WordFix() {}
    virtual void Copy(const IWord& src)
    {
        WordPtr::Copy(src);
        const WordInfo* info = src.GetInfo();
        if (info != NULL) {
            m_Info = *info;
        } else {
            m_Info.Reset();
        }
    }
    virtual void Clear()
    {
        WordPtr::Clear();
        m_Info.Reset();
    }
    virtual WordInfo* GetInfo() { return &m_Info; }
    virtual const WordInfo* GetInfo() const { return &m_Info; }

    WordInfo m_Info;        // 0x0C
    char16 m_Buffer[N];     // 0x18
};
// the sizes whose constructor the original calls (a copy in the template area); the others have it inline
template <>
WordFix<4u>::WordFix();
template <>
WordFix<9u>::WordFix();
template <>
WordFix<11u>::WordFix();
template <>
WordFix<12u>::WordFix();
template <>
WordFix<13u>::WordFix();
template <>
WordFix<20u>::WordFix();
template <>
WordFix<21u>::WordFix();
template <>
WordFix<22u>::WordFix();
template <>
WordFix<33u>::WordFix();
template <>
WordFix<36u>::WordFix();
template <>
WordFix<45u>::WordFix();
template <>
WordFix<50u>::WordFix();
template <>
WordFix<60u>::WordFix();
template <>
WordFix<64u>::WordFix();
template <>
WordFix<70u>::WordFix();
template <>
WordFix<109u>::WordFix();
} // namespace script
