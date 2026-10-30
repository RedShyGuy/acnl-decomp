// scanf of the ARM C library (Thumb code in the original): sscanf with its string source, the engine, and the
// handlers of the integer, character and string conversions (narrow and wide). See clib_Scanf.h. All names except
// the C library functions are ours.

#include <stdarg.h>
#include "decomp.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Scanf.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
inline const u8* Arguments(va_list arguments)
{
    return *reinterpret_cast<const u8* const*>(&arguments);
}

// a bit of the set of %[...]
inline bool InSet(const u32* pSet, int c)
{
    return (pSet[c / 32] & (1 << (c % 32))) != 0;
}
} // namespace

extern "C" {

int __digit_value(u32 c, u32 base);
__attribute__((pcs("aapcs"))) u64 __aeabi_lmul(u64 a, u64 b);
int __mbrtowc_byte(wchar_t* character, u8 byte, mbstate_t* state);
int __wcrtomb_char(char* buffer, wchar_t character, mbstate_t* state);
int __scanf_float(int unused, void* pSource, int* pConsumed, ScanState* pState);

// ---- the string source of sscanf

// 0x001020D0 (name is ours)
int __scanf_string_getc(void* pSource)
{
    StringSource* pString = static_cast<StringSource*>(pSource);
    int remaining = pString->m_Remaining;
    if (remaining != 0) {
        const char* pCurrent = pString->m_pCurrent;
        int c = static_cast<u8>(*pCurrent++);
        if (c != 0) {
            pString->m_pCurrent = pCurrent;
            pString->m_Remaining = remaining - 1;
            return c;
        }
    }
    pString->m_IsEnd = 1;
    return SCAN_END;
}

// 0x001020F0 (name is ours)
int __scanf_string_ungetc(void* pSource)
{
    StringSource* pString = static_cast<StringSource*>(pSource);
    int remaining = pString->m_Remaining;
    if (remaining != 0 && pString->m_IsEnd == 0) {
        const char* pCurrent = pString->m_pCurrent;
        if (pString->m_pStart != pCurrent) {
            pString->m_pCurrent = pCurrent - 1;
            pString->m_Remaining = remaining + 1;
            return 0;
        }
    }
    return -1;
}

// 0x002F8070 (name is ours)
int __scanf_getf_narrow(const void** ppFormat, int step)
{
    const u8* pFormat = static_cast<const u8*>(*ppFormat);
    int c = *pFormat;
    *ppFormat = pFormat + step;
    return c;
}

// 0x001024E8 (name is ours)
int __scanf_isspace(int c)
{
    return ((*__rt_ctype_table())[c] & CTYPE_SPACE) != 0;
}

// ---- the handlers

// %d %i %o %u %x %p (32 bits; with h and hh)
// 0x00102D20 (name is ours)
int __scanf_int(int unused, void* pSource, int base, ScanState* pState)
{
    int count = -1;
    u32 flags = pState->m_Flags;
    u32 value = 0;
    int width = pState->m_Width;
    int c;
    do {
        count++;
        c = pState->m_GetChar(pSource);
    } while (pState->m_IsSpace(c));
    if (c == SCAN_END) {
        return SCAN_END;
    }
    flags &= ~(SCANF_DIGITS | SCANF_NEGATIVE);
    if (width > 0) {
        if ((flags & SCANF_SIGNED) != 0 && (c == '+' || c == '-')) {
            if (c == '-') {
                flags |= SCANF_NEGATIVE;
            }
            count++;
            c = pState->m_GetChar(pSource);
            if (--width <= 0) {
                goto noPrefix;
            }
        }
        if (c == '0') {
            flags |= SCANF_DIGITS;
            width--;
            count++;
            c = pState->m_GetChar(pSource);
            if (width > 0 && (c == 'x' || c == 'X')) {
                if (base == 0 || base == 16) {
                    flags &= ~SCANF_DIGITS;
                    width--;
                    count++;
                    c = pState->m_GetChar(pSource);
                    base = 16;
                }
            } else if (base == 0) {
                base = 8;
            }
            goto digits;
        }
    }
noPrefix:
    if (base == 0) {
        base = 10;
    }
digits:
    for (int digit; width > 0 && (digit = __digit_value(c, base)) >= 0;) {
        flags |= SCANF_DIGITS;
        width--;
        value = value * base + digit;
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if ((flags & SCANF_DIGITS) == 0) {
        return SCAN_MISMATCH;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        if ((flags & SCANF_SIGNED) != 0) {
            if ((flags & SCANF_NEGATIVE) != 0) {
                value = -value;
            }
            void* pTarget = NextTarget(pState);
            if ((flags & SCANF_CHAR) != 0) {
                *static_cast<u8*>(pTarget) = value;
            } else if ((flags & SCANF_SHORT) != 0) {
                *static_cast<u16*>(pTarget) = value;
            } else {
                *static_cast<u32*>(pTarget) = value;
            }
        } else {
            void* pTarget = NextTarget(pState);
            if ((flags & SCANF_CHAR) != 0) {
                *static_cast<u8*>(pTarget) = value;
            } else if ((flags & SCANF_SHORT) != 0) {
                *static_cast<u16*>(pTarget) = value;
            } else {
                *static_cast<u32*>(pTarget) = value;
            }
        }
    }
    return count;
}

// %lld ... (64 bits)
// 0x00102BDC (name is ours)
int __scanf_longlong(int unused, void* pSource, int base, ScanState* pState)
{
    int count = -1;
    u64 value = 0;
    int width = pState->m_Width;
    u32 flags = pState->m_Flags;
    int c;
    do {
        count++;
        c = pState->m_GetChar(pSource);
    } while (pState->m_IsSpace(c));
    if (c == SCAN_END) {
        return SCAN_END;
    }
    flags &= ~(SCANF_DIGITS | SCANF_NEGATIVE);
    if (width > 0) {
        if ((flags & SCANF_SIGNED) != 0 && (c == '+' || c == '-')) {
            if (c == '-') {
                flags |= SCANF_NEGATIVE;
            }
            count++;
            c = pState->m_GetChar(pSource);
            if (--width <= 0) {
                goto noPrefix;
            }
        }
        if (c == '0') {
            flags |= SCANF_DIGITS;
            width--;
            count++;
            c = pState->m_GetChar(pSource);
            if (width > 0 && (c == 'x' || c == 'X')) {
                if (base == 0 || base == 16) {
                    flags &= ~SCANF_DIGITS;
                    width--;
                    count++;
                    c = pState->m_GetChar(pSource);
                    base = 16;
                }
            } else if (base == 0) {
                base = 8;
            }
            goto digits;
        }
    }
noPrefix:
    if (base == 0) {
        base = 10;
    }
digits:
    for (int digit; width > 0 && (digit = __digit_value(c, base)) >= 0;) {
        flags |= SCANF_DIGITS;
        width--;
        value = __aeabi_lmul(value, static_cast<s64>(base)) + static_cast<s64>(digit);
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if ((flags & SCANF_DIGITS) == 0) {
        return SCAN_MISMATCH;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        if ((flags & SCANF_SIGNED) != 0) {
            u64 result = (flags & SCANF_NEGATIVE) != 0 ? -value : value;
            *static_cast<u64*>(NextTarget(pState)) = result;
        } else {
            *static_cast<u64*>(NextTarget(pState)) = value;
        }
    }
    return count;
}

// %s %c %[ into a char array (pSet: the 256 bits of %[, NULL for %s and %c)
// 0x00102E6A (name is ours)
int __scanf_string(int unused, void* pSource, ScanState* pState, const u32* pSet, int isChar, int negate, int setCount)
{
    char* pTarget = NULL;
    int count = 0;
    u32 flags = pState->m_Flags;
    int width = pState->m_Width;
    int c = pState->m_GetChar(pSource);
    if ((reinterpret_cast<uptr>(pSet) | isChar) == 0) {
        while (pState->m_IsSpace(c)) {
            count++;
            c = pState->m_GetChar(pSource);
        }
    }
    if (c == SCAN_END) {
        return SCAN_END;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        pTarget = static_cast<char*>(NextTarget(pState));
    }
    while (width > 0 && c != SCAN_END) {
        if (pSet != NULL) {
            if (!InSet(pSet, c)) {
                break;
            }
        } else if (isChar == 0 && pState->m_IsSpace(c)) {
            break;
        }
        if ((flags & SCANF_SUPPRESS) == 0) {
            *pTarget++ = c;
        }
        width--;
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if (((flags & SCANF_SUPPRESS) | isChar) == 0) {
        *pTarget = 0;
    }
    // nothing read, or %c with fewer characters than its width
    if (pState->m_Width <= width || (isChar != 0 && width != 0)) {
        return SCAN_MISMATCH;
    }
    return count;
}

// %ls %lc %l[ from a narrow input into a wchar_t array
// 0x00103020 (name is ours)
int __scanf_wstring(int unused, void* pSource, ScanState* pState, const u32* pSet, int isChar, int negate, int setCount)
{
    wchar_t* pTarget = NULL;
    int count = 0;
    u32 flags = pState->m_Flags;
    int incomplete = 0;
    mbstate_t state = {};
    int width = pState->m_Width;
    int c = pState->m_GetChar(pSource);
    if ((reinterpret_cast<uptr>(pSet) | isChar) == 0) {
        while (pState->m_IsSpace(c)) {
            count++;
            c = pState->m_GetChar(pSource);
        }
    }
    if (c == SCAN_END) {
        return SCAN_END;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        pTarget = static_cast<wchar_t*>(NextTarget(pState));
    }
    while (width > 0 && c != SCAN_END) {
        if (pSet != NULL) {
            if (!InSet(pSet, c)) {
                break;
            }
        } else if (isChar == 0 && pState->m_IsSpace(c)) {
            break;
        }
        wchar_t character;
        int converted = __mbrtowc_byte(&character, static_cast<u8>(c), &state);
        if (converted == -1) {
            pState->m_UngetChar(pSource);
            return SCAN_MISMATCH;
        }
        if (converted == -2) {
            incomplete = 1;                         // more bytes of the character follow
        } else {
            incomplete = 0;
            if ((flags & SCANF_SUPPRESS) == 0) {
                *pTarget++ = character;
            }
            width--;
        }
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if (incomplete != 0) {
        return SCAN_MISMATCH;
    }
    if (((flags & SCANF_SUPPRESS) | isChar) == 0) {
        *pTarget = 0;
    }
    if (pState->m_Width <= width || (isChar != 0 && width != 0)) {
        return SCAN_MISMATCH;
    }
    return count;
}

// a wide character in the set of %[ of a wide format (setCount characters at pSet)
// 0x002FA614 (name is ours)
int __scanf_wide_in_set(const wchar_t* pSet, u32 setCount, int negate, int c)
{
    while (setCount-- != 0) {
        if (*pSet++ == c) {
            return negate != 0 ? 0 : 1;
        }
    }
    return negate;
}

// %s %c %[ of a wide input into a char array
// 0x0010313A (name is ours)
int __scanf_wide_string(int unused, void* pSource, ScanState* pState, const wchar_t* pSet, int setCount, int negate,
                        int isChar)
{
    char* pTarget = NULL;
    u32 flags = pState->m_Flags;
    int width = pState->m_Width;
    mbstate_t state = {};
    int count = 0;
    int c = pState->m_GetChar(pSource);
    if ((reinterpret_cast<uptr>(pSet) | isChar) == 0) {
        while (pState->m_IsSpace(c)) {
            count++;
            c = pState->m_GetChar(pSource);
        }
    }
    if (c == SCAN_END) {
        return SCAN_END;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        pTarget = static_cast<char*>(NextTarget(pState));
    }
    while (width > 0 && c != SCAN_END) {
        if (pSet != NULL) {
            if (!__scanf_wide_in_set(pSet, setCount, negate, c)) {
                break;
            }
        } else if (isChar == 0 && pState->m_IsSpace(c)) {
            break;
        }
        if ((flags & SCANF_SUPPRESS) == 0) {
            int converted = __wcrtomb_char(pTarget, static_cast<wchar_t>(c), &state);
            if (converted == -1) {
                return SCAN_MISMATCH;
            }
            pTarget += converted;
        }
        width--;
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if (((flags & SCANF_SUPPRESS) | isChar) == 0) {
        *pTarget = 0;
    }
    if (pState->m_Width <= width || (isChar != 0 && width != 0)) {
        return SCAN_MISMATCH;
    }
    return count;
}

// %ls %lc %l[ of a wide input into a wchar_t array
// 0x0010321E (name is ours)
int __scanf_wide_wstring(int unused, void* pSource, ScanState* pState, const wchar_t* pSet, int setCount, int negate,
                         int isChar)
{
    wchar_t* pTarget = NULL;
    int count = 0;
    u32 flags = pState->m_Flags;
    int width = pState->m_Width;
    int c = pState->m_GetChar(pSource);
    if ((reinterpret_cast<uptr>(pSet) | isChar) == 0) {
        while (pState->m_IsSpace(c)) {
            count++;
            c = pState->m_GetChar(pSource);
        }
    }
    if (c == SCAN_END) {
        return SCAN_END;
    }
    if ((flags & SCANF_SUPPRESS) == 0) {
        pTarget = static_cast<wchar_t*>(NextTarget(pState));
    }
    while (width > 0 && c != SCAN_END) {
        if (pSet != NULL) {
            if (!__scanf_wide_in_set(pSet, setCount, negate, c)) {
                break;
            }
        } else if (isChar == 0 && pState->m_IsSpace(c)) {
            break;
        }
        if ((flags & SCANF_SUPPRESS) == 0) {
            *pTarget++ = c;
        }
        width--;
        count++;
        c = pState->m_GetChar(pSource);
    }
    pState->m_UngetChar(pSource);
    if (((flags & SCANF_SUPPRESS) | isChar) == 0) {
        *pTarget = 0;
    }
    if (pState->m_Width <= width || (isChar != 0 && width != 0)) {
        return SCAN_MISMATCH;
    }
    return count;
}

// ---- the engine

// the conversions of the format; the number of results assigned (-1: the input ended before the first one)
// 0x00102848 (name is ours)
int __scanf_engine(void* pSource, ScanState* pState)
{
    int assigned = 0;
    int isFirst = 1;
    int consumed = 0;
    const void** ppFormat = &pState->m_pFormat;
    for (;;) {
        int c = pState->m_GetFormatChar(ppFormat, 1);
        if (c == 0) {
            return assigned;
        }
        if (c != '%') {
            if (pState->m_IsSpace(c)) {
                // white space in the format: any white space of the input
                while (pState->m_IsSpace(pState->m_GetFormatChar(ppFormat, 1))) {
                }
                pState->m_GetFormatChar(ppFormat, -1);
                while (pState->m_IsSpace(pState->m_GetChar(pSource))) {
                    consumed++;
                }
                pState->m_UngetChar(pSource);
                continue;
            }
            // a character of the format: the same in the input
            int input = pState->m_GetChar(pSource);
            if (input == c) {
                consumed++;
                continue;
            }
            pState->m_UngetChar(pSource);
            if (input != SCAN_END || assigned != 0) {
                return assigned;
            }
            return -1;
        }
        int width = 0;
        u32 flags = 0;
        if (pState->m_GetFormatChar(ppFormat, 0) == '*') {
            pState->m_GetFormatChar(ppFormat, 1);
            flags = SCANF_SUPPRESS;
        }
        for (;;) {
            c = pState->m_GetFormatChar(ppFormat, 1);
            if (static_cast<u32>(c - '0') >= 10) {
                break;
            }
            if (width > 0x0CCCCCCC) {
                return assigned;
            }
            width = width * 10 + c - '0';
            if (width < 0) {
                return assigned;
            }
            flags |= SCANF_WIDTH;
        }
        if ((flags & SCANF_WIDTH) == 0) {
            width = 0x7FFFFFFF;
        }
        if (c == 'l') {
            c = pState->m_GetFormatChar(ppFormat, 1);
            if (c == 'l') {
                flags |= SCANF_LONG_LONG;
                c = pState->m_GetFormatChar(ppFormat, 1);
            } else {
                flags |= SCANF_LONG;
            }
        } else if (c == 'L') {
            flags |= SCANF_LONG_DOUBLE;
            c = pState->m_GetFormatChar(ppFormat, 1);
        } else if (c == 'h') {
            c = pState->m_GetFormatChar(ppFormat, 1);
            if (c == 'h') {
                flags |= SCANF_CHAR;
                c = pState->m_GetFormatChar(ppFormat, 1);
            } else {
                flags |= SCANF_SHORT;
            }
        } else if (c == 'j') {
            flags |= SCANF_LONG_LONG;
            c = pState->m_GetFormatChar(ppFormat, 1);
        } else if (c == 't' || c == 'z') {
            c = pState->m_GetFormatChar(ppFormat, 1);
        }
        pState->m_Width = width;
        pState->m_Flags = flags;
        int result;
        int base;
        switch (c) {
        case 'A':
        case 'E':
        case 'F':
        case 'G':
        case 'a':
        case 'e':
        case 'f':
        case 'g': {
            int floatConsumed;
            result = __scanf_float(-2, pSource, &floatConsumed, pState);
            break;
        }
        case '%': {
            int input = pState->m_GetChar(pSource);
            if (input == '%') {
                consumed++;
                continue;
            }
            pState->m_UngetChar(pSource);
            if (input != SCAN_END || assigned != 0) {
                return assigned;
            }
            return -1;
        }
        case 'n': {
            void* pTarget = NextTarget(pState);
            if ((flags & SCANF_CHAR) != 0) {
                *static_cast<u8*>(pTarget) = consumed;
            } else if ((flags & SCANF_SHORT) != 0) {
                *static_cast<u16*>(pTarget) = consumed;
            } else if ((flags & SCANF_LONG_LONG) != 0) {
                *static_cast<s64*>(pTarget) = consumed;
            } else {
                *static_cast<int*>(pTarget) = consumed;
            }
            continue;
        }
        case 'p':
            pState->m_Flags = flags & 0xFFFFF7F1;   // a pointer: no length
            result = __scanf_int(-2, pSource, 16, pState);
            break;
        case 'd':
        case 'u':
            base = 10;
            goto integer;
        case 'i':
            base = 0;
            goto integer;
        case 'o':
            base = 8;
            goto integer;
        case 'X':
        case 'x':
            base = 16;
        integer:
            pState->m_Flags = flags | SCANF_SIGNED;
            if ((flags & SCANF_LONG_LONG) != 0) {
                result = __scanf_longlong(-2, pSource, base, pState);
            } else {
                result = __scanf_int(-2, pSource, base, pState);
            }
            break;
        case '[':
        case 'c':
        case 's': {
            int isChar = 0;
            int negate = 0;
            int setCount = 0;
            const wchar_t* pWideSet = NULL;
            u32 set[8];
            const u32* pSet = NULL;
            if (c == 'c') {
                if ((flags & SCANF_WIDTH) == 0) {
                    pState->m_Width = 1;
                }
                isChar = 1;
            } else if (c == '[') {
                c = pState->m_GetFormatChar(ppFormat, 1);
                if (c == '^') {
                    negate = 1;
                    c = pState->m_GetFormatChar(ppFormat, 1);
                }
                if (pState->m_IsWide != 0) {
                    pWideSet = static_cast<const wchar_t*>(pState->m_pFormat) - 1;
                } else {
                    for (int i = 0; i < 8; i++) {
                        set[i] = 0;
                    }
                }
                // (the first character belongs to the set even if it is ']')
                do {
                    if (c == 0) {
                        return assigned;
                    }
                    if (pState->m_IsWide != 0) {
                        setCount++;
                    } else {
                        set[c / 32] |= 1 << (c % 32);
                    }
                    c = pState->m_GetFormatChar(ppFormat, 1);
                } while (c != ']');
                if (negate != 0) {
                    for (int i = 0; i < 8; i++) {
                        set[i] = ~set[i];
                    }
                }
                pSet = set;
            }
            if (pState->m_IsWide != 0) {
                if ((pState->m_Flags & SCANF_LONG) != 0) {
                    result = __scanf_wide_wstring(-2, pSource, pState, pWideSet, setCount, negate, isChar);
                } else {
                    result = __scanf_wide_string(-2, pSource, pState, pWideSet, setCount, negate, isChar);
                }
            } else if ((pState->m_Flags & SCANF_LONG) != 0) {
                result = __scanf_wstring(-2, pSource, pState, pSet, isChar, negate, setCount);
            } else {
                result = __scanf_string(-2, pSource, pState, pSet, isChar, negate, setCount);
            }
            break;
        }
        default:
            return assigned;
        }
        if (result < 0) {
            if (result == SCAN_END && isFirst != 0) {
                return -1;
            }
            return assigned;
        }
        if ((flags & SCANF_SUPPRESS) == 0) {
            assigned++;
        }
        consumed += result;
        isFirst = 0;
    }
}

// 0x002F807C (name is ours)
int __scanf_narrow(void* pSource, const char* pFormat, ScanState* pState)
{
    pState->m_pFormat = pFormat;
    pState->m_GetFormatChar = __scanf_getf_narrow;
    pState->m_IsSpace = __scanf_isspace;
    pState->m_IsWide = 0;
    return __scanf_engine(pSource, pState);
}

// 0x00101BCC (name is ours, after the C standard)
// 0x001012F0 (copy)
int sscanf(const char* pString, const char* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    StringSource source;
    ScanState state;
    source.m_pCurrent = pString;
    source.m_pStart = pString;
    source.m_Remaining = -1;
    state.m_pArguments = Arguments(arguments);
    source.m_IsEnd = 0;
    state.m_GetChar = __scanf_string_getc;
    state.m_UngetChar = __scanf_string_ungetc;
    return __scanf_narrow(&source, pFormat, &state);
}

} // extern "C"
