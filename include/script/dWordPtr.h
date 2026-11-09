#pragma once

#include "decomp.h"
#include "script/dIWord.h"

namespace script {
// a word on a writable buffer (libgarden: WordPtr with GetWord/GetSize; the other names are ours)
// RTTI N6script7WordPtrE @ 0x008D3DD0
// vtable 0x0090B65C (vptr 0x0090B664), offset_to_top 0, 12 entries
class WordPtr : public ::script::IWord
{
public:
    DECOMP_EMIT_COPY WordPtr() : m_pBuffer(NULL), m_Size(0) {} // 0x005FDA6C
    WordPtr(char16* buffer, u32 size); // 0x003081E8 (the stub name of symbols.json is WordPtr())
    virtual ~WordPtr(); // 0x00308368 slot 0x00 | libgarden
    // 0x005FDA88 slot 0x04 (deleting dtor)
    virtual char16* GetWord(); // 0x005FDA64 slot 0x08 | libgarden
    virtual const char16* GetWord() const; // 0x0075DC78 slot 0x0C | libgarden
    virtual u32 GetSize() const; // 0x0075DC60 slot 0x10 | libgarden
    // copies a text (without tags into a word of the save data; a save text with tags clears the word)
    virtual void Copy(const IWord& src); // 0x005FD774 slot 0x14
    virtual void Append(const IWord& src); // 0x005FD5F0 slot 0x18
    virtual void Clear(); // 0x005FD9F4 slot 0x1C
    virtual void Fill(char16 c, u32 num); // 0x005FD980 slot 0x20

    // (inline)
    DECOMP_EMIT_COPY void Set(char16* buffer, u32 size) // 0x005FDA4C
    {
        m_pBuffer = buffer;
        m_Size = size;
    }
    DECOMP_EMIT_COPY void Set(const WordPtr& word) // 0x005FDA58
    {
        m_pBuffer = word.m_pBuffer;
        m_Size = word.m_Size;
    }

    char16* m_pBuffer;  // 0x4
    u32 m_Size;         // 0x8
};
ASSERT_SIZE(WordPtr, 0xC);

// the empty character of the words (name is ours)
extern const char16 WORD_NULL_CHAR;
} // namespace script
