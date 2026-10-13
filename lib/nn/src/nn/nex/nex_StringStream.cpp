#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include <cstring>
#include <cwchar>

namespace nn {
namespace nex {
// 0x00361144 | fefates:bytes [tier B]
nn::nex::StringStream& nn::nex::StringStream::StreamNumber(unsigned char value)
{
    TestFreeRoom(NUMBER_LENGTH);
    const wchar_t* pFormat;
    if (!m_IsHex) {
        pFormat = L"%u";
    } else {
        if (m_ShowsBase) {
            wcscat(m_pCursor, L"0x");
            m_pCursor += 2;
        }
        pFormat = m_IsZeroPadded ? L"%02x" : L"%x";
    }
    m_pCursor += swprintf(m_pCursor, NUMBER_LENGTH, pFormat, value);
    return *this;
}

// 0x003611E4 | fefates:bytes [tier B]
nn::nex::StringStream& nn::nex::StringStream::StreamNumber(int value)
{
    TestFreeRoom(NUMBER_LENGTH);
    const wchar_t* pFormat;
    if (!m_IsHex) {
        pFormat = L"%d";
    } else {
        if (m_ShowsBase) {
            wcscat(m_pCursor, L"0x");
            m_pCursor += 2;
        }
        pFormat = m_IsZeroPadded ? L"%08x" : L"%x";
    }
    m_pCursor += swprintf(m_pCursor, NUMBER_LENGTH, pFormat, value);
    return *this;
}

// 0x00361284 | fefates:bytes [tier B]
nn::nex::StringStream& nn::nex::StringStream::StreamNumber(unsigned int value)
{
    TestFreeRoom(NUMBER_LENGTH);
    const wchar_t* pFormat;
    if (!m_IsHex) {
        pFormat = L"%u";
    } else {
        if (m_ShowsBase) {
            wcscat(m_pCursor, L"0x");
            m_pCursor += 2;
        }
        pFormat = m_IsZeroPadded ? L"%08x" : L"%x";
    }
    m_pCursor += swprintf(m_pCursor, NUMBER_LENGTH, pFormat, value);
    return *this;
}

// 0x00361314 | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::StringStream::TestFreeRoom(unsigned int count)
{
    wchar_t* pOld = m_pBuffer;
    u32 capacity = m_Capacity;
    int length = m_pCursor - pOld;
    u32 needed = count + length;
    if (needed <= capacity) {
        return;
    }
    while (needed > capacity) {
        capacity += capacity >> 1;
    }
    m_pBuffer = NewArray<wchar_t>(capacity);
    m_Capacity = capacity;
    memcpy(m_pBuffer, pOld, length * sizeof(wchar_t) + sizeof(wchar_t));
    m_pCursor = m_pBuffer + length;
    if (pOld != m_InlineBuffer) {
        DeleteArray(pOld);
    }
}

// 0x003613B4 | fefates:bytes [tier B]
void nn::nex::StringStream::Clear()
{
    if (m_pBuffer != m_InlineBuffer) {
        DeleteArray(m_pBuffer);
    }
    m_Capacity = INLINE_LENGTH;
    m_pBuffer = m_InlineBuffer;
    m_InlineBuffer[0] = L'\0';
    m_pCursor = m_InlineBuffer;
}

// 0x00361408 | mk7dlp:bytes [tier A]
nn::nex::StringStream::StringStream()
    : m_pBuffer(m_InlineBuffer), m_Capacity(INLINE_LENGTH), m_IsHex(false), m_ShowsBase(true), m_Unknown0x212(true)
{
    m_InlineBuffer[0] = L'\0';
    m_pCursor = m_InlineBuffer;
}

// 0x00361478 | fefates:bytes
// 0x00361448 (deleting dtor)
nn::nex::StringStream::~StringStream()
{
    if (m_pBuffer != m_InlineBuffer) {
        DeleteArray(m_pBuffer);
    }
}

// 0x003614C4 | mk7dlp:bytes-fuzzy [tier A]
nn::nex::StringStream& nn::nex::StringStream::operator<<(const void* p)
{
    TestFreeRoom(NUMBER_LENGTH);
    if (m_ShowsBase) {
        wcscat(m_pCursor, L"0x");
        m_pCursor += 2;
    }
    m_pCursor += swprintf(m_pCursor, NUMBER_LENGTH, L"%llx", static_cast<unsigned long long>(reinterpret_cast<u32>(p)));
    return *this;
}

// 0x0036153C | mk7dlp:bytes [tier A]
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::StationURL& url)
{
    return stream << url.GetURL();
}

// 0x00361568 | fefates:bytes [tier B]
nn::nex::StringStream& nn::nex::StringStream::operator<<(const wchar_t* pString)
{
    if (pString == nullptr) {
        pString = L"(null)";
    }
    u32 length = wcslen(pString);
    TestFreeRoom(length + 1);
    wcscpy(m_pCursor, pString);
    m_pCursor += length;
    return *this;
}

// 0x003615B8 (name is ours)
nn::nex::StringStream& nn::nex::StringStream::StreamNumber(unsigned long long value)
{
    TestFreeRoom(NUMBER_LENGTH);
    const wchar_t* pFormat;
    if (!m_IsHex) {
        pFormat = L"%llu";
    } else {
        if (m_ShowsBase) {
            wcscat(m_pCursor, L"0x");
            m_pCursor += 2;
        }
        pFormat = m_IsZeroPadded ? L"%016llx" : L"%llx";
    }
    m_pCursor += swprintf(m_pCursor, NUMBER_LENGTH, pFormat, value);
    return *this;
}

// 0x003DA0A0 | fefates:callgraph [tier C]
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::String& string)
{
    return stream << string.m_pString;
}

} // namespace nex
} // namespace nn
