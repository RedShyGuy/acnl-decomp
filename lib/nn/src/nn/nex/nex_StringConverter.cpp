#include "nn/nex/nex_StringConverter.h"
#include "nn/nex/nex_StringConversion.h"

namespace nn {
namespace nex {
namespace {
// the UTF-8 bytes of a UTF-16 character
const wchar_t ONE_BYTE_LIMIT = 0x80;
const wchar_t TWO_BYTES_LIMIT = 0x800;
} // namespace

// 0x0037DC64 | fefates:bytes [tier B]
void nn::nex::StringConverter::Decode(wchar_t* pString, unsigned short size, bool isTerminated)
{
    StringConversion::Char8_2T(m_pBuffer, pString, size);
    if (isTerminated) {
        pString[size - 1] = L'\0';
    }
}

// 0x0037DC94 | mk7dlp:callgraph [tier A]
void nn::nex::StringConverter::Encode(const wchar_t* pString, bool isTerminated)
{
    ReleaseBuffer();
    u32 size = 1;
    for (const wchar_t* p = pString; *p != L'\0'; p++) {
        u32 c = static_cast<u16>(*p);
        if (c < ONE_BYTE_LIMIT) {
            size += 1;
        } else if (static_cast<int>(c) < TWO_BYTES_LIMIT) {
            size += 2;
        } else if (static_cast<int>(c) < 0x10000) {
            size += 3;
        }
    }
    m_Size = size;
    u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size));
    pBuffer[0] = size;
    m_pBuffer = reinterpret_cast<char*>(pBuffer + 1);
    StringConversion::T2Char8(pString, m_pBuffer, m_Size);
    if (isTerminated) {
        m_pBuffer[m_Size - 1] = '\0';
    }
}

// 0x0037DD58 | mk7dlp:bytes [tier A]
nn::nex::StringConverter::StringConverter(int length) : m_pBuffer(nullptr), m_Size(length * 3)
{
    if (static_cast<int>(m_Size) > 0) {
        u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + m_Size));
        pBuffer[0] = m_Size;
        m_pBuffer = reinterpret_cast<char*>(pBuffer + 1);
    }
}

// 0x0037DD90 | mk7dlp:callgraph [tier A]
nn::nex::StringConverter::~StringConverter()
{
    ReleaseBuffer();
}

} // namespace nex
} // namespace nn
