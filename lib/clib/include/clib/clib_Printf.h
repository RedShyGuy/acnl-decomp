#pragma once

// The printf of the ARM C library (Thumb code): the wrappers (sprintf, snprintf, ...) fill a state with an output
// function and a function that reads the format, the engine parses the format and hands each conversion to a
// dispatcher (built by armlink from the conversions linked in), which calls the formatter of the conversion.
// All names are ours.

#include "types.h"

namespace clib {
struct PrintfState;

typedef void (*PrintfPutChar)(int c, void* pArgument);
typedef int (*PrintfGetChar)(PrintfState* pState);

// the flags of a conversion
const u32 PRINTF_LEFT = 0x1;            // '-'
const u32 PRINTF_PLUS = 0x2;            // '+'
const u32 PRINTF_SPACE = 0x4;           // ' '
const u32 PRINTF_ALTERNATE = 0x8;       // '#'
const u32 PRINTF_ZERO = 0x10;           // '0'
const u32 PRINTF_PRECISION = 0x20;      // a precision is given
const u32 PRINTF_LONG = 0x40;           // l
const u32 PRINTF_LONG_LONG = 0x80;      // ll, j
const u32 PRINTF_SHORT = 0x100;         // h
const u32 PRINTF_CHAR = 0x400;          // hh
const u32 PRINTF_UPPER = 0x800;         // an upper case conversion (X, E, G, ...)

// the fields of a conversion
const int PRINTF_WIDTH = 0;
const int PRINTF_PRECISION_FIELD = 1;

struct PrintfState
{
    u32 m_Flags;                 // 0x0
    PrintfPutChar m_PutChar;     // 0x4
    void* m_pPutArgument;        // 0x8
    PrintfGetChar m_GetChar;     // 0xC
    const void* m_pFormat;       // 0x10
    int m_IsWide;                // 0x14 (swprintf: the strings are wide)
    int m_Fields[2];             // 0x18 (width, precision)
    int m_Count;                 // 0x20 (characters written)
    char m_Buffer[24];           // 0x24 (the digits, from the last one on)
};
} // namespace clib

extern "C" {
// the result of a formatter: 0 not a conversion (the character is written), 1 one word of the arguments used,
// 2 two words (aligned to 8)
int __printf_engine(clib::PrintfState* pState, const u8* pArguments);
int __printf_dispatch(clib::PrintfState* pState, int c, const u8* pArguments);
int __printf_narrow(const char* pFormat, void* pPutArgument, const u8* pArguments, clib::PrintfPutChar putChar);
int __printf_wide(const wchar_t* pFormat, void* pPutArgument, const u8* pArguments, clib::PrintfPutChar putChar);
void __printf_pre_padding(clib::PrintfState* pState);
void __printf_post_padding(clib::PrintfState* pState);
int __printf_int_common(clib::PrintfState* pState, int digits, const char* pPrefix, int prefixLength);
}
