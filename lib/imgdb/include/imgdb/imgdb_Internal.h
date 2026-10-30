#pragma once

// helpers for the DCF names of several imgdb files (names are ours)

#include "imgdb/imgdb_Types.h"

namespace imgdb {
namespace detail {

inline bool IsLower(int c)
{
    return c >= 'a' && c <= 'z';
}

// a name of the buffer (at most size characters) as a terminated wide string in upper case; its length
inline int CopyBufToStr(wchar_t* string, const char* buffer, int size)
{
    int i = 0;
    for (; i < size; i++)
    {
        s8 c = buffer[i];
        if (c == '\0')
        {
            break;
        }
        string[i] = static_cast<s8>(IsLower(c) ? c - 0x20 : c);
    }
    string[i] = L'\0';
    return i;
}

} // namespace detail
} // namespace imgdb
