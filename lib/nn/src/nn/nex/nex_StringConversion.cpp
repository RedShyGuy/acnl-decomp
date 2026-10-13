#include "nn/nex/nex_StringConversion.h"

namespace nn {
namespace nex {
// 0x00528F0C (name is ours)
int nn::nex::StringConversion::T2Char8(const wchar_t* pSource, char* pDestination, unsigned int size)
{
    int count = 0;
    bool isInvalid = false;
    u32 left = size - 1;
    while (*pSource != L'\0' && left != 0 && !isInvalid) {
        int c = *pSource;
        if (c < 0x80) {
            left--;
            count++;
            *pDestination++ = c;
        } else if (c < 0x800) {
            *pDestination++ = 0xC0 | (c >> 6);
            *pDestination++ = 0x80 | (c & 0x3F);
            left -= 2;
            count += 2;
        } else if (c < 0x10000) {
            *pDestination++ = 0xE0 | (c >> 12);
            *pDestination++ = 0x80 | ((c >> 6) & 0x3F);
            *pDestination++ = 0x80 | (c & 0x3F);
            left -= 3;
            count += 3;
        } else {
            isInvalid = true;
        }
        pSource++;
    }
    *pDestination = '\0';
    return count;
}

// 0x00528FD0 | mk7dlp:callseq-callee [tier B]
int nn::nex::StringConversion::Char8_2T(const char* pSource, wchar_t* pDestination, unsigned int size)
{
    int count = 0;
    bool isInvalid = false;
    wchar_t* pCurrent = pDestination;
    u32 left = size - 1;
    while (*pSource != '\0' && left != 0) {
        if (isInvalid) {
            break;
        }
        u8 c = *pSource;
        if (c < 0x80) {
            left--;
            *pCurrent++ = static_cast<signed char>(c);
            count++;
        } else if (c >= 0xC2 && c <= 0xDF) {
            u8 c1 = *++pSource;
            left--;
            *pCurrent++ = ((c - 0xC0) << 6) + (c1 - 0x80);
            count++;
        } else if (c >= 0xE0 && c <= 0xEF) {
            u8 c1 = pSource[1];
            u8 c2 = *(pSource += 2);
            left--;
            *pCurrent++ = ((c - 0xE0) << 12) + ((c1 - 0x80) << 6) + (c2 - 0x80);
            count++;
        } else {
            // (4 byte sequences, overlong forms and invalid bytes are not supported)
            isInvalid = true;
        }
        pSource++;
    }
    if (isInvalid) {
        *pDestination = L'\0';
        return -1;
    }
    *pCurrent = L'\0';
    return count + 1;
}

} // namespace nex
} // namespace nn
