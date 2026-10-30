#pragma once

// The scanf of the ARM C library (Thumb code): a wrapper fills a state with the input source (a string for sscanf),
// the engine reads the format and hands each conversion to a handler. The handlers return the characters they
// consumed, -1 at the end of the input and -2 on a mismatch. All names are ours.

#include "types.h"

namespace clib {
// the flags of a conversion
const u32 SCANF_SUPPRESS = 0x1;         // '*'
const u32 SCANF_LONG_LONG = 0x2;        // ll, j
const u32 SCANF_LONG = 0x4;             // l
const u32 SCANF_SHORT = 0x8;            // h
const u32 SCANF_WIDTH = 0x10;           // a width is given
const u32 SCANF_LONG_DOUBLE = 0x20;     // L
const u32 SCANF_SIGNED = 0x40;          // a signed conversion (d i o u x)
const u32 SCANF_POINT = 0x80;           // the decimal point was read (floating point conversions)
const u32 SCANF_EXPONENT_NEGATIVE = 0x100;  // a '-' after the 'e'
const u32 SCANF_DIGITS = 0x200;         // digits were read
const u32 SCANF_NEGATIVE = 0x400;       // a '-' was read
const u32 SCANF_CHAR = 0x800;           // hh

const int SCAN_END = -1;            // the end of the input
const int SCAN_MISMATCH = -2;       // the input does not match
const int SCAN_NOT_HANDLED = -3;    // passed to the special floating point handlers (they never return it)

struct ScanState
{
    const u8* m_pArguments;                                   // 0x0
    u32 m_Flags;                                              // 0x4
    int m_Width;                                              // 0x8
    const void* m_pFormat;                                    // 0xC
    int m_IsWide;                                             // 0x10
    int (*m_GetFormatChar)(const void** ppFormat, int step);  // 0x14 (reads one character and moves on by step)
    int (*m_GetChar)(void* pSource);                          // 0x18
    int (*m_UngetChar)(void* pSource);                        // 0x1C
    int (*m_IsSpace)(int c);                                  // 0x20
};

// the address of the next argument (a pointer to the result)
inline void* NextTarget(ScanState* pState)
{
    void* pTarget = *reinterpret_cast<void* const*>(pState->m_pArguments);
    pState->m_pArguments += 4;
    return pTarget;
}

// the input of sscanf
struct StringSource
{
    const char* m_pCurrent;     // 0x0
    int m_Remaining;            // 0x4 (-1: unlimited)
    const char* m_pStart;       // 0x8
    int m_IsEnd;                // 0xC
};
} // namespace clib
