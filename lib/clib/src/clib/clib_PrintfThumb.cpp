// printf of the ARM C library (Thumb code in the original): the wrappers, the narrow and wide front ends, the
// engine that parses the format, and the formatters of the integer, character and string conversions (the
// floating point ones are separate). See clib_Printf.h. All names except the C library functions are ours.

#include <stdarg.h>
#include "decomp.h"
#include "clib/clib_Printf.h"
#include "clib/clib_Stdio.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
// a buffer with an end (snprintf): characters behind the end are dropped
struct LimitedBuffer
{
    char* m_pCurrent;
    char* m_pEnd;
};

struct LimitedWideBuffer
{
    wchar_t* m_pCurrent;
    wchar_t* m_pEnd;
};

// the address of the arguments behind a va_list
inline const u8* Arguments(va_list arguments)
{
    return *reinterpret_cast<const u8* const*>(&arguments);
}

// the arguments aligned to 8 (a 64 bit value)
inline const u8* Align8(const u8* pArguments)
{
    return reinterpret_cast<const u8*>((reinterpret_cast<uptr>(pArguments) + 7) >> 3 << 3);
}

inline void PutChar(PrintfState* pState, int c)
{
    pState->m_PutChar(c, pState->m_pPutArgument);
    pState->m_Count++;
}

// _ll_udiv10 returns the quotient in r0/r1 and the remainder in r2 (__value_in_regs): value /= 10, the remainder
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

// the flags of the characters ' ' to '0'
// 0x008C9404
extern const u8 s_PrintfFlagChars[17] = {
    PRINTF_SPACE, 0, 0, PRINTF_ALTERNATE, 0, 0, 0, 0, 0, 0, 0, PRINTF_PLUS, 0, PRINTF_LEFT, 0, 0, PRINTF_ZERO,
};
// the hexadecimal digits, then the prefix of %#p ("@") and of %#x ("0x")
// 0x008C93DC
extern const char s_HexDigitsUpper[20] = "0123456789ABCDEF@0X";
// 0x008C93F0
extern const char s_HexDigitsLower[20] = "0123456789abcdef@0x";

extern "C" {

__attribute__((pcs("aapcs"))) u64 _udiv10(u32 value);
int __isdigit(int c);
int __mbrtowc_byte(wchar_t* character, u8 byte, mbstate_t* state);
int __wcrtomb_char(char* buffer, wchar_t character, mbstate_t* state);
extern File __stdout;

// ---- the output and input of the front ends

// 0x002F7A98 (name is ours)
DECOMP_NOINLINE void __printf_putc_buffer(int c, void* pArgument)
{
    char** ppCurrent = static_cast<char**>(pArgument);
    **ppCurrent = c;
    (*ppCurrent)++;
}

// 0x00101A90 (name is ours)
void __printf_putc_limited(int c, void* pArgument)
{
    LimitedBuffer* pBuffer = static_cast<LimitedBuffer*>(pArgument);
    char* pCurrent = pBuffer->m_pCurrent;
    if (pCurrent < pBuffer->m_pEnd) {
        pBuffer->m_pCurrent = pCurrent + 1;
        *pCurrent = c;
    }
}

// 0x002F7B6C (name is ours)
DECOMP_NOINLINE void __printf_putwc_limited(int c, void* pArgument)
{
    LimitedWideBuffer* pBuffer = static_cast<LimitedWideBuffer*>(pArgument);
    wchar_t* pCurrent = pBuffer->m_pCurrent;
    if (pCurrent < pBuffer->m_pEnd) {
        pBuffer->m_pCurrent = pCurrent + 1;
        *pCurrent = c;
    }
}

// 0x002F7A68 (name is ours)
int __printf_getc_narrow(PrintfState* pState)
{
    const u8* pFormat = static_cast<const u8*>(pState->m_pFormat);
    pState->m_pFormat = pFormat + 1;
    return *pFormat;
}

// 0x002F7B3C (name is ours)
int __printf_getc_wide(PrintfState* pState)
{
    const wchar_t* pFormat = static_cast<const wchar_t*>(pState->m_pFormat);
    pState->m_pFormat = pFormat + 1;
    return *pFormat;
}

// 0x002F7A72 (name is ours)
DECOMP_NOINLINE int __printf_narrow(const char* pFormat, void* pPutArgument, const u8* pArguments, PrintfPutChar putChar)
{
    PrintfState state;
    state.m_pPutArgument = pPutArgument;
    state.m_IsWide = 0;
    state.m_GetChar = __printf_getc_narrow;
    state.m_PutChar = putChar;
    state.m_pFormat = pFormat;
    return __printf_engine(&state, pArguments);
}

// 0x002F7B46 (name is ours)
DECOMP_NOINLINE int __printf_wide(const wchar_t* pFormat, void* pPutArgument, const u8* pArguments, PrintfPutChar putChar)
{
    PrintfState state;
    state.m_pPutArgument = pPutArgument;
    state.m_IsWide = 1;
    state.m_GetChar = __printf_getc_wide;
    state.m_PutChar = putChar;
    state.m_pFormat = pFormat;
    return __printf_engine(&state, pArguments);
}

// ---- the engine

// 0x002F9238 (name is ours)
DECOMP_NOINLINE int __printf_engine(PrintfState* pState, const u8* pArguments)
{
    pState->m_Count = 0;
    for (;;) {
        int c = pState->m_GetChar(pState);
        if (c == 0) {
            return pState->m_Count;
        }
        if (c != '%') {
            PutChar(pState, c);
            continue;
        }
        // the flags
        u32 flags = 0;
        for (;;) {
            c = pState->m_GetChar(pState);
            if (c < ' ' || static_cast<u32>(c) >= '1') {
                break;
            }
            u8 flag = s_PrintfFlagChars[c - ' '];
            if (flag == 0) {
                break;
            }
            flags |= flag;
        }
        if ((flags & PRINTF_PLUS) != 0) {
            flags &= ~PRINTF_SPACE;
        }
        // the width, then the precision (a number or '*')
        pState->m_Fields[PRINTF_PRECISION_FIELD] = 0;
        int field = 0;
        pState->m_Fields[PRINTF_WIDTH] = 0;
        for (;;) {
            if (c == '*') {
                pState->m_Fields[field] = *reinterpret_cast<const int*>(pArguments);
                pArguments += 4;
                c = pState->m_GetChar(pState);
                if (field == PRINTF_PRECISION_FIELD) {
                    if (pState->m_Fields[PRINTF_PRECISION_FIELD] < 0) {
                        flags &= ~PRINTF_PRECISION;
                    }
                    break;
                }
            } else {
                if (__isdigit(c)) {
                    int* pField = &pState->m_Fields[field];
                    *pField = c - '0';
                    while (__isdigit(c = pState->m_GetChar(pState))) {
                        *pField = *pField * 10 + c - '0';
                    }
                }
                if (field == PRINTF_PRECISION_FIELD) {
                    break;
                }
            }
            if (c != '.') {
                break;
            }
            c = pState->m_GetChar(pState);
            flags |= PRINTF_PRECISION;
            if (++field >= 2) {
                break;
            }
        }
        if (pState->m_Fields[PRINTF_WIDTH] < 0) {
            pState->m_Fields[PRINTF_WIDTH] = -pState->m_Fields[PRINTF_WIDTH];
            flags |= PRINTF_LEFT;
        }
        if ((flags & PRINTF_LEFT) != 0) {
            flags &= ~PRINTF_ZERO;
        }
        // the length
        if (c == 'l' || c == 'h') {
            int length = c;
            c = pState->m_GetChar(pState);
            if (c == length) {
                flags |= length == 'l' ? PRINTF_LONG_LONG : PRINTF_CHAR;
                c = pState->m_GetChar(pState);
            } else {
                flags |= length == 'l' ? PRINTF_LONG : PRINTF_SHORT;
            }
        } else if (c == 'L') {
            c = pState->m_GetChar(pState);
        } else if (c == 'j') {
            flags |= PRINTF_LONG_LONG;
            c = pState->m_GetChar(pState);
        } else if (c == 't' || c == 'z') {
            c = pState->m_GetChar(pState);
        }
        if (c == 0) {
            return pState->m_Count;
        }
        if (static_cast<u32>(c - 'A') <= 'Z' - 'A') {
            flags |= PRINTF_UPPER;
            c += 'a' - 'A';
        }
        pState->m_Flags = flags;
        int used = __printf_dispatch(pState, c, pArguments);
        if (used == 0) {
            PutChar(pState, c);                     // not a conversion
        } else if (used == 1) {
            pArguments += 4;
        } else {
            pArguments = Align8(pArguments) + 8;
        }
    }
}

// ---- the helpers of the formatters

// 0x002F7D40 (name is ours)
DECOMP_NOINLINE void __printf_pre_padding(PrintfState* pState)
{
    int count = pState->m_Fields[PRINTF_WIDTH];
    u32 flags = pState->m_Flags;
    int padding = (flags & PRINTF_ZERO) != 0 ? '0' : ' ';
    if ((flags & PRINTF_LEFT) != 0) {
        return;
    }
    while (--count >= 0) {
        PutChar(pState, padding);
    }
}

// 0x002F7D6C (name is ours)
DECOMP_NOINLINE void __printf_post_padding(PrintfState* pState)
{
    int count = pState->m_Fields[PRINTF_WIDTH];
    if ((pState->m_Flags & PRINTF_LEFT) == 0) {
        return;
    }
    while (--count >= 0) {
        PutChar(pState, ' ');
    }
}

// 0x002F7D8E (name is ours)
DECOMP_NOINLINE int __printf_truncate_signed(int value, PrintfState* pState)
{
    u32 flags = pState->m_Flags;
    if ((flags & PRINTF_CHAR) != 0) {
        return static_cast<s8>(value);
    }
    if ((flags & PRINTF_SHORT) != 0) {
        return static_cast<s16>(value);
    }
    return value;
}

// 0x002F7DA4 (name is ours)
DECOMP_NOINLINE u32 __printf_truncate_unsigned(u32 value, PrintfState* pState)
{
    u32 flags = pState->m_Flags;
    if ((flags & PRINTF_CHAR) != 0) {
        return static_cast<u8>(value);
    }
    if ((flags & PRINTF_SHORT) != 0) {
        return static_cast<u16>(value);
    }
    return value;
}

// the digits of the buffer (from the last one on) with the prefix, leading zeros and the padding
// 0x002F7DE8 (name is ours)
int __printf_int_common(PrintfState* pState, int digits, const char* pPrefix, int prefixLength)
{
    const char* pDigits = pState->m_Buffer;
    u32 flags = pState->m_Flags;
    int precision;
    if ((flags & PRINTF_PRECISION) != 0) {
        precision = pState->m_Fields[PRINTF_PRECISION_FIELD];
        pState->m_Flags = flags & ~PRINTF_ZERO;     // a precision turns the zero padding off
    } else {
        precision = 1;
    }
    int zeros = precision > digits ? precision - digits : 0;
    pState->m_Fields[PRINTF_WIDTH] -= zeros + digits + prefixLength;
    if ((pState->m_Flags & PRINTF_ZERO) == 0) {
        __printf_pre_padding(pState);
    }
    for (int i = 0; i < prefixLength; i++) {
        PutChar(pState, pPrefix[i]);
    }
    if ((pState->m_Flags & PRINTF_ZERO) != 0) {
        __printf_pre_padding(pState);
    }
    while (zeros-- > 0) {
        PutChar(pState, '0');
    }
    while (digits-- > 0) {
        PutChar(pState, pDigits[digits]);
    }
    __printf_post_padding(pState);
    return (pState->m_Flags & PRINTF_LONG_LONG) != 0 ? 2 : 1;
}

// ---- the integer conversions

// %d %i %u
// 0x0010100C (name is ours)
int __printf_int_dec(PrintfState* pState, int c, const u8* pArguments)
{
    const char* pPrefix = "";
    int prefixLength = 0;
    u32 value = *reinterpret_cast<const u32*>(pArguments);
    if (c == 'u') {
        value = __printf_truncate_unsigned(value, pState);
    } else {
        int signedValue = __printf_truncate_signed(value, pState);
        if (signedValue < 0) {
            signedValue = -signedValue;
            pPrefix = "-";
            prefixLength = 1;
        } else if ((pState->m_Flags & PRINTF_PLUS) != 0) {
            pPrefix = "+";
            prefixLength = 1;
        } else if ((pState->m_Flags & PRINTF_SPACE) != 0) {
            pPrefix = " ";
            prefixLength = 1;
        }
        value = signedValue;
    }
    int digits = 0;
    while (value != 0) {
        u64 result = _udiv10(value);
        pState->m_Buffer[digits++] = static_cast<u32>(result >> 32) + '0';
        value = static_cast<u32>(result);
    }
    return __printf_int_common(pState, digits, pPrefix, prefixLength);
}

// %lld %lli %llu (the conversion character has bit 7 set)
// 0x00101168 (name is ours)
int __printf_longlong_dec(PrintfState* pState, int c, const u8* pArguments)
{
    const char* pPrefix = "";
    int prefixLength = 0;
    u64 value = *reinterpret_cast<const u64*>(Align8(pArguments));
    if (c != ('u' | 0x80)) {
        if (static_cast<s64>(value) < 0) {
            value = -value;
            pPrefix = "-";
            prefixLength = 1;
        } else if ((pState->m_Flags & PRINTF_PLUS) != 0) {
            pPrefix = "+";
            prefixLength = 1;
        } else if ((pState->m_Flags & PRINTF_SPACE) != 0) {
            pPrefix = " ";
            prefixLength = 1;
        }
    }
    int digits = 0;
    while (value != 0) {
        pState->m_Buffer[digits++] = DivideBy10(&value) + '0';
    }
    return __printf_int_common(pState, digits, pPrefix, prefixLength);
}

// 0x001011E0 (name is ours)
DECOMP_NOINLINE int __printf_oct_common(PrintfState* pState, u64 value)
{
    int digits = 0;
    while (value != 0) {
        pState->m_Buffer[digits++] = (static_cast<u32>(value) & 7) + '0';
        value >>= 3;
    }
    u32 flags = pState->m_Flags;
    int prefixLength = 0;
    const char* pPrefix = "";
    if ((flags & PRINTF_ALTERNATE) != 0 && ((flags & PRINTF_PRECISION) | digits) != 0) {
        // %#o: a leading 0, which counts as a digit of the precision
        prefixLength = 1;
        pPrefix = "0";
        pState->m_Fields[PRINTF_PRECISION_FIELD]--;
    }
    return __printf_int_common(pState, digits, pPrefix, prefixLength);
}

// %o
// 0x00101224 (name is ours)
int __printf_int_oct(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_oct_common(pState, __printf_truncate_unsigned(*reinterpret_cast<const u32*>(pArguments), pState));
}

// %llo
// 0x0010123C (name is ours)
int __printf_longlong_oct(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_oct_common(pState, *reinterpret_cast<const u64*>(Align8(pArguments)));
}

// 0x00101258 (name is ours)
DECOMP_NOINLINE int __printf_hex_common(PrintfState* pState, int c, u64 value)
{
    const char* pDigits = (pState->m_Flags & PRINTF_UPPER) != 0 ? s_HexDigitsUpper : s_HexDigitsLower;
    int digits = 0;
    while (value != 0) {
        pState->m_Buffer[digits++] = pDigits[static_cast<u32>(value) & 15];
        value >>= 4;
    }
    const char* pPrefix = pDigits;
    int prefixLength = 0;
    if ((pState->m_Flags & PRINTF_ALTERNATE) != 0) {
        if (c == 'p') {
            pPrefix = pDigits + 16;                 // "@"
            prefixLength = 1;
        } else if (digits != 0) {
            pPrefix = pDigits + 17;                 // "0x"
            prefixLength = 2;
        }
    }
    return __printf_int_common(pState, digits, pPrefix, prefixLength);
}

// %x
// 0x001012B0 (name is ours)
int __printf_int_hex(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_hex_common(pState, c,
                               __printf_truncate_unsigned(*reinterpret_cast<const u32*>(pArguments), pState));
}

// %llx
// 0x001012CC (name is ours)
int __printf_longlong_hex(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_hex_common(pState, c, *reinterpret_cast<const u64*>(Align8(pArguments)));
}

// %p: 8 hexadecimal digits
// 0x001012D6 (name is ours)
int __printf_pointer(PrintfState* pState, int c, const u8* pArguments)
{
    pState->m_Flags |= PRINTF_PRECISION;
    pState->m_Fields[PRINTF_PRECISION_FIELD] = 8;
    return __printf_hex_common(pState, c, *reinterpret_cast<const u32*>(pArguments));
}

// %n
// 0x00101A68 (name is ours)
int __printf_count(PrintfState* pState, int c, const u8* pArguments)
{
    void* pTarget = *reinterpret_cast<void* const*>(pArguments);
    u32 flags = pState->m_Flags;
    int count = pState->m_Count;
    if ((flags & PRINTF_CHAR) != 0) {
        *static_cast<s8*>(pTarget) = count;
    } else if ((flags & PRINTF_SHORT) != 0) {
        *static_cast<s16*>(pTarget) = count;
    } else if ((flags & PRINTF_LONG_LONG) != 0) {
        *static_cast<s64*>(pTarget) = count;
    } else {
        *static_cast<int*>(pTarget) = count;
    }
    return 1;
}

// ---- the strings

// a narrow string into a narrow output: length 1 (%c) or at most the precision (-1: up to the terminator)
// 0x00100FB0 (name is ours)
DECOMP_NOINLINE void __printf_out_narrow(PrintfState* pState, const char* pString, u32 length)
{
    u32 count;
    if (length == 1) {
        count = 1;
    } else {
        if ((pState->m_Flags & PRINTF_PRECISION) != 0) {
            length = pState->m_Fields[PRINTF_PRECISION_FIELD];
        }
        for (count = 0; count < length && pString[count] != 0; count++) {
        }
    }
    const char* pEnd = pString + count;
    pState->m_Fields[PRINTF_WIDTH] -= count;
    pState->m_Count += count;
    __printf_pre_padding(pState);
    while (pString < pEnd) {
        pState->m_PutChar(*pString++, pState->m_pPutArgument);
    }
    __printf_post_padding(pState);
}

// a wide string into a wide output
// 0x00101108 (name is ours)
DECOMP_NOINLINE void __printf_out_wide(PrintfState* pState, const wchar_t* pString, u32 length)
{
    u32 count;
    if (length == 1) {
        count = 1;
    } else {
        if ((pState->m_Flags & PRINTF_PRECISION) != 0) {
            length = pState->m_Fields[PRINTF_PRECISION_FIELD];
        }
        for (count = 0; count < length && pString[count] != 0; count++) {
        }
    }
    pState->m_Fields[PRINTF_WIDTH] -= count;
    pState->m_Count += count;
    const wchar_t* pEnd = pString + count;
    __printf_pre_padding(pState);
    while (pString < pEnd) {
        pState->m_PutChar(*pString++, pState->m_pPutArgument);
    }
    __printf_post_padding(pState);
}

// a narrow (multibyte) string into a wide output (swprintf %s)
// 0x00101078 (name is ours)
DECOMP_NOINLINE void __printf_out_narrow_wide(PrintfState* pState, const char* pString, int length)
{
    mbstate_t state = {};
    int characters = 0;
    int i = 0;
    while (((pState->m_Flags & PRINTF_PRECISION) == 0 || pState->m_Fields[PRINTF_PRECISION_FIELD] > characters) &&
           (i < length || pString[i] != 0)) {
        wchar_t character;
        if (__mbrtowc_byte(&character, pString[i], &state) == 1) {
            characters++;
        }
        i++;
    }
    pState->m_Fields[PRINTF_WIDTH] -= characters;
    __printf_pre_padding(pState);
    state = mbstate_t();
    for (int j = 0; j < i; j++) {
        wchar_t character;
        if (__mbrtowc_byte(&character, pString[j], &state) == 1) {
            PutChar(pState, character);
        }
    }
    __printf_post_padding(pState);
}

// a wide string into a narrow output (printf %ls): the precision counts bytes
// 0x00101AD8 (name is ours)
DECOMP_NOINLINE void __printf_out_wide_narrow(PrintfState* pState, const wchar_t* pString, int length)
{
    mbstate_t state = {};
    char bytes[8];
    u32 count = 0;
    int i = 0;
    while (((pState->m_Flags & PRINTF_PRECISION) == 0 || pState->m_Fields[PRINTF_PRECISION_FIELD] > static_cast<int>(count)) &&
           (i < length || pString[i] != 0)) {
        int converted = __wcrtomb_char(bytes, pString[i], &state);
        if (converted != -1) {
            if ((pState->m_Flags & PRINTF_PRECISION) != 0 &&
                count + converted > static_cast<u32>(pState->m_Fields[PRINTF_PRECISION_FIELD])) {
                break;
            }
            count += converted;
        }
        i++;
    }
    pState->m_Fields[PRINTF_WIDTH] -= count;
    __printf_pre_padding(pState);
    state = mbstate_t();
    for (int j = 0; j < i; j++) {
        int converted = __wcrtomb_char(bytes, pString[j], &state);
        if (converted != -1) {
            for (u32 k = 0; k < static_cast<u32>(converted); k++) {
                pState->m_PutChar(bytes[k], pState->m_pPutArgument);
            }
        }
    }
    pState->m_Count += count;
    __printf_post_padding(pState);
}

// 0x00101AA8 (name is ours)
DECOMP_NOINLINE int __printf_string_common(PrintfState* pState, const char* pString, int length)
{
    if (pState->m_IsWide != 0) {
        __printf_out_narrow_wide(pState, pString, length);
    } else {
        __printf_out_narrow(pState, pString, length);
    }
    return 1;
}

// %c
// 0x00101ABE (name is ours)
int __printf_char(PrintfState* pState, int c, const u8* pArguments)
{
    char* pBuffer = pState->m_Buffer;
    pBuffer[0] = *reinterpret_cast<const int*>(pArguments);
    pBuffer[1] = 0;
    return __printf_string_common(pState, pBuffer, 1);
}

// %s
// 0x00101ACE (name is ours)
int __printf_string(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_string_common(pState, *reinterpret_cast<const char* const*>(pArguments), -1);
}

// 0x00101B9C (name is ours)
DECOMP_NOINLINE int __printf_wstring_common(PrintfState* pState, const wchar_t* pString, int length)
{
    if (pState->m_IsWide != 0) {
        __printf_out_wide(pState, pString, length);
    } else {
        __printf_out_wide_narrow(pState, pString, length);
    }
    return 1;
}

// %lc
// 0x00101BB2 (name is ours)
int __printf_wchar(PrintfState* pState, int c, const u8* pArguments)
{
    wchar_t* pBuffer = reinterpret_cast<wchar_t*>(pState->m_Buffer);
    pBuffer[0] = *reinterpret_cast<const int*>(pArguments);
    pBuffer[1] = 0;
    return __printf_wstring_common(pState, pBuffer, 1);
}

// %ls
// 0x00101BC2 (name is ours)
int __printf_wstring(PrintfState* pState, int c, const u8* pArguments)
{
    return __printf_wstring_common(pState, *reinterpret_cast<const wchar_t* const*>(pArguments), -1);
}

// ---- the C library functions

// 0x00100DF8 | manual:ours [tier A]
// 0x00100F20 (copy)
int snprintf(char* pDestination, size_t size, const char* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    LimitedBuffer buffer;
    buffer.m_pCurrent = pDestination;
    if (size != 0) {
        pDestination = pDestination + size - 1;
    }
    buffer.m_pEnd = pDestination;
    int count = __printf_narrow(pFormat, &buffer, Arguments(arguments), __printf_putc_limited);
    if (size != 0) {
        __printf_putc_buffer(0, &buffer);
    }
    va_end(arguments);
    return count;
}

// 0x00100E34 | manual:ours [tier A]
int vsprintf(char* pDestination, const char* pFormat, va_list arguments)
{
    char* pCurrent = pDestination;
    int count = __printf_narrow(pFormat, &pCurrent, Arguments(arguments), __printf_putc_buffer);
    __printf_putc_buffer(0, &pCurrent);
    return count;
}

// 0x00100E58 | manual:ours [tier A]
// 0x00101A28 (copy)
int vsnprintf(char* pDestination, size_t size, const char* pFormat, va_list arguments)
{
    LimitedBuffer buffer;
    buffer.m_pCurrent = pDestination;
    if (size != 0) {
        pDestination = pDestination + size - 1;
    }
    buffer.m_pEnd = pDestination;
    int count = __printf_narrow(pFormat, &buffer, Arguments(arguments), __printf_putc_limited);
    if (size != 0) {
        __printf_putc_buffer(0, &buffer);
    }
    return count;
}

// -1 if the result does not fit
// 0x00100E90 | libgarden [tier A]
int vswprintf(wchar_t* pDestination, size_t size, const wchar_t* pFormat, va_list arguments)
{
    LimitedWideBuffer buffer;
    buffer.m_pCurrent = pDestination;
    if (size != 0) {
        pDestination = pDestination + size - 1;
    }
    buffer.m_pEnd = pDestination;
    u32 count = __printf_wide(pFormat, &buffer, Arguments(arguments), __printf_putwc_limited);
    if (size != 0) {
        buffer.m_pEnd++;                            // room for the terminator
        __printf_putwc_limited(0, &buffer);
    }
    if (count >= size) {
        return -1;
    }
    return count;
}

// the format first, then the stream (the order of the C library's internal vfprintf)
// 0x002F7B7C (name is ours)
DECOMP_NOINLINE int __vfprintf(const char* pFormat, File* pStream, va_list arguments)
{
    int count = __printf_narrow(pFormat, pStream, Arguments(arguments), reinterpret_cast<PrintfPutChar>(fputc));
    if (ferror(pStream) != 0) {
        return -1;
    }
    return count;
}

// 0x00100ED8 (name is ours, after the C standard)
int printf(const char* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    return __vfprintf(pFormat, &__stdout, arguments);
}

// 0x00307A0C (name is ours, after the C standard)
int fprintf(File* pStream, const char* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    return __vfprintf(pFormat, pStream, arguments);
}

// 0x00100EF4 | manual:ours [tier A]
int sprintf(char* pDestination, const char* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    char* pCurrent = pDestination;
    int count = __printf_narrow(pFormat, &pCurrent, Arguments(arguments), __printf_putc_buffer);
    __printf_putc_buffer(0, &pCurrent);
    va_end(arguments);
    return count;
}

// 0x00100F64 | manual:ours [tier A]
int swprintf(wchar_t* pDestination, size_t size, const wchar_t* pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);
    LimitedWideBuffer buffer;
    buffer.m_pCurrent = pDestination;
    if (size != 0) {
        pDestination = pDestination + size - 1;
    }
    buffer.m_pEnd = pDestination;
    u32 count = __printf_wide(pFormat, &buffer, Arguments(arguments), __printf_putwc_limited);
    if (size != 0) {
        buffer.m_pEnd++;                            // room for the terminator
        __printf_putwc_limited(0, &buffer);
    }
    va_end(arguments);
    if (count >= size) {
        return -1;
    }
    return count;
}

} // extern "C"
