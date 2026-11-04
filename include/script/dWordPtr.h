#pragma once

#include "decomp.h"
#include "script/dIWord.h"

namespace script {
// RTTI N6script7WordPtrE @ 0x008D3DD0
// vtable 0x0090B65C (vptr 0x0090B664), offset_to_top 0, 12 entries
class WordPtr : public ::script::IWord
{
public:
    WordPtr(); // ctor candidate 0x005FDA6C (unverified)
    WordPtr(char16* buffer, u32 size); // 0x003081E8 (the stub name of symbols.json is WordPtr())
    virtual ~WordPtr(); // 0x00308368 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x005FDA88 slot 0x04 | virtual slot, introduced by script::WordPtr
    virtual void GetWord(); // 0x005FDA64 slot 0x08 | libgarden
    virtual void GetWord() const; // 0x0075DC78 slot 0x0C | libgarden
    virtual void GetSize() const; // 0x0075DC60 slot 0x10 | libgarden
    // copies the text of another word (through a CopyEngine)
    virtual void Copy(const IWord& src); // 0x005FD774 slot 0x14 (name is ours)
    virtual void Append(const IWord& src); // 0x005FD5F0 slot 0x18 | virtual slot, introduced by script::WordPtr
    virtual void Clear(); // 0x005FD9F4 slot 0x1C | virtual slot, introduced by script::WordPtr
    virtual void vf_0x20(); // 0x005FD980 slot 0x20 | virtual slot, introduced by script::WordPtr
    virtual void vf_0x24(); // 0x005EB0A0 slot 0x24 | virtual slot, introduced by script::WordCPtr
    virtual void vf_0x28(); // 0x0075C8BC slot 0x28 | virtual slot, introduced by script::WordCPtr
    virtual void vf_0x2C(); // 0x0075C8C4 slot 0x2C | virtual slot, introduced by script::WordCPtr

protected:
    char16* m_pBuffer;  // 0x4 (names are ours)
    u32 m_Size;         // 0x8
};

// the empty character of the words (name is ours)
extern const char16 WORD_NULL_CHAR;
} // namespace script
