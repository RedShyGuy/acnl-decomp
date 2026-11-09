#pragma once

#include "decomp.h"

namespace script {
// the additional data of the fixed words (WordFix, name is ours)
struct WordInfo;

// The interface of the texts of the scripts: a buffer of UTF-16 characters with a size (libgarden: GetWord, GetWord
// const, GetSize; the other slot names are ours). The slots 0x24-0x2C have their default bodies here.
// RTTI N6script5IWordE @ 0x008D3A70
class IWord
{
public:
    IWord() {}
    virtual ~IWord() {}
    // NULL for a constant text
    virtual char16* GetWord() = 0; // slot 0x08 (name after libgarden)
    virtual const char16* GetWord() const = 0; // slot 0x0C (name after libgarden)
    virtual u32 GetSize() const = 0; // slot 0x10 (name after libgarden)
    virtual void Copy(const IWord& src) = 0; // slot 0x14
    virtual void Append(const IWord& src) = 0; // slot 0x18
    // fills the whole buffer with 0
    virtual void Clear() = 0; // slot 0x1C
    // fills num characters (0: all but the last) with c and terminates the text
    virtual void Fill(char16 c, u32 num) = 0; // slot 0x20
    virtual WordInfo* GetInfo(); // 0x005EB0A0 slot 0x24
    virtual const WordInfo* GetInfo() const; // 0x0075C8BC slot 0x28
    // the words of the save data (WordPtrSv, WordCPtrSv)
    virtual bool IsSv() const; // 0x0075C8C4 slot 0x2C

    // the number of characters before the end of the text (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetLength() const // 0x0075C7B4
    {
        u32 size = GetSize();
        const char16* text = GetWord();
        s32 count = (text + size - 1) - text + 1;
        u32 length = 0;
        for (s32 i = 1; i < count && *text != 0; i++, text++) {
            length++;
        }
        return length;
    }
    // whether the text has tags (name is ours)
    DECOMP_EMIT_COPY bool HasTag() const; // 0x0075C82C
};
ASSERT_SIZE(IWord, 0x4);
} // namespace script
