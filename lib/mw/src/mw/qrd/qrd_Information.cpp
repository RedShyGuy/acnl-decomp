#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00167DE0
s32 DecodeVersion(s32 bits)
{
    // the version information codes of versions 7 to 40 (BCH(18, 6))
    const s32 VERSION_CODES[34] = {
        0x07C94, 0x085BC, 0x09A99, 0x0A4D3, 0x0BBF6, 0x0C762, 0x0D847, 0x0E60D, 0x0F928, 0x10B78, 0x1145D, 0x12A17,
        0x13532, 0x149A6, 0x15683, 0x168C9, 0x177EC, 0x18EC4, 0x191E1, 0x1AFAB, 0x1B08E, 0x1CC1A, 0x1D33F, 0x1ED75,
        0x1F250, 0x209D5, 0x216F0, 0x228BA, 0x2379F, 0x24B0B, 0x2542E, 0x26A64, 0x27541, 0x28C69,
    }; // 0x008BD9C4
    s32 version = 0;

    Yield();
    for (s32 i = 0; i < 34; i++)
    {
        if (VERSION_CODES[i] == bits)
        {
            return i + 7;
        }

        const s32 BIT_COUNTS[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4}; // 0x008BDA4C
        u32 diff = VERSION_CODES[i] ^ bits;
        s32 distance = BIT_COUNTS[diff & 15] + BIT_COUNTS[(diff >> 4) & 15] + BIT_COUNTS[(diff >> 8) & 15] +
                       BIT_COUNTS[(diff >> 12) & 15] + BIT_COUNTS[(diff >> 16) & 15] + BIT_COUNTS[(diff >> 20) & 15] +
                       BIT_COUNTS[(diff >> 24) & 15] + BIT_COUNTS[diff >> 28];
        // (never true: only an exact match is taken)
        if (distance < -1)
        {
            version = i + 7;
        }
    }
    return version;
}

// 0x00170188
s32 DecodeFormat(s32 bits, s8* maskPattern, u8* errorCorrectionLevel)
{
    u32 code = bits ^ 0x5412;
    s32 best = 0;

    Yield();
    s32 minDistance = 0x7FFFFFFF;
    for (s32 i = 0; i < 32; i++)
    {
        if (s_FormatCodes[i].code == code)
        {
            *errorCorrectionLevel = (s_FormatCodes[i].info >> 3) & 3;
            *maskPattern = s_FormatCodes[i].info & 7;
            return 1;
        }

        u32 diff = s_FormatCodes[i].code ^ code;
        s32 distance = s_NibbleBitCounts[diff & 15] + s_NibbleBitCounts[(diff >> 4) & 15] +
                       s_NibbleBitCounts[(diff >> 8) & 15] + s_NibbleBitCounts[(diff >> 12) & 15] +
                       s_NibbleBitCounts[(diff >> 16) & 15] + s_NibbleBitCounts[(diff >> 20) & 15] +
                       s_NibbleBitCounts[(diff >> 24) & 15] + s_NibbleBitCounts[diff >> 28];
        if (distance < minDistance)
        {
            minDistance = distance;
            best = s_FormatCodes[i].info;
        }
    }

    // up to three wrong bits can be corrected
    if (minDistance > 3)
    {
        return 0;
    }
    *errorCorrectionLevel = (best >> 3) & 3;
    *maskPattern = best & 7;
    return 1;
}

} // namespace detail
} // namespace qrd
} // namespace mw
