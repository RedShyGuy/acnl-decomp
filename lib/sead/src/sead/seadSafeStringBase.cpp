#include "sead/seadSafeStringBase.h"
#include "sead/seadBufferedSafeStringBase.h"

#include <cstdarg>
#include <cstdio>

// The null characters of the strings and the functions that know their value (format and
// appendWithFormat compare with 0; the inline functions of the other files load the null character
// from memory). The template instances are in seadSafeString.cpp (the names of the files are ours).

namespace sead {
// 0x008CAF0C
template <> const char sead::SafeStringBase<char>::s_NullChar = '\0';
// 0x008CAF0E
template <> const wchar_t sead::SafeStringBase<wchar_t>::s_NullChar = L'\0';

// 0x0030F5FC | mk7dlp:callgraph
template <>
s32 sead::BufferedSafeStringBase<char>::format(const char* fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);
    char* top = getMutableStringTop();
    s32 size = mBufferSize;
    s32 length = std::vsnprintf(top, size, fmt, args);
    if (length < 0 || length >= size) {
        top[size - 1] = '\0';
        length = size - 1;
    }
    va_end(args);
    return length;
}

// 0x0054B97C | mk7dlp:bytes-fuzzy
template <>
s32 sead::BufferedSafeStringBase<char>::appendWithFormat(const char* fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);
    char* top = getMutableStringTop();
    s32 oldLength = getLength();
    char* dst = top + oldLength;
    s32 size = mBufferSize - oldLength;
    s32 length = std::vsnprintf(dst, size, fmt, args);
    if (length < 0 || length >= size) {
        dst[size - 1] = '\0';
        length = size - 1;
    }
    va_end(args);
    return length + oldLength;
}
} // namespace sead
