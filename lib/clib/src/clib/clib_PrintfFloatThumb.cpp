// The floating point conversions of printf (%e %f %g; Thumb code in the original): the value is scaled by a power
// of ten in extended precision (btod: sign/exponent word with bias 0x3FFF, 64 bit mantissa with its leading one)
// until its integer part holds the wanted digits. The btod routines are hand-written assembly that return the
// extended number in r0-r2 (ARMCC's __value_in_regs), so their calls are inline assembly with fixed registers.
// All names are ours.

#include "decomp.h"
#include "clib/clib_Btod.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Printf.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
// a power of ten with the correction of its last bit for the rounding direction that it was rounded against
struct ExtendedPower
{
    Extended m_Value;
    s32 m_Rounding;
};

// the digits of a value: the decimal exponent of the first digit, their count, and the mode that produced them
struct FloatDigits
{
    int m_Exponent;
    int m_Count;
    int m_Mode;     // 1: a number of digits after the point (%f), 0: significant digits
};

// the exponent bias between a double converted by __btod_d2e and the extended numbers
const s32 DOUBLE_TO_EXTENDED_BIAS = -0x201F;

const int NO_EXPONENT = static_cast<int>(0x80000000);

inline void PutChar(PrintfState* pState, int c)
{
    pState->m_PutChar(c, pState->m_pPutArgument);
    pState->m_Count++;
}

inline Extended Multiply(const Extended* pA, const Extended* pB, int rounding)
{
    register u32 r0 asm("r0") = reinterpret_cast<u32>(pA);
    register u32 r1 asm("r1") = reinterpret_cast<u32>(pB);
    register u32 r2 asm("r2") = rounding;
    asm volatile("bl __btod_emul" : "+r"(r0), "+r"(r1), "+r"(r2) : : "r3", "ip", "lr", "cc", "memory");
    Extended result = {r0, r1, r2};
    return result;
}

inline Extended Divide(const Extended* pA, const Extended* pB, int rounding)
{
    register u32 r0 asm("r0") = reinterpret_cast<u32>(pA);
    register u32 r1 asm("r1") = reinterpret_cast<u32>(pB);
    register u32 r2 asm("r2") = rounding;
    asm volatile("bl __btod_ediv" : "+r"(r0), "+r"(r1), "+r"(r2) : : "r3", "ip", "lr", "cc", "memory");
    Extended result = {r0, r1, r2};
    return result;
}

inline Extended ToExtended(u32 high, u32 low)
{
    register u32 r0 asm("r0") = high;
    register u32 r1 asm("r1") = low;
    register u32 r2 asm("r2");
    asm("bl __btod_d2e" : "+r"(r0), "+r"(r1), "=r"(r2) : : "r3", "ip", "lr", "cc");
    Extended result = {r0, r1, r2};
    return result;
}

// _ll_udiv10 returns the quotient in r0/r1 and the remainder in r2: value /= 10, the remainder
inline u32 DivideBy10(u64* pValue)
{
    register u32 low asm("r0") = static_cast<u32>(*pValue);
    register u32 high asm("r1") = static_cast<u32>(*pValue >> 32);
    register u32 remainder asm("r2");
    asm("bl _ll_udiv10" : "+r"(low), "+r"(high), "=r"(remainder) : : "r3", "ip", "lr", "cc");
    *pValue = static_cast<u64>(high) << 32 | low;
    return remainder;
}
} // namespace

// 10^(2^i) for i = 0..4
// 0x008C98B8
extern const Extended s_PowersOfTenSmall[5] = {
    {0x4002, 0xA0000000, 0x00000000}, {0x4005, 0xC8000000, 0x00000000}, {0x400C, 0x9C400000, 0x00000000},
    {0x4019, 0xBEBC2000, 0x00000000}, {0x4034, 0x8E1BC9BF, 0x04000000},
};
// 10^(55 * 2^i) for i = 0..3
// 0x008C98F4
extern const ExtendedPower s_PowersOfTenLarge[4] = {
    {{0x40B5, 0xD0CF4B50, 0xCFE20766}, 1},
    {{0x416C, 0xAA51823E, 0x34A7EEDF}, 1},
    {{0x42D9, 0xE2A0B5DC, 0x971F303A}, -1},
    {{0x45B4, 0xC8A025FD, 0x4FC1A3E9}, -1},
};

// the digits of %a, then the letters of the prefix and of the exponent
// 0x008C9890
extern const char s_HexFloatDigitsLower[19] = "0123456789abcdefxp";
// 0x008C98A3
extern const char s_HexFloatDigitsUpper[19] = "0123456789ABCDEFXP";

extern "C" {

__attribute__((pcs("aapcs"))) int __ARM_fpclassify(f64 value);
u32 __fp_status(u32 mask, u32 flags);
__attribute__((pcs("aapcs"))) u64 _udiv10(u32 value);
void __printf_float_special(PrintfState* pState, int c, int category, int sign);

// result = 10^exponent (rounding: 0 to nearest, 1 up, -1 down): 10^(55 * large) * 10^small or / 10^-small
// 0x002F8A40 (name is ours)
void _btod_etento(Extended* pResult, int exponent, int rounding)
{
    // (local constants: ARMCC copies them from 0x8C9934 and 0x8C9940)
    Extended small = {0x3FFF, 0x80000000, 0};
    Extended large = {0x3FFF, 0x80000000, 0};
    int largeBits = (exponent + 7067) / 55 - 128;
    int smallBits = (exponent + 7067) % 55 - 27;
    bool isNegative = smallBits < 0;
    if (isNegative) {
        smallBits = -smallBits;
    }
    for (int i = 0; smallBits != 0; i++, smallBits >>= 1) {
        if ((smallBits & 1) != 0) {
            small = Multiply(&small, &s_PowersOfTenSmall[i], rounding);
        }
    }
    for (int i = 0; largeBits != 0; i++, largeBits >>= 1) {
        if ((largeBits & 1) != 0) {
            Extended power = s_PowersOfTenLarge[i].m_Value;
            if (s_PowersOfTenLarge[i].m_Rounding == -rounding) {
                power.m_MantissaLow += rounding;
            }
            large = Multiply(&large, &power, rounding);
        }
    }
    *pResult = isNegative ? Divide(&large, &small, rounding) : Multiply(&large, &small, rounding);
}

// the decimal digits of *pValue (a double as two words): count significant digits (mode 0) or the digits up to count
// places after the point (mode 1; with more than 17 digits it falls back to 17 significant digits)
// 0x002F8DAA (name is ours)
void __printf_float_digits(FloatDigits* pResult, char* pDigits, const u32* pValue, int count, int mode)
{
    u32 low = pValue[0];
    u32 high = pValue[1];
    u32 magnitude = high << 1;
    int binaryExponent = magnitude >> 21;
    if (binaryExponent == 0) {
        binaryExponent--;
    }
    // the rounding direction (towards +inf and -inf swap for a negative value)
    u32 status = __fp_status(0, 0) & 0xC00000;
    if (static_cast<s32>(high) < 0) {
        status = ((status + (status << 2)) >> 1) & 0xC00000;
    }
    int rounding = status >> 22;
    if (rounding > 1) {
        rounding = -1;
    }
    int digits;
    int exponent;
    if ((magnitude | low) == 0) {
        // zero
        if (mode == 1) {
            digits = 0;
            exponent = ~count;
        } else {
            for (digits = 0; digits < count; digits++) {
                pDigits[digits] = '0';
            }
            digits = count;
            exponent = 0;
        }
        pDigits[digits] = 0;
        pResult->m_Exponent = exponent;
        pResult->m_Count = digits;
        pResult->m_Mode = mode;
        return;
    }
    exponent = ((binaryExponent - 1023) * 0x4D10) >> 16;   // an estimate of the decimal exponent
    for (;;) {
        // value * 10^-scale has the wanted digits in its integer part
        int scale = mode != 0 ? -count : exponent - count + 1;
        Extended power;
        _btod_etento(&power, scale >= 0 ? scale : -scale, scale > 0 ? -rounding : rounding);
        Extended value = ToExtended(high, low);
        value.m_SignExponent += DOUBLE_TO_EXTENDED_BIAS;
        if (scale > 0) {
            power.m_SignExponent -= DOUBLE_TO_EXTENDED_BIAS;
            value = Divide(&value, &power, rounding);
        } else {
            power.m_SignExponent += DOUBLE_TO_EXTENDED_BIAS;
            value = Multiply(&value, &power, rounding);
        }
        u64 integer;
        if ((value.m_SignExponent << 16) != 0) {
            integer = 0x7FFFFFFFFFFFFFFF;            // too large
        } else {
            integer = static_cast<u64>(value.m_MantissaHigh) << 32 | value.m_MantissaLow;
        }
        if (mode != 0) {
            digits = 0;
            while (integer != 0 && digits < 17) {
                pDigits[digits++] = DivideBy10(&integer) + '0';
            }
            if (integer == 0) {
                // the digits came from the last one on
                for (int i = 0, j = digits - 1; i < j; i++, j--) {
                    char c = pDigits[j];
                    pDigits[j] = pDigits[i];
                    pDigits[i] = c;
                }
                exponent = digits - count - 1;
                break;
            }
            // more than 17 digits: 17 significant digits instead
            count = 17;
            mode = 0;
            continue;
        }
        for (int i = count - 1; i >= 0; i--) {
            pDigits[i] = DivideBy10(&integer) + '0';
        }
        digits = count;
        if (integer != 0) {
            exponent++;                                 // one digit too many: the estimate was low
        } else if (pDigits[0] == '0') {
            exponent--;                                 // one digit too few
        } else {
            break;
        }
    }
    pDigits[digits] = 0;
    pResult->m_Count = digits;
    pResult->m_Exponent = exponent;
    pResult->m_Mode = mode;
}

// infinity and NaN
// 0x002F8D20 (name is ours)
void __printf_float_special(PrintfState* pState, int c, int category, int sign)
{
    u32 flags = pState->m_Flags;
    const char* pText;
    if (category >= 7) {
        pText = (flags & PRINTF_UPPER) != 0 ? "NAN" : "nan";
    } else {
        pText = (flags & PRINTF_UPPER) != 0 ? "INF" : "inf";
    }
    pState->m_Flags = flags & ~PRINTF_ZERO;
    pState->m_Fields[PRINTF_WIDTH] -= 3;
    if (sign != 0) {
        pState->m_Fields[PRINTF_WIDTH]--;
    }
    __printf_pre_padding(pState);
    if (sign != 0) {
        PutChar(pState, sign);
    }
    pState->m_Count += 3;
    for (int n = 3; n-- != 0;) {
        pState->m_PutChar(*pText++, pState->m_pPutArgument);
    }
    __printf_post_padding(pState);
}

// %e %f %g (%E %G with PRINTF_UPPER)
// 0x002F8F44 (name is ours)
int __printf_float(PrintfState* pState, int c, const u8* pArguments)
{
    const u32* pAligned = reinterpret_cast<const u32*>((reinterpret_cast<uptr>(pArguments) + 7) >> 3 << 3);
    u32 value[2] = {pAligned[0], pAligned[1]};
    int category = __ARM_fpclassify(*reinterpret_cast<const f64*>(value));
    int sign;
    if ((value[1] >> 31) != 0) {
        sign = '-';
    } else if ((pState->m_Flags & PRINTF_PLUS) != 0) {
        sign = '+';
    } else {
        sign = ((pState->m_Flags << 29) >> 31) << 5;      // ' ' or nothing
    }
    if (category == 3 || category >= 7) {
        __printf_float_special(pState, c, category, sign);
        return 2;
    }
    int precision = (pState->m_Flags & PRINTF_PRECISION) != 0 ? pState->m_Fields[PRINTF_PRECISION_FIELD] : 6;
    char digits[36];
    FloatDigits result;
    int printed;            // the digits to print
    int first;              // the index of the first digit printed (negative: leading zeros)
    int available;          // the digits in the buffer
    int pointAfter;         // the digits before the decimal point (or less than 0: no point)
    int exponent;           // the exponent printed (NO_EXPONENT for %f)
    if (c == 'e') {
        __printf_float_digits(&result, digits, value, precision < 17 ? precision + 1 : 17, 0);
        available = result.m_Count;
        first = 0;
        printed = precision + 1;
        pointAfter = 1;
        exponent = result.m_Exponent;
    } else if (c == 'f') {
        exponent = NO_EXPONENT;
        __printf_float_digits(&result, digits, value, precision, 1);
        printed = result.m_Count;
        first = 0;
        available = printed;
        if (result.m_Mode == 0) {
            printed = result.m_Exponent + precision + 1;
        }
        if (precision - printed >= 0) {
            // below 1: leading zeros
            first = -(precision - printed) - 1;
            printed = precision + 1;
        }
        pointAfter = printed - precision;
    } else {
        if (precision < 1) {
            precision = 1;
        }
        __printf_float_digits(&result, digits, value, precision > 17 ? 17 : precision, 0);
        available = result.m_Count;
        first = 0;
        printed = precision;
        int decimalExponent = result.m_Exponent;
        if ((pState->m_Flags & PRINTF_ALTERNATE) == 0) {
            // without '#' no zeros at the end
            if (available < printed) {
                printed = available;
            }
            while (printed > 1 && digits[printed - 1] == '0') {
                printed--;
            }
        }
        if (decimalExponent >= precision || decimalExponent + 4 < 0) {
            pointAfter = 1;
            exponent = decimalExponent;
        } else {
            if (decimalExponent > 0) {
                if (decimalExponent + 1 > printed) {
                    printed = decimalExponent + 1;
                }
            } else {
                first += decimalExponent;
                printed -= decimalExponent;
            }
            pointAfter = decimalExponent - first + 1;
            exponent = NO_EXPONENT;
        }
    }
    if ((pState->m_Flags & PRINTF_ALTERNATE) == 0 && pointAfter >= printed) {
        pointAfter = -1;                                // nothing after the point: no point
    }
    // the exponent, written backwards ("e+05")
    char exponentText[8];
    char* pExponent = &exponentText[7];
    *pExponent = 0;
    if (exponent != NO_EXPONENT) {
        int minimum = 2;
        int exponentSign = '+';
        if (exponent < 0) {
            exponent = -exponent;
            exponentSign = '-';
        }
        while (minimum-- > 0 || exponent != 0) {
            u64 quotient = _udiv10(exponent);
            *--pExponent = static_cast<u32>(quotient >> 32) + '0';
            exponent = static_cast<u32>(quotient);
        }
        *--pExponent = exponentSign;
        *--pExponent = (pState->m_Flags & PRINTF_UPPER) != 0 ? 'E' : 'e';
    }
    int exponentLength = &exponentText[7] - pExponent;
    pState->m_Fields[PRINTF_WIDTH] -= (sign != 0 ? 1 : 0) + printed + (pointAfter >> 31) + exponentLength + 1;
    if ((pState->m_Flags & PRINTF_ZERO) == 0) {
        __printf_pre_padding(pState);
    }
    if (sign != 0) {
        PutChar(pState, sign);
    }
    if ((pState->m_Flags & PRINTF_ZERO) != 0) {
        __printf_pre_padding(pState);
    }
    while (printed-- > 0) {
        PutChar(pState, first >= 0 && first < available ? digits[first] : '0');
        first++;
        if (--pointAfter == 0) {
            const NumericData* pNumeric = GetCLibraryLocale()->m_pNumeric;
            PutChar(pState, reinterpret_cast<const char*>(pNumeric)[pNumeric->m_DecimalPointOffset]);
        }
    }
    while (exponentLength-- > 0) {
        PutChar(pState, *pExponent++);
    }
    __printf_post_padding(pState);
    return 2;
}

// %a: the mantissa in hexadecimal ("0x1.8p+3"); the zeros asked for beyond the 13 digits of a double are only
// counted ('>' in the buffer)
// 0x00102574 (name is ours)
int __printf_hexfloat(PrintfState* pState, int c, const u8* pArguments)
{
    const u32* pAligned = reinterpret_cast<const u32*>((reinterpret_cast<uptr>(pArguments) + 7) >> 3 << 3);
    u32 high = pAligned[1];
    u32 low = pAligned[0];
    u32 value[2] = {low, high};
    int category = __ARM_fpclassify(*reinterpret_cast<const f64*>(value));
    int sign;
    if ((high >> 31) != 0) {
        sign = '-';
    } else if ((pState->m_Flags & PRINTF_PLUS) != 0) {
        sign = '+';
    } else {
        sign = ((pState->m_Flags << 29) >> 31) << 5;
    }
    char prefix[8];
    prefix[0] = sign;
    int prefixLength = sign != 0 ? 1 : 0;
    if (category == 3 || category >= 7) {
        __printf_float_special(pState, c, category, sign);
        return 2;
    }
    int precision = (pState->m_Flags & PRINTF_PRECISION) == 0 ? 13 : pState->m_Fields[PRINTF_PRECISION_FIELD];
    int exponent = (high << 1) >> 21;
    if (precision < 13) {
        // rounded at the last digit kept
        u32 signExponent = high >> 20;
        s32 status = __fp_status(0, 0) & 0xC00000;
        if (static_cast<s32>(high) < 0) {
            status = ((status * 5) / 2) & 0xC00000;
        }
        int rounding = status >> 22;
        if (rounding > 1) {
            rounding = -1;
        }
        u32 shift = precision * 4;
        u32 unitHigh;
        u32 unitLow;
        if (shift >= 24) {
            unitHigh = 0;
            unitLow = 0x10000000 >> (shift - 24);
        } else {
            unitHigh = 0x100000 >> shift;
            unitLow = 0;
        }
        u32 fractionShift = shift + 12;
        u32 rest = fractionShift >= 32 ? low << (fractionShift - 32) : high << fractionShift;
        bool up;
        if (rounding > 0) {
            up = rest != 0;
        } else {
            up = rounding == 0 && rest >= 0x80000000;
        }
        if (up) {
            u64 sum = (static_cast<u64>(high) << 32 | low) + (static_cast<u64>(unitHigh) << 32 | unitLow);
            low = static_cast<u32>(sum);
            high = static_cast<u32>(sum >> 32);
            if (rounding == 0 && rest == 0x80000000) {
                low &= ~unitLow;                    // a tie: to even
                high &= ~unitHigh;
            }
            if (high >> 20 != signExponent) {
                exponent++;
            }
        }
    }
    const char* pDigits = (pState->m_Flags & PRINTF_UPPER) != 0 ? s_HexFloatDigitsUpper : s_HexFloatDigitsLower;
    prefix[prefixLength] = '0';
    prefix[prefixLength + 1] = pDigits[16];
    prefixLength += 2;
    char text[32];
    text[0] = exponent > 0 ? '1' : '0';
    int length = 1;
    if (exponent != 0) {
        exponent -= 1023;
    } else if (((high << 1) | low) != 0) {
        exponent = -1022;                           // a denormal
    }
    if (precision > 0 || (pState->m_Flags & PRINTF_ALTERNATE) != 0) {
        text[1] = '.';
        length = 2;
    }
    int afterPoint = length;
    for (int i = 0; precision > 0 && i < 5; i++) {
        u32 digit = (high << 12) >> 28;
        high <<= 4;
        precision--;
        text[length++] = pDigits[digit];
    }
    for (int i = 0; precision > 0 && i < 8; i++) {
        u32 digit = low >> 28;
        low <<= 4;
        precision--;
        text[length++] = pDigits[digit];
    }
    if ((pState->m_Flags & PRINTF_PRECISION) == 0) {
        while (length > afterPoint && text[length - 1] == '0') {
            length--;
        }
        if (length > 0 && text[length - 1] == '.') {
            length--;
        }
    }
    if (precision > 0) {
        text[length++] = '>';                       // the remaining zeros
        precision--;
    }
    text[length++] = pDigits[17];
    if (exponent < 0) {
        text[length] = '-';
        exponent = -exponent;
    } else {
        text[length] = '+';
    }
    length++;
    if (exponent >= 1000) {
        text[length++] = exponent / 1000 + '0';
        exponent %= 1000;
        goto hundreds;
    }
    if (exponent >= 100) {
    hundreds:
        text[length++] = exponent / 100 + '0';
        exponent %= 100;
        goto tens;
    }
    if (exponent >= 10) {
    tens:
        text[length++] = exponent / 10 + '0';
        exponent %= 10;
    }
    text[length++] = exponent % 10 + '0';
    pState->m_Fields[PRINTF_WIDTH] -= length + precision + prefixLength;
    if ((pState->m_Flags & PRINTF_ZERO) == 0) {
        __printf_pre_padding(pState);
    }
    pState->m_Count += prefixLength;
    for (const char* p = prefix; prefixLength-- != 0;) {
        pState->m_PutChar(*p++, pState->m_pPutArgument);
    }
    if ((pState->m_Flags & PRINTF_ZERO) != 0) {
        __printf_pre_padding(pState);
    }
    for (const char* p = text; length-- != 0;) {
        char ch = *p++;
        if (ch == '>') {
            pState->m_Count += precision;
            while (precision-- != 0) {
                pState->m_PutChar('0', pState->m_pPutArgument);
            }
            continue;
        }
        PutChar(pState, ch);
    }
    __printf_post_padding(pState);
    return 2;
}

} // extern "C"
