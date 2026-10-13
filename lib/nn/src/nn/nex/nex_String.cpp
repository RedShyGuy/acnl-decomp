#include "nn/nex/nex_String.h"
#include <cstdarg>
#include <cstdlib>

namespace nn {
namespace nex {
// 0x00354EE0 | fefates:bytes [tier B]
void CopyString(wchar_t** ppCopy, const wchar_t* pString)
{
    if (pString == nullptr) {
        *ppCopy = nullptr;
        return;
    }
    u32 size = wcslen(pString) + 1;
    u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size * sizeof(wchar_t)));
    pBuffer[0] = size;
    *ppCopy = reinterpret_cast<wchar_t*>(pBuffer + 1);
    wcscpy(*ppCopy, pString);
}

// 0x003D1140 | fefates:bytes [tier B]
void nn::nex::String::ReleaseCopy(char* pCopy)
{
    ReleaseBuffer(pCopy);
}

// 0x003D1168 | fefates:bytes [tier B]
void nn::nex::String::Format(const wchar_t* pFormat, ...)
{
    wchar_t buffer[256];
    va_list args;
    va_start(args, pFormat);
    vswprintf(buffer, 256, pFormat, args);
    va_end(args);
    *this = buffer;
}

// 0x003D11A4 | fefates:bytes [tier B]
bool nn::nex::String::IsEqual(const wchar_t* pA, const wchar_t* pB)
{
    if (pA == pB) {
        return true;
    }
    if (pA == nullptr) {
        return IsEqual(L"", pB);
    }
    if (pB == nullptr) {
        return IsEqual(pA, L"");
    }
    return wcscmp(pA, pB) == 0;
}

// 0x003D11F4 | fefates:bytes [tier B]
void nn::nex::String::Reserve(int size)
{
    ReleaseBuffer(m_pString);
    u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size * sizeof(wchar_t)));
    pBuffer[0] = size;
    m_pString = reinterpret_cast<wchar_t*>(pBuffer + 1);
    m_pString[0] = L'\0';
}

// 0x003D1250 | mk7dlp:bytes-fuzzy [tier A]
nn::nex::String::String(const char* pString)
{
    CopyUtf8String(&m_pString, pString);
}

// 0x003D12AC | mk7dlp:callgraph [tier A]
nn::nex::String::String(const wchar_t* pString)
{
    CopyString(&m_pString, pString);
}

// 0x003D12D8 | fefates:bytes [tier B]
nn::nex::String::String(const nn::nex::String& other)
{
    CopyString(&m_pString, other.m_pString);
}

// 0x003D1300 | fefates:bytes [tier B]
nn::nex::String::String()
{
    CopyString(&m_pString, L"");
}

// 0x003D135C | fefates:bytes
// 0x003D132C (deleting dtor)
nn::nex::String::~String()
{
    ReleaseBuffer(m_pString);
}

// 0x003D13A4 | mk7dlp:callseq [tier A]
nn::nex::String& nn::nex::String::operator=(const char* pString)
{
    ReleaseBuffer(m_pString);
    CopyUtf8String(&m_pString, pString);
    return *this;
}

// 0x003D1424 | fefates:bytes [tier B]
nn::nex::String& nn::nex::String::operator=(const wchar_t* pString)
{
    if (m_pString == pString) {
        return *this;
    }
    ReleaseBuffer(m_pString);
    CopyString(&m_pString, pString);
    return *this;
}

// 0x003D1484 | fefates:bytes [tier B]
nn::nex::String& nn::nex::String::operator=(const nn::nex::String& other)
{
    if (m_pString != other.m_pString) {
        ReleaseBuffer(m_pString);
        CopyString(&m_pString, other.m_pString);
    }
    return *this;
}

// 0x003D14DC | fefates:bytes [tier B]
nn::nex::String& nn::nex::String::operator+=(const nn::nex::String& other)
{
    *this = *this + other;
    return *this;
}

// 0x0072E178 | fefates:bytes [tier B]
void nn::nex::String::CreateCopy(char** ppCopy) const
{
    const wchar_t* pString = m_pString;
    if (pString == nullptr) {
        *ppCopy = nullptr;
        return;
    }
    u32 size = wcslen(pString) + 1;
    u32* pBuffer = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + size));
    pBuffer[0] = size;
    *ppCopy = reinterpret_cast<char*>(pBuffer + 1);
    StringConversion::T2Char8(pString, *ppCopy, size);
}

// 0x0072E1C8 | fefates:bytes [tier B]
int nn::nex::String::FindSubstringNoCase(const wchar_t* pString) const
{
    int index = -1;
    if (m_pString != nullptr && pString != nullptr && m_pString[0] != L'\0' && pString[0] != L'\0') {
        String string(m_pString);
        string.ToUpper();
        String substring(pString);
        substring.ToUpper();
        const wchar_t* pFound = wcsstr(string.m_pString, substring.m_pString);
        if (pFound != nullptr) {
            index = pFound - string.m_pString;
        }
    }
    return index;
}

// 0x0072E2CC | fefates:bytes [tier B]
unsigned long long nn::nex::String::ToUInt64() const
{
    char* pCopy;
    CreateCopy(&pCopy);
    char* pEnd = nullptr;
    unsigned long long value = strtoull(pCopy, &pEnd, 10);
    ReleaseCopy(pCopy);
    return value;
}

// 0x0072E360 | mk7dlp:callgraph [tier A]
u32 nn::nex::String::GetLength() const
{
    if (m_pString == nullptr) {
        return 0;
    }
    return wcslen(m_pString);
}

// 0x0072E374 | fefates:bytes [tier B]
bool nn::nex::String::operator<(const nn::nex::String& other) const
{
    if (m_pString == other.m_pString) {
        return false;
    }
    if (m_pString == nullptr) {
        return String(L"") < other;
    }
    if (other.m_pString == nullptr) {
        return *this < String(L"");
    }
    return wcscmp(m_pString, other.m_pString) < 0;
}

// 0x003DA184 | fefates:bytes [tier B]
nn::nex::String operator+(const wchar_t* pA, const nn::nex::String& b)
{
    if (pA == nullptr || pA[0] == L'\0') {
        return String(b.m_pString);
    }
    u32 lengthB;
    if (b.m_pString == nullptr || (lengthB = wcslen(b.m_pString)) == 0) {
        return String(pA);
    }
    u32 lengthA = wcslen(pA);
    String result;
    result.Reserve(lengthA + lengthB + 1);
    wcscpy(result.m_pString, pA);
    wcscpy(result.m_pString + lengthA, b.m_pString);
    return result;
}

// 0x003DA284 | fefates:bytes [tier B]
nn::nex::String operator+(const nn::nex::String& a, const wchar_t* pB)
{
    u32 lengthA;
    if (a.m_pString == nullptr || (lengthA = wcslen(a.m_pString)) == 0) {
        return String(pB);
    }
    if (pB == nullptr || pB[0] == L'\0') {
        return String(a.m_pString);
    }
    u32 lengthB = wcslen(pB);
    String result;
    result.Reserve(lengthA + lengthB + 1);
    wcscpy(result.m_pString, a.m_pString);
    wcscpy(result.m_pString + lengthA, pB);
    return result;
}

// 0x003DA388 | fefates:bytes [tier B]
nn::nex::String operator+(const nn::nex::String& a, const nn::nex::String& b)
{
    u32 lengthA;
    if (a.m_pString == nullptr || (lengthA = wcslen(a.m_pString)) == 0) {
        return String(b.m_pString);
    }
    u32 lengthB;
    if (b.m_pString == nullptr || (lengthB = wcslen(b.m_pString)) == 0) {
        return String(a.m_pString);
    }
    String result;
    result.Reserve(lengthA + lengthB + 1);
    wcscpy(result.m_pString, a.m_pString);
    wcscpy(result.m_pString + lengthA, b.m_pString);
    return result;
}

} // namespace nex
} // namespace nn
