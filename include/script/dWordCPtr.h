#pragma once

#include "decomp.h"
#include "script/dIWord.h"

namespace script {
// a word on a constant text: GetWord gives NULL and the writing slots do nothing (names are ours)
// RTTI N6script8WordCPtrE @ 0x008D3E20
// vtable 0x0090BA34 (vptr 0x0090BA3C), offset_to_top 0, 12 entries
class WordCPtr : public ::script::IWord
{
public:
    DECOMP_EMIT_COPY WordCPtr() : m_pWord(NULL), m_Size(0) {} // 0x005FEA50
    DECOMP_EMIT_COPY WordCPtr(const char16* word, u32 size) : m_pWord(word), m_Size(size) {} // 0x005FEA38
    virtual ~WordCPtr(); // 0x005FEA70 slot 0x00
    // 0x005FEA6C slot 0x04 (deleting dtor)
    virtual char16* GetWord(); // 0x005EB12C slot 0x08
    virtual const char16* GetWord() const; // 0x0075DD28 slot 0x0C
    virtual u32 GetSize() const; // 0x0075DD20 slot 0x10
    virtual void Copy(const IWord& src); // 0x005EB120 slot 0x14
    virtual void Append(const IWord& src); // 0x005EB11C slot 0x18
    virtual void Clear(); // 0x005EB128 slot 0x1C
    virtual void Fill(char16 c, u32 num); // 0x005EB124 slot 0x20

    const char16* m_pWord;  // 0x4
    u32 m_Size;             // 0x8
};
ASSERT_SIZE(WordCPtr, 0xC);
} // namespace script
