#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"

#include <cctype>
#include <climits>

namespace sead {
// The number parsers of strings. tryParseS32/U32 are named after pead::StringUtil (reference symbols
// of nintendogs), the other parsers after them; the helpers, CardinalNumber's values and the
// templates are ours. The parsers are inline functions of the header (ARMCC put them out of line
// into the collection area), the helpers with a fixed type are in seadStringUtil.cpp.
class StringUtil
{
public:
    // the base of a number; BASE_AUTO: from the prefix (0b binary, 0x hexadecimal, 0 octal)
    enum CardinalNumber : s32
    {
        BASE_AUTO = -1,
        BASE_2 = 2,
        BASE_8 = 8,
        BASE_10 = 10,
        BASE_16 = 16,
    };

    typedef SafeStringBase<char>::iterator Iterator;

    // the upper / lower case of c (Latin, Latin-1 and Cyrillic letters; other characters stay)
    static wchar_t toUpper(wchar_t c); // 0x0053F2FC (name is ours)
    static wchar_t toLower(wchar_t c); // 0x0053F24C (name is ours)
    // converts src (srcLength characters or up to the null character; -1: no limit) and writes at
    // most dstSize units including the null character; the length written
    static s32 convertUtf16ToUtf8(char* dst, u32 dstSize, const wchar_t* src, s32 srcLength); // 0x0053F3BC (name is ours)
    static s32 convertUtf8ToUtf16(wchar_t* dst, u32 dstSize, const char* src, s32 srcLength); // 0x0053F4B8 (name is ours)

    // the sign at the iterator (+1 or -1; a sign character is skipped)
    static void parseSign_(s32* sign, Iterator* it); // 0x0053F5C8
    // the base of the prefix at the iterator (the prefix 0b / 0x is skipped); false if no digit
    // follows
    static bool detectBase_(CardinalNumber* base, Iterator* it, const SafeStringBase<char>& str); // 0x0053F66C

    // the value of the digit c of a base up to 16, -1 if it is none
    static s32 digitValue_(s32 c)
    {
        c = static_cast<char>(tolower(c));
        if (c <= '9') {
            return c - '0';
        }
        if (c - 'a' <= 5u) {
            return c - 'a' + 10;
        }
        return -1;
    }

    // the digits of a base other than 10 at the iterator as an unsigned value (it stops at the
    // first character that is no digit); false on an overflow or without a digit
    template <typename T>
    static DECOMP_NOINLINE bool parseNonDecimal_(T* out, Iterator& it, s32 base)
    {
        const T max = static_cast<T>(-1);
        bool isParsed = false;
        T value = 0;
        const T limit = max / base;
        while (true) {
            s32 digit = digitValue_(*it);
            if (digit < 0 || digit >= base) {
                break;
            }
            if (value > limit || max - static_cast<T>(digit) < static_cast<T>(value * base)) {
                return false;
            }
            value = static_cast<T>(value * base) + digit;
            isParsed = true;
            ++it;
        }
        if (!isParsed) {
            return false;
        }
        if (out != nullptr) {
            *out = value;
        }
        return true;
    }

    // the decimal digits at the iterator, with the sign; false on an overflow or without a digit
    template <typename T>
    static bool parseDecimal_(T* out, Iterator& it, s32 sign, T min, T max)
    {
        const T minDiv10 = min / 10;
        const T maxDiv10 = max / 10;
        bool isParsed = false;
        T value = 0;
        while (true) {
            s32 c = *it;
            if (c > '9') {
                break;
            }
            s32 digit = c - '0';
            if (digit < 0) {
                break;
            }
            if (value < minDiv10 || maxDiv10 < value) {
                return false;
            }
            T value10 = static_cast<T>(value * 10);
            if (sign < 0) {
                if (min + digit > value10) {
                    return false;
                }
            } else if (max - digit < value10) {
                return false;
            }
            value = static_cast<T>(value10 + sign * digit);
            isParsed = true;
            ++it;
        }
        if (!isParsed) {
            return false;
        }
        if (out != nullptr) {
            *out = value;
        }
        return true;
    }

    // T: the type of the value, U: its unsigned type
    template <typename T, typename U>
    static bool tryParse_(T* out, const SafeStringBase<char>& str, CardinalNumber base, T min, T max)
    {
        Iterator it(&str);
        s32 sign;
        parseSign_(&sign, &it);
        if (base <= 0 && !detectBase_(&base, &it, str)) {
            return false;
        }
        if (base != BASE_10) {
            U value;
            if (!parseNonDecimal_<U>(&value, it, base)) {
                return false;
            }
            *out = static_cast<T>(value * sign);
            return true;
        }
        return parseDecimal_<T>(out, it, sign, min, max);
    }

    static DECOMP_NOINLINE bool tryParseS8(s8* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00810178 (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseU8(u8* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00810344 (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseS16(s16* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00810AA4 (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseU16(u16* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00810C74 (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseS32(s32* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x0081050C (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseU32(u32* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x008107DC (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseS64(s64* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00810E40 (name after pead::StringUtil)
    static DECOMP_NOINLINE bool tryParseU64(u64* out, const SafeStringBase<char>& str, CardinalNumber base); // 0x00811084 (name after pead::StringUtil)
};

// ARMCC put the u32 version into tryParseS32/U32
template <>
DECOMP_ALWAYS_INLINE bool StringUtil::parseNonDecimal_<u32>(u32* out, Iterator& it, s32 base)
{
    const u32 max = 0xFFFFFFFF;
    bool isParsed = false;
    u32 value = 0;
    const u32 limit = max / base;
    while (true) {
        s32 digit = digitValue_(*it);
        if (digit < 0 || digit >= base) {
            break;
        }
        if (value > limit || max - digit < value * base) {
            return false;
        }
        value = value * base + digit;
        isParsed = true;
        ++it;
    }
    if (!isParsed) {
        return false;
    }
    if (out != nullptr) {
        *out = value;
    }
    return true;
}

inline bool StringUtil::tryParseS8(s8* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<s8, u8>(out, str, base, SCHAR_MIN, SCHAR_MAX);
}

inline bool StringUtil::tryParseU8(u8* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<u8, u8>(out, str, base, 0, UCHAR_MAX);
}

inline bool StringUtil::tryParseS16(s16* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<s16, u16>(out, str, base, SHRT_MIN, SHRT_MAX);
}

inline bool StringUtil::tryParseU16(u16* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<u16, u16>(out, str, base, 0, USHRT_MAX);
}

inline bool StringUtil::tryParseS32(s32* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<s32, u32>(out, str, base, INT_MIN, INT_MAX);
}

inline bool StringUtil::tryParseU32(u32* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<u32, u32>(out, str, base, 0, UINT_MAX);
}

inline bool StringUtil::tryParseS64(s64* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<s64, u64>(out, str, base, LLONG_MIN, LLONG_MAX);
}

inline bool StringUtil::tryParseU64(u64* out, const SafeStringBase<char>& str, CardinalNumber base)
{
    return tryParse_<u64, u64>(out, str, base, 0, ULLONG_MAX);
}
} // namespace sead
