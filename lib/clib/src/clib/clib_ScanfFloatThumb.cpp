// The floating point conversions of scanf (%e %f %g %a; Thumb code in the original): the decimal digits are collected
// into two byte strings (mantissa and exponent, a sign first and 0xFF at the end) and converted with the btod
// routines; "inf"/"infinity"/"nan(...)" and hexadecimal numbers ("0x...p...") have handlers of their own. All names
// are ours.

#include "decomp.h"
#include "clib/clib_Btod.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Scanf.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
// the end of the digit strings
const u8 DIGITS_END = 0xFF;

// the values of errno (names are ours)
const int ERROR_RANGE = 2;

// beyond 10^500 a double is 0 or infinite
const int MAX_DECIMAL_EXPONENT = 500;

// the exponent of an extended number whose mantissa (top bit set) is an integer
const u32 EXTENDED_INTEGER_EXPONENT = 0x403E;

const u32 DOUBLE_SIGN = 0x80000000;
const u32 DOUBLE_EXPONENT_MAX = 0x7FF;

// the double of an extended number: __btod_mult and __btod_div return it with the high word in r0
inline DoubleWords MultiplyToDouble(const Extended* pA, const Extended* pB, int rounding)
{
    register u32 r0 asm("r0") = reinterpret_cast<u32>(pA);
    register u32 r1 asm("r1") = reinterpret_cast<u32>(pB);
    register u32 r2 asm("r2") = rounding;
    asm volatile("bl __btod_mult" : "+r"(r0), "+r"(r1), "+r"(r2) : : "r3", "ip", "lr", "cc", "memory");
    DoubleWords result = {r1, r0};
    return result;
}

inline DoubleWords DivideToDouble(const Extended* pA, const Extended* pB, int rounding)
{
    register u32 r0 asm("r0") = reinterpret_cast<u32>(pA);
    register u32 r1 asm("r1") = reinterpret_cast<u32>(pB);
    register u32 r2 asm("r2") = rounding;
    asm volatile("bl __btod_div" : "+r"(r0), "+r"(r1), "+r"(r2) : : "r3", "ip", "lr", "cc", "memory");
    DoubleWords result = {r1, r0};
    return result;
}

inline int GetChar(ScanState* pState, void* pSource)
{
    return pState->m_GetChar(pSource);
}

inline void UngetChar(ScanState* pState, void* pSource)
{
    pState->m_UngetChar(pSource);
}
} // namespace

extern "C" {

int* __aeabi_errno_addr();
u32 __fp_status(u32 mask, u32 flags);
int __isdigit(int c);
int __digit_value(u32 c, u32 base);
void __mathlib_narrow(f32* result, const f64* value);
void __support_ldexp(f64* value, int exponent);
int __scanf_infnan(int unused, void* pSource, int* pConsumed, ScanState* pState, int count, u32 flags, int width,
                   int c);
int __scanf_hexfloat(int unused, void* pSource, int* pConsumed, ScanState* pState, int count, u32 flags, int width);

// *pResult = mantissa * 10^(exponent + pointAdjust) (both as digit strings: a sign, the digit values, 0xFF; the
// exponent string may also be just 0xFF), rounded in the direction of the floating point status; ERANGE when it
// overflows or underflows
// 0x00101C0C (name is ours)
void __scanf_make_double(DoubleWords* pResult, const u8* pExponent, const u8* pMantissa, int pointAdjust)
{
    int exponent;
    if (*pExponent == DIGITS_END) {
        exponent = 0;
    } else {
        int isExponentNegative = *pExponent++ == '-';
        exponent = 0;
        while (*pExponent != DIGITS_END) {
            exponent = *pExponent++ + exponent * 10;
        }
        if (isExponentNegative) {
            exponent = -exponent;
        }
    }

    int isNegative = 0;
    u64 mantissa = 0;
    if (*pMantissa == '-' || *pMantissa == '+') {
        isNegative = *pMantissa++ == '-';
    }
    for (int c; (c = *pMantissa) != DIGITS_END;) {
        pMantissa++;
        mantissa = mantissa * 10 + c;
    }

    // the rounding mode of the magnitude: up and down swap for a negative value; 1 up, 0 nearest, -1 down
    u32 mode = __fp_status(0, 0);
    if (isNegative) {
        mode = (mode * 5) >> 1;
    }
    int rounding = (mode << 8) >> 30;
    if (rounding > 1) {
        rounding = -1;
    }

    exponent += pointAdjust;
    if (mantissa == 0) {
        pResult->m_High = isNegative << 31;
        pResult->m_Low = 0;
        return;
    }
    if (exponent < -MAX_DECIMAL_EXPONENT) {
        pResult->m_High = isNegative << 31;
        pResult->m_Low = 0;
        *__aeabi_errno_addr() = ERROR_RANGE;
        return;
    }
    if (exponent > MAX_DECIMAL_EXPONENT) {
        *pResult = s_Infinity;
        pResult->m_High |= isNegative << 31;
        *__aeabi_errno_addr() = ERROR_RANGE;
        return;
    }

    Extended power;
    _btod_etento(&power, exponent < 0 ? -exponent : exponent, exponent < 0 ? -rounding : rounding);
    Extended value;
    value.m_SignExponent = EXTENDED_INTEGER_EXPONENT;
    if ((mantissa >> 32) == 0) {
        mantissa <<= 32;
        value.m_SignExponent = EXTENDED_INTEGER_EXPONENT - 32;
    }
    if ((mantissa >> 48) == 0) {
        mantissa <<= 16;
        value.m_SignExponent -= 16;
    }
    if ((mantissa >> 56) == 0) {
        mantissa <<= 8;
        value.m_SignExponent -= 8;
    }
    if ((mantissa >> 60) == 0) {
        mantissa <<= 4;
        value.m_SignExponent -= 4;
    }
    if ((mantissa >> 62) == 0) {
        mantissa <<= 2;
        value.m_SignExponent -= 2;
    }
    if ((mantissa >> 63) == 0) {
        mantissa += mantissa;
        value.m_SignExponent -= 1;
    }
    value.m_MantissaHigh = static_cast<u32>(mantissa >> 32);
    value.m_MantissaLow = static_cast<u32>(mantissa);
    DoubleWords result = exponent > 0 ? MultiplyToDouble(&value, &power, rounding)
                                      : DivideToDouble(&value, &power, rounding);
    if (isNegative) {
        result.m_High ^= DOUBLE_SIGN;
    }
    if (((result.m_High & ~DOUBLE_SIGN) | result.m_Low) == 0 || (result.m_High << 1) >> 21 == DOUBLE_EXPONENT_MAX) {
        *__aeabi_errno_addr() = ERROR_RANGE;
        if ((result.m_High << 1) >> 21 == DOUBLE_EXPONENT_MAX) {
            pResult->m_Low = s_Infinity.m_Low;
            pResult->m_High = s_Infinity.m_High | (result.m_High & DOUBLE_SIGN);
            return;
        }
    }
    *pResult = result;
}

// %e %f %g: optional sign, "inf"/"nan", "0x" for a hexadecimal number, digits with the decimal point of the locale,
// an exponent after 'e'; at most 18 digits are kept (the others only move the point)
// 0x00101E0A (name is ours)
int __scanf_float(int unused, void* pSource, int* pConsumed, ScanState* pState)
{
    int pointAdjust = 0;
    int width = pState->m_Width;
    u32 flags = pState->m_Flags;
    int count = -1;
    int zeros = 0;
    const NumericData* pNumeric = GetCLibraryLocale()->m_pNumeric;
    int point = reinterpret_cast<const u8*>(pNumeric)[pNumeric->m_DecimalPointOffset];
    int c;
    do {
        count++;
        c = GetChar(pState, pSource);
    } while (pState->m_IsSpace(c));
    if (c == SCAN_END) {
        return SCAN_END;
    }
    flags &= ~(SCANF_POINT | SCANF_DIGITS | SCANF_NEGATIVE);
    if (width > 0 && (c == '+' || c == '-')) {
        if (c == '-') {
            flags |= SCANF_NEGATIVE;
        }
        count++;
        c = GetChar(pState, pSource);
        width--;
    }

    u8 mantissa[20];    // the sign, up to 18 digits, the end
    u8 exponent[10];    // the sign, up to 8 digits, the end
    mantissa[0] = (flags & SCANF_NEGATIVE) != 0 ? '-' : '+';
    u8* pMantissa = &mantissa[1];
    u8* pExponent = exponent;
    if (width > 0) {
        if (c == 'i' || c == 'I' || c == 'n' || c == 'N') {
            int result = __scanf_infnan(SCAN_NOT_HANDLED, pSource, pConsumed, pState, count, flags, width, c);
            if (result != SCAN_NOT_HANDLED) {
                return result;
            }
        }
        while (c == '0') {
            count++;
            c = GetChar(pState, pSource);
            width--;
            flags |= SCANF_DIGITS;
            *pConsumed = count;
            zeros++;
            if (width <= 0) {
                break;
            }
        }
        if (width > 0 && (c == 'x' || c == 'X') && zeros == 1) {
            int result = __scanf_hexfloat(SCAN_NOT_HANDLED, pSource, pConsumed, pState, count, flags, width);
            if (result != SCAN_NOT_HANDLED) {
                return result;
            }
        }
    }

    // the zeros right after the point only move it (the width is not checked for them)
    if (c == point) {
        flags |= SCANF_POINT;
        for (;;) {
            width--;
            count++;
            c = GetChar(pState, pSource);
            if (c != '0') {
                break;
            }
            pointAdjust--;
            flags |= SCANF_DIGITS;
            *pConsumed = count + 1;
        }
    }
    while (width > 0) {
        if (c == point && (flags & SCANF_POINT) == 0) {
            flags |= SCANF_POINT;
            width--;
        } else {
            if (!__isdigit(c)) {
                if ((c == 'e' || c == 'E') && (flags & SCANF_DIGITS) != 0) {
                    flags &= ~(SCANF_DIGITS | SCANF_EXPONENT_NEGATIVE);
                    width--;
                    count++;
                    c = GetChar(pState, pSource);
                    if (width > 0 && (c == '+' || c == '-')) {
                        if (c == '-') {
                            flags |= SCANF_EXPONENT_NEGATIVE;
                        }
                        count++;
                        c = GetChar(pState, pSource);
                        width--;
                    }
                    *pExponent++ = (flags & SCANF_EXPONENT_NEGATIVE) != 0 ? '-' : '+';
                    const u8* pExponentDigits = pExponent;
                    while (width > 0 && __isdigit(c)) {
                        flags |= SCANF_DIGITS;
                        width--;
                        if (pExponent < &exponent[9]) {
                            // (leading zeros are dropped)
                            u8 digit = c - '0';
                            *pExponent = digit;
                            if (digit != 0 || pExponent > pExponentDigits) {
                                pExponent++;
                            }
                        } else {
                            pointAdjust = (flags & SCANF_EXPONENT_NEGATIVE) != 0 ? -9999 : 9999;
                        }
                        count++;
                        c = GetChar(pState, pSource);
                        *pConsumed = count;
                    }
                }
                break;
            }
            flags |= SCANF_DIGITS;
            width--;
            if (pMantissa < &mantissa[19]) {
                *pMantissa++ = c - '0';
                if ((flags & SCANF_POINT) != 0) {
                    pointAdjust--;
                }
            } else if ((flags & SCANF_POINT) == 0) {
                pointAdjust++;
            }
        }
        if ((flags & SCANF_DIGITS) != 0) {
            *pConsumed = count + 1;
        }
        count++;
        c = GetChar(pState, pSource);
    }

    UngetChar(pState, pSource);
    *pMantissa = DIGITS_END;
    *pExponent = DIGITS_END;
    DoubleWords value = {0, 0};
    __scanf_make_double(&value, exponent, mantissa, pointAdjust);
    if ((flags & SCANF_DIGITS) == 0) {
        return SCAN_MISMATCH;
    }
    if ((flags & (SCANF_LONG | SCANF_LONG_DOUBLE)) != 0) {
        if ((flags & SCANF_SUPPRESS) == 0) {
            *static_cast<DoubleWords*>(NextTarget(pState)) = value;
        }
    } else {
        f32 result;
        __mathlib_narrow(&result, reinterpret_cast<const f64*>(&value));
        if ((flags & SCANF_SUPPRESS) == 0) {
            *static_cast<f32*>(NextTarget(pState)) = result;
        }
    }
    return count;
}

// "inf", "infinity", "nan" and "nan(...)" (any case); c is the first character
// 0x001036A8 (name is ours)
int __scanf_infnan(int unused, void* pSource, int* pConsumed, ScanState* pState, int count, u32 flags, int width,
                   int c)
{
    u32 sign = ((flags & SCANF_NEGATIVE) != 0) << 31;
    DoubleWords value = {0, 0x7FF00000 | sign};
    u32 floatValue = 0x7F800000 | sign;
    width--;
    switch (c & ~0x20) {
    case 'I': {
        // "INF" and "INFINITY" match, everything in between is a mismatch
        const char infinity[12] = "INFINITY";
        for (int i = 1;;) {
            count++;
            c = GetChar(pState, pSource);
            if (--width < 0 || infinity[i] == '\0' || (c & ~0x20) != infinity[i]) {
                UngetChar(pState, pSource);
                if (i == 3 || i == 8) {
                    break;
                }
                return SCAN_MISMATCH;
            }
            i++;
            if (i == 3 || i == 8) {
                *pConsumed = count + 1;
            }
        }
        break;
    }
    case 'N':
        count++;
        c = GetChar(pState, pSource);
        if (--width < 0 || (c & ~0x20) != 'A') {
            UngetChar(pState, pSource);
            return SCAN_MISMATCH;
        }
        count++;
        c = GetChar(pState, pSource);
        if (--width < 0 || (c & ~0x20) != 'N') {
            UngetChar(pState, pSource);
            return SCAN_MISMATCH;
        }
        count++;
        c = GetChar(pState, pSource);
        width--;
        // a quiet NaN
        value.m_High |= 0x80000;
        floatValue |= 0x400000;
        *pConsumed = count;
        if (width >= 0 && c == '(') {
            do {
                count++;
                c = GetChar(pState, pSource);
                if (--width < 0 || c < 0) {
                    UngetChar(pState, pSource);
                    return SCAN_MISMATCH;
                }
            } while (c != ')');
            *pConsumed = ++count;
        } else {
            UngetChar(pState, pSource);
        }
        break;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        if ((flags & (SCANF_LONG | SCANF_LONG_DOUBLE)) != 0) {
            *static_cast<DoubleWords*>(NextTarget(pState)) = value;
        } else {
            *static_cast<u32*>(NextTarget(pState)) = floatValue;
        }
    }
    return count;
}

// a hexadecimal number after "0x": hexadecimal digits with '.', then 'p' and a decimal power of two; rounded to
// the precision of the result (also into the denormals) in the direction of the floating point status
// 0x00103370 (name is ours)
int __scanf_hexfloat(int unused, void* pSource, int* pConsumed, ScanState* pState, int count, u32 flags, int width)
{
    u32 sign = ((flags & SCANF_NEGATIVE) != 0) << 31;
    int isNonZero = 0;
    u64 mantissa = 0;
    int binaryExponent = -1;
    int hasDigits = 0;
    int isAfterPoint = 0;
    int shift = 60;
    count++;
    int c = GetChar(pState, pSource);
    width--;
    for (;;) {
        if (width <= 0) {
            UngetChar(pState, pSource);
            if (hasDigits == 0) {
                return SCAN_MISMATCH;
            }
            break;
        }
        if (c == '.') {
            isAfterPoint = 1;
        } else {
            int digit = __digit_value(c, 16);
            if (digit < 0) {
                if (c != 'p' && c != 'P') {
                    UngetChar(pState, pSource);
                    if (hasDigits == 0) {
                        return SCAN_MISMATCH;
                    }
                    break;
                }
                int power = 0;
                int powerSign = 1;
                if (hasDigits == 0) {
                    UngetChar(pState, pSource);
                    return SCAN_MISMATCH;
                }
                count++;
                c = GetChar(pState, pSource);
                if (--width <= 0) {
                    UngetChar(pState, pSource);
                    return SCAN_MISMATCH;
                }
                if (c == '+' || c == '-') {
                    if (c == '-') {
                        powerSign = -1;
                    }
                    count++;
                    c = GetChar(pState, pSource);
                    if (--width <= 0) {
                        UngetChar(pState, pSource);
                        return SCAN_MISMATCH;
                    }
                }
                if (c == SCAN_END || (digit = __digit_value(c, 10)) < 0) {
                    UngetChar(pState, pSource);
                    return SCAN_MISMATCH;
                }
                do {
                    power = power * 10 + digit;
                    count++;
                    c = GetChar(pState, pSource);
                    digit = __digit_value(c, 10);
                    width--;
                    *pConsumed = count;
                } while (width > 0 && digit >= 0);
                UngetChar(pState, pSource);
                binaryExponent += power * powerSign;
                break;
            }
            hasDigits = 1;
            // (leading zeros are skipped; digits beyond the 64 bits only set the lowest bit)
            if (isNonZero || digit != 0) {
                if (shift >= 0) {
                    mantissa |= static_cast<u64>(digit) << shift;
                    shift -= 4;
                } else if (digit != 0) {
                    mantissa |= 1;
                }
                isNonZero = 1;
                if (isAfterPoint == 0) {
                    binaryExponent += 4;
                }
            }
        }
        count++;
        c = GetChar(pState, pSource);
        width--;
        if (hasDigits != 0) {
            *pConsumed = count;
        }
    }

    u32 isDouble = flags & (SCANF_LONG | SCANF_LONG_DOUBLE);
    DoubleWords value;
    if (mantissa == 0) {
        value.m_High = sign;
        value.m_Low = 0;
    } else {
        // the first digit is not 0, so at most three shifts bring its top bit to bit 63
        if ((mantissa >> 62) == 0) {
            mantissa <<= 2;
            binaryExponent -= 2;
        }
        if ((mantissa >> 63) == 0) {
            mantissa += mantissa;
            binaryExponent -= 1;
        }
        // the bits below the precision of the result
        int dropBits;
        int minExponent;
        if (isDouble != 0) {
            dropBits = 11;
            minExponent = -1022;
        } else {
            dropBits = 40;
            minExponent = -126;
        }
        if (binaryExponent < minExponent) {
            dropBits += minExponent - binaryExponent;
            if (dropBits > 65) {
                dropBits = 65;
            }
        }
        u64 half = static_cast<u64>(1) << (dropBits - 1);
        int isHalf = (mantissa & half) != 0;
        int isBelowHalf = (mantissa & (half - 1)) != 0;
        if (isHalf || isBelowHalf) {
            int mode = __fp_status(0, 0) & 0xC00000;
            if (sign != 0) {
                mode = mode * 5 / 2 & 0xC00000;
            }
            int rounding = mode >> 22;
            if (rounding > 1) {
                rounding = -1;
            }
            if (rounding == 1 || (rounding != -1 && isHalf &&
                                  (isBelowHalf || (mantissa & static_cast<u64>(1) << dropBits) != 0))) {
                u64 unit = static_cast<u64>(1) << dropBits;
                u64 rounded = mantissa + unit;
                if (rounded < mantissa) {
                    // carried out of the 64 bits
                    if (dropBits > 64) {
                        binaryExponent = minExponent - (isDouble != 0 ? 52 : 23);
                    } else {
                        binaryExponent++;
                    }
                    mantissa = static_cast<u64>(1) << 63;
                } else {
                    mantissa = rounded & ~(unit - 1);
                }
            }
        }
        if (binaryExponent > 1 - minExponent) {
            value.m_High = sign | 0x7FF00000;
            value.m_Low = 0;
        } else {
            // 1.mantissa * 2^binaryExponent
            u64 fraction = (mantissa << 1) >> 12;
            value.m_Low = static_cast<u32>(fraction);
            value.m_High = sign | static_cast<u32>(fraction >> 32) | 0x3FF00000;
            __support_ldexp(reinterpret_cast<f64*>(&value), binaryExponent);
        }
    }
    if (isDouble != 0) {
        if ((flags & SCANF_SUPPRESS) == 0) {
            *static_cast<DoubleWords*>(NextTarget(pState)) = value;
        }
    } else {
        f32 result;
        __mathlib_narrow(&result, reinterpret_cast<const f64*>(&value));
        if ((flags & SCANF_SUPPRESS) == 0) {
            *static_cast<f32*>(NextTarget(pState)) = result;
        }
    }
    return count;
}

} // extern "C"
