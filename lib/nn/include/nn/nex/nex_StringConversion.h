#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// Conversions between UTF-8 and the wide characters (UCS-2) of nex. They write at most size
// characters including the terminator and return the number of characters (-1 for an invalid
// one).
class StringConversion
{
public:
    static int Char8_2T(const char* pSource, wchar_t* pDestination, unsigned int size); // 0x00528FD0 | mk7dlp:callseq-callee [tier B]
    // (name is ours)
    static int T2Char8(const wchar_t* pSource, char* pDestination, unsigned int size); // 0x00528F0C
};
} // namespace nex
} // namespace nn
