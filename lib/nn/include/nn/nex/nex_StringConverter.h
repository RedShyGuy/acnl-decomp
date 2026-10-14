#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// A UTF-8 copy of a string (with a length word in front, like the copies of String). The member
// names and the names of the inline accessors are ours.
class StringConverter
{
public:
    // room for length characters (three bytes each)
    explicit StringConverter(int length); // 0x0037DD58 | mk7dlp:bytes [tier A]
    ~StringConverter(); // 0x0037DD90 | mk7dlp:callgraph [tier A]
    // the UTF-8 of the string (the last byte cleared with isTerminated)
    void Encode(const wchar_t* pString, bool isTerminated); // 0x0037DC94 | mk7dlp:callgraph [tier A]
    // the UTF-8 as size characters to pString (the last one cleared with isTerminated)
    void Decode(wchar_t* pString, unsigned short size, bool isTerminated); // 0x0037DC64 | fefates:bytes [tier B]

    // (inline)
    char* GetBuffer() { return m_pBuffer; }
    u32 GetSize() const { return m_Size; }
    // (inline) frees the buffer
    void ReleaseBuffer()
    {
        if (m_pBuffer != nullptr) {
            MemoryManager::FreeInline(reinterpret_cast<u32*>(m_pBuffer) - 1);
        }
    }

    char* m_pBuffer; // 0x0
    u32 m_Size;      // 0x4 (bytes)
};
ASSERT_SIZE(StringConverter, 0x8);
} // namespace nex
} // namespace nn
