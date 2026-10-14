#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StringConversion.h"
#include <cstring>
#include <cwchar>
#include <cwctype>

namespace nn {
namespace nex {
// A copy of a string (CopyString): MemoryManager memory with the number of characters (with the
// terminator) in the word in front of them.
DECOMP_NOINLINE void CopyString(wchar_t** ppCopy, const wchar_t* pString); // 0x00354EE0 | fefates:bytes [tier B]

// A string of wide characters (a copy, null for none). The member name and the inline helpers are
// ours.
// RTTI N2nn3nex6StringE @ 0x008CF5DC
// vtable 0x008FFA78 (vptr 0x008FFA80), offset_to_top 0, 2 entries
class String : public ::nn::nex::RootObject
{
public:
    String(); // 0x003D1300 | fefates:bytes [tier B] (symbols.json names it nn::boss::TaskIdList::TaskIdList)
    String(const char* pString); // 0x003D1250 | mk7dlp:bytes-fuzzy [tier A]
    String(const wchar_t* pString); // 0x003D12AC | mk7dlp:callgraph [tier A]
    String(const nn::nex::String& other); // 0x003D12D8 | fefates:bytes [tier B]
    DECOMP_NOINLINE virtual ~String(); // 0x003D135C slot 0x00 | fefates:bytes
    // 0x003D132C slot 0x04 (deleting dtor)

    // frees a copy of CreateCopy
    static void ReleaseCopy(char* pCopy); // 0x003D1140 | fefates:bytes [tier B]
    // (at most 255 characters)
    void Format(const wchar_t* pFormat, ...); // 0x003D1168 | fefates:bytes [tier B]
    // (null is the empty string)
    static bool IsEqual(const wchar_t* pA, const wchar_t* pB); // 0x003D11A4 | fefates:bytes [tier B]
    // an empty string with room for size characters (with the terminator)
    void Reserve(int size); // 0x003D11F4 | fefates:bytes [tier B]
    nn::nex::String& operator=(const char* pString); // 0x003D13A4 | mk7dlp:callseq [tier A]
    nn::nex::String& operator=(const wchar_t* pString); // 0x003D1424 | fefates:bytes [tier B]
    nn::nex::String& operator=(const nn::nex::String& other); // 0x003D1484 | fefates:bytes [tier B]
    nn::nex::String& operator+=(const nn::nex::String& other); // 0x003D14DC | fefates:bytes [tier B]
    // a copy as UTF-8 (null for none); ReleaseCopy frees it
    void CreateCopy(char** ppCopy) const; // 0x0072E178 | fefates:bytes [tier B]
    // the index of pString in the string ignoring the case, -1 if it is not in it
    int FindSubstringNoCase(const wchar_t* pString) const; // 0x0072E1C8 | fefates:bytes [tier B]
    unsigned long long ToUInt64() const; // 0x0072E2CC | fefates:bytes [tier B]
    u32 GetLength() const; // 0x0072E360 | mk7dlp:callgraph [tier A]
    // (inline where the original inlines GetLength)
    u32 GetLengthInline() const { return m_pString != nullptr ? wcslen(m_pString) : 0; }
    // (null is the empty string)
    bool operator<(const nn::nex::String& other) const; // 0x0072E374 | fefates:bytes [tier B]

    // (inline) frees a copy of CopyString / CreateCopy
    static DECOMP_ALWAYS_INLINE void ReleaseBuffer(void* pCopy)
    {
        if (pCopy != nullptr) {
            MemoryManager::FreeInline(static_cast<u32*>(pCopy) - 1);
        }
    }
    // (inline) a copy of a UTF-8 string
    static DECOMP_ALWAYS_INLINE void CopyUtf8String(wchar_t** ppCopy, const char* pString)
    {
        if (pString == nullptr) {
            *ppCopy = nullptr;
            return;
        }
        u32 size = strlen(pString) + 1;
        u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size * sizeof(wchar_t)));
        pBuffer[0] = size;
        *ppCopy = reinterpret_cast<wchar_t*>(pBuffer + 1);
        StringConversion::Char8_2T(pString, *ppCopy, size);
    }
    // (inline)
    void ToUpper()
    {
        if (m_pString == nullptr) {
            return;
        }
        for (wchar_t* p = m_pString; *p != L'\0'; p++) {
            *p = towupper(*p);
        }
    }

    wchar_t* m_pString; // 0x04
};
ASSERT_SIZE(String, 0x8);

nn::nex::String operator+(const wchar_t* pA, const nn::nex::String& b); // 0x003DA184 | fefates:bytes [tier B]
nn::nex::String operator+(const nn::nex::String& a, const wchar_t* pB); // 0x003DA284 | fefates:bytes [tier B]
nn::nex::String operator+(const nn::nex::String& a, const nn::nex::String& b); // 0x003DA388 | fefates:bytes [tier B]
} // namespace nex
} // namespace nn
