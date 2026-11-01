#include "cfl/cfl_Api.h"
#include "nn/fnd/fnd_DateTime.h"

#include <cstring>

namespace {
// the days of the months 1-12 (February with the leap day)
// 0x008A4AC4
const s32 s_DaysOfMonth[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// the user ID (the transferable ID of the console) and the random generator of CFL (names are ours)
} // namespace

// 0x00166374 | nintendogs:bytes [tier B]
DECOMP_NOINLINE u16 CFLi_GetCRC16(int crc, const void* data, unsigned size)
{
    const u8* p = static_cast<const u8*>(data);
    u16 value = crc;
    for (unsigned i = 0; i < size; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (value & 0x8000) {
                value = (value << 1) ^ 0x1021;
            } else {
                value = value << 1;
            }
        }
        value ^= p[i];
    }
    return value;
}

// writes the CRC16 big endian into the last two bytes of the data
// 0x0013AA00 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_PutCRC16(int crc, void* data, unsigned size)
{
    u16* out = reinterpret_cast<u16*>(static_cast<u8*>(data) + size) - 1;
    *out = 0;
    const u8* p = static_cast<const u8*>(data);
    u16 value = crc;
    for (unsigned i = 0; i < size; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (value & 0x8000) {
                value = (value << 1) ^ 0x1021;
            } else {
                value = value << 1;
            }
        }
        value ^= p[i];
    }
    *out = __builtin_bswap16(value);
}

// copies up to length characters and fills the rest with zeros
// 0x0016E260 | nintendogs:bytes-fuzzy [tier B]
DECOMP_NOINLINE void CFLi_CopyString16(unsigned short* dst, const unsigned short* src, int length)
{
    for (int i = 0; i < length; i++) {
        dst[i] = src[i];
        if (src[i] == 0) {
            for (int j = i + 1; j < length; j++) {
                dst[j] = 0;
            }
            return;
        }
    }
}

// 0x00175AD8 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE int CFLi_GetStringLength8(const char* str)
{
    int length = 0;
    while (str[length] != 0) {
        length++;
    }
    return length;
}

// the index of the character, or of the terminator
// 0x00176344 | nintendogs:bytes-fuzzy [tier B]
DECOMP_NOINLINE int CFLi_SearchCharacter8(const char* str, char c)
{
    int i = 0;
    while (str[i] != 0 && str[i] != c) {
        i++;
    }
    return i;
}

// digits, also full width, Roman numerals and circled numbers
// 0x001762E8 | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsNumberCharCode(unsigned short c)
{
    if (static_cast<u32>(c - '0') <= 9 || static_cast<u32>(c - 0xFF10) <= 9) {
        return true;
    }
    if (static_cast<u32>(c - 0x2160) <= 9 || static_cast<u32>(c - 0x2170) <= 9) {
        return true;
    }
    if (static_cast<u32>(c - 0x2460) <= 19 || static_cast<u32>(c - 0x2776) <= 8) {
        return true;
    }
    return false;
}

// the number of days of a month (1-12), 0 for others
// 0x001708D8 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE int CFLi_GetMonthOfDay(int month)
{
    if (static_cast<u32>(month - 1) <= 11) {
        return s_DaysOfMonth[month];
    }
    return 0;
}

// 0x00172664 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_GetNowDateTime(int* year, int* month, int* day, int* hour, int* minute, int* second)
{
    nn::fnd::DateTimeParameters now = nn::fnd::DateTime::GetNow().GetParameters();
    *year = now.year;
    *month = now.month;
    *day = now.day;
    *hour = now.hour;
    *minute = now.minute;
    *second = now.second;
}

// 0x00168F04 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE const CFLiAuthorID* CFLi_GetUserID()
{
    return reinterpret_cast<const CFLiAuthorID*>(&s_UserID);
}

// takes a block of size bytes (aligned to 32) from the front of a buffer and clears it
// 0x0017EDB4 | nintendogs:callseq-callee [tier C]
DECOMP_NOINLINE void* CFLi_AllocateAlignedBuffer(void** buffer, unsigned* bufferSize, unsigned size)
{
    uptr start = reinterpret_cast<uptr>(*buffer);
    uptr padding = -start & 31;
    *buffer = reinterpret_cast<void*>(start + padding + size);
    *bufferSize -= padding + size;
    void* block = reinterpret_cast<void*>(start + padding);
    std::memset(block, 0, size);
    return block;
}
