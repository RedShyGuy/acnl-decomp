#include "sead/seadStringUtil.h"

#include <climits>

namespace {
// a pair of the case tables (names are ours)
struct CaseEntry
{
    wchar_t from;
    wchar_t to;
};

// the lower case letters with their upper case (sorted), then the upper case letters with their
// lower case
// 0x008AEE70
const CaseEntry s_CaseEntries[172] = {
    {0x0061, 0x0041}, {0x0062, 0x0042}, {0x0063, 0x0043}, {0x0064, 0x0044}, {0x0065, 0x0045}, {0x0066, 0x0046},
    {0x0067, 0x0047}, {0x0068, 0x0048}, {0x0069, 0x0049}, {0x006A, 0x004A}, {0x006B, 0x004B}, {0x006C, 0x004C},
    {0x006D, 0x004D}, {0x006E, 0x004E}, {0x006F, 0x004F}, {0x0070, 0x0050}, {0x0071, 0x0051}, {0x0072, 0x0052},
    {0x0073, 0x0053}, {0x0074, 0x0054}, {0x0075, 0x0055}, {0x0076, 0x0056}, {0x0077, 0x0057}, {0x0078, 0x0058},
    {0x0079, 0x0059}, {0x007A, 0x005A}, {0x00E0, 0x00C0}, {0x00E1, 0x00C1}, {0x00E2, 0x00C2}, {0x00E3, 0x00C3},
    {0x00E4, 0x00C4}, {0x00E6, 0x00C6}, {0x00E7, 0x00C7}, {0x00E8, 0x00C8}, {0x00E9, 0x00C9}, {0x00EA, 0x00CA},
    {0x00EB, 0x00CB}, {0x00EC, 0x00CC}, {0x00ED, 0x00CD}, {0x00EE, 0x00CE}, {0x00EF, 0x00CF}, {0x00F1, 0x00D1},
    {0x00F2, 0x00D2}, {0x00F3, 0x00D3}, {0x00F4, 0x00D4}, {0x00F5, 0x00D5}, {0x00F6, 0x00D6}, {0x00F9, 0x00D9},
    {0x00FA, 0x00DA}, {0x00FB, 0x00DB}, {0x00FC, 0x00DC}, {0x00FF, 0x0178}, {0x0153, 0x0152}, {0x0430, 0x0410},
    {0x0431, 0x0411}, {0x0432, 0x0412}, {0x0433, 0x0413}, {0x0434, 0x0414}, {0x0435, 0x0415}, {0x0436, 0x0416},
    {0x0437, 0x0417}, {0x0438, 0x0418}, {0x0439, 0x0419}, {0x043A, 0x041A}, {0x043B, 0x041B}, {0x043C, 0x041C},
    {0x043D, 0x041D}, {0x043E, 0x041E}, {0x043F, 0x041F}, {0x0440, 0x0420}, {0x0441, 0x0421}, {0x0442, 0x0422},
    {0x0443, 0x0423}, {0x0444, 0x0424}, {0x0445, 0x0425}, {0x0446, 0x0426}, {0x0447, 0x0427}, {0x0448, 0x0428},
    {0x0449, 0x0429}, {0x044A, 0x042A}, {0x044B, 0x042B}, {0x044C, 0x042C}, {0x044D, 0x042D}, {0x044E, 0x042E},
    {0x044F, 0x042F}, {0x0451, 0x0401}, {0x0041, 0x0061}, {0x0042, 0x0062}, {0x0043, 0x0063}, {0x0044, 0x0064},
    {0x0045, 0x0065}, {0x0046, 0x0066}, {0x0047, 0x0067}, {0x0048, 0x0068}, {0x0049, 0x0069}, {0x004A, 0x006A},
    {0x004B, 0x006B}, {0x004C, 0x006C}, {0x004D, 0x006D}, {0x004E, 0x006E}, {0x004F, 0x006F}, {0x0050, 0x0070},
    {0x0051, 0x0071}, {0x0052, 0x0072}, {0x0053, 0x0073}, {0x0054, 0x0074}, {0x0055, 0x0075}, {0x0056, 0x0076},
    {0x0057, 0x0077}, {0x0058, 0x0078}, {0x0059, 0x0079}, {0x005A, 0x007A}, {0x00C0, 0x00E0}, {0x00C1, 0x00E1},
    {0x00C2, 0x00E2}, {0x00C3, 0x00E3}, {0x00C4, 0x00E4}, {0x00C6, 0x00E6}, {0x00C7, 0x00E7}, {0x00C8, 0x00E8},
    {0x00C9, 0x00E9}, {0x00CA, 0x00EA}, {0x00CB, 0x00EB}, {0x00CC, 0x00EC}, {0x00CD, 0x00ED}, {0x00CE, 0x00EE},
    {0x00CF, 0x00EF}, {0x00D1, 0x00F1}, {0x00D2, 0x00F2}, {0x00D3, 0x00F3}, {0x00D4, 0x00F4}, {0x00D5, 0x00F5},
    {0x00D6, 0x00F6}, {0x00D9, 0x00F9}, {0x00DA, 0x00FA}, {0x00DB, 0x00FB}, {0x00DC, 0x00FC}, {0x0152, 0x0153},
    {0x0178, 0x00FF}, {0x0401, 0x0451}, {0x0410, 0x0430}, {0x0411, 0x0431}, {0x0412, 0x0432}, {0x0413, 0x0433},
    {0x0414, 0x0434}, {0x0415, 0x0435}, {0x0416, 0x0436}, {0x0417, 0x0437}, {0x0418, 0x0438}, {0x0419, 0x0439},
    {0x041A, 0x043A}, {0x041B, 0x043B}, {0x041C, 0x043C}, {0x041D, 0x043D}, {0x041E, 0x043E}, {0x041F, 0x043F},
    {0x0420, 0x0440}, {0x0421, 0x0441}, {0x0422, 0x0442}, {0x0423, 0x0443}, {0x0424, 0x0444}, {0x0425, 0x0445},
    {0x0426, 0x0446}, {0x0427, 0x0447}, {0x0428, 0x0448}, {0x0429, 0x0449}, {0x042A, 0x044A}, {0x042B, 0x044B},
    {0x042C, 0x044C}, {0x042D, 0x044D}, {0x042E, 0x044E}, {0x042F, 0x044F},
};

// (called through a pointer by the binary search)
// 0x0053F3AC (name is ours)
DECOMP_NOINLINE s32 compareCaseEntry(const CaseEntry* a, const CaseEntry* b)
{
    return a->from - b->from;
}

// a sorted case table (name is ours)
class CaseTable
{
public:
    CaseTable(s32 size, const CaseEntry* entries) : mSize(size), mEntries(entries) {}

    // the index of the entry of c, -1 if there is none
    s32 binarySearch(const CaseEntry* key, s32 (*compare)(const CaseEntry*, const CaseEntry*)) const
    {
        if (mSize == 0) {
            return -1;
        }
        s32 lo = 0;
        s32 hi = mSize - 1;
        while (lo < hi) {
            s32 mid = (lo + hi) / 2;
            s32 result = compare(&mEntries[mid], key);
            if (result == 0) {
                return mid;
            }
            if (result > 0) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        if (compare(&mEntries[lo], key) == 0) {
            return lo;
        }
        return -1;
    }

    const CaseEntry& operator[](s32 index) const
    {
        if (static_cast<u32>(index) < static_cast<u32>(mSize)) {
            return mEntries[index];
        }
        return mEntries[0];
    }

    wchar_t convert(wchar_t c) const
    {
        CaseEntry key;
        key.from = c;
        s32 index = binarySearch(&key, compareCaseEntry);
        if (index < 0) {
            return c;
        }
        return (*this)[index].to;
    }

    s32 mSize;                 // 0x0
    const CaseEntry* mEntries; // 0x4
};

// 0x009763D0
CaseTable s_ToUpperTable(86, s_CaseEntries);
// 0x009763D8
CaseTable s_ToLowerTable(86, s_CaseEntries + 86);
} // namespace

// the static initializer of the case tables
// 0x00798EA0 (static initializer)

namespace sead {
// 0x0053F24C (name is ours)
wchar_t sead::StringUtil::toLower(wchar_t c)
{
    return s_ToLowerTable.convert(c);
}

// 0x0053F2FC (name is ours)
wchar_t sead::StringUtil::toUpper(wchar_t c)
{
    return s_ToUpperTable.convert(c);
}

// 0x0053F3BC (name is ours)
s32 sead::StringUtil::convertUtf16ToUtf8(char* dst, u32 dstSize, const wchar_t* src, s32 srcLength)
{
    if (dstSize == 0 || srcLength < -1) {
        return 0;
    }
    if (srcLength == -1) {
        srcLength = INT_MAX;
    }
    u32 n = 0;
    for (s32 i = 0; n < dstSize; i++) {
        u32 c = static_cast<u16>(src[i]);
        if (i >= srcLength || c == 0) {
            dst[n] = '\0';
            return n;
        }
        if (c < 0x80) {
            dst[n++] = static_cast<char>(c);
        } else if (c < 0x800) {
            if (n + 1 >= dstSize) {
                break;
            }
            dst[n] = static_cast<char>(0xC0 | (c >> 6));
            dst[n + 1] = static_cast<char>(0x80 | (c & 0x3F));
            n += 2;
        } else {
            if (n + 2 >= dstSize) {
                break;
            }
            dst[n] = static_cast<char>(0xE0 | (c >> 12));
            dst[n + 1] = static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            dst[n + 2] = static_cast<char>(0x80 | (c & 0x3F));
            n += 3;
        }
    }
    dst[n - 1] = '\0';
    return n - 1;
}

// 0x0053F4B8 (name is ours)
s32 sead::StringUtil::convertUtf8ToUtf16(wchar_t* dst, u32 dstSize, const char* src, s32 srcLength)
{
    if (dstSize == 0 || srcLength < -1) {
        return 0;
    }
    if (srcLength == -1) {
        srcLength = INT_MAX;
    }
    u32 n = 0;
    s32 i = 0;
    for (; n < dstSize; n++) {
        if (i >= srcLength) {
            dst[n] = L'\0';
            return n;
        }
        u8 c = static_cast<u8>(src[i]);
        if (c == 0) {
            dst[n] = L'\0';
            return n;
        }
        if ((c & 0x80) == 0) {
            dst[n] = c;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            u8 c1 = static_cast<u8>(src[i + 1]);
            if (c1 == 0) {
                dst[n] = L'\0';
                return n;
            }
            dst[n] = static_cast<wchar_t>(((c & 0x1F) << 6) | (c1 & 0x3F));
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            u8 c1 = static_cast<u8>(src[i + 1]);
            u8 c2;
            if (c1 == 0 || (c2 = static_cast<u8>(src[i + 2])) == 0) {
                dst[n] = L'\0';
                return n;
            }
            dst[n] = static_cast<wchar_t>((c << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F));
            i += 3;
        } else {
            dst[n] = L'\0';
            return n;
        }
    }
    dst[n - 1] = L'\0';
    return n - 1;
}

// 0x0053F5C8 (name is ours)
void sead::StringUtil::parseSign_(s32* sign, Iterator* it)
{
    *sign = 1;
    char c = **it;
    if (c == '+') {
        ++*it;
    } else if (c == '-') {
        *sign = -1;
        ++*it;
    }
}

// 0x0053F66C (name is ours)
bool sead::StringUtil::detectBase_(CardinalNumber* base, Iterator* it, const SafeStringBase<char>& str)
{
    char c = **it;
    if (static_cast<u32>(c - '0') >= 10) {
        return false;
    }
    if (c != '0') {
        *base = BASE_10;
        return true;
    }
    char next = str.at(it->mIndex + 1);
    if (next == 'b') {
        *base = BASE_2;
        it->mIndex += 2;
    } else if (next == 'x') {
        *base = BASE_16;
        it->mIndex += 2;
    } else {
        *base = BASE_8;
    }
    return true;
}

// the instances that are out of line in the original (u32 is inlined into tryParseS32/U32)
// 0x008112C4 (name is ours)
template bool sead::StringUtil::parseNonDecimal_<u8>(u8* out, Iterator& it, s32 base);
// 0x008113FC (name is ours)
template bool sead::StringUtil::parseNonDecimal_<u16>(u16* out, Iterator& it, s32 base);
// 0x00811538 (name is ours)
template bool sead::StringUtil::parseNonDecimal_<u64>(u64* out, Iterator& it, s32 base);

} // namespace sead
