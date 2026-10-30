#pragma once

// The extended precision numbers of the decimal conversions of the C library (printf and scanf, "btod"): a word with
// the sign and the exponent (bias 0x3FFF) and a 64 bit mantissa with its leading one. All names are ours.

#include "types.h"

namespace clib {
struct Extended
{
    u32 m_SignExponent;
    u32 m_MantissaHigh;
    u32 m_MantissaLow;
};

// a double as two words (the Thumb code of the library has no VFP)
struct DoubleWords
{
    u32 m_Low;
    u32 m_High;
};

extern const DoubleWords s_Infinity;
} // namespace clib

extern "C" {
// result = 10^exponent (rounding: 0 to nearest, 1 up, -1 down)
void _btod_etento(clib::Extended* pResult, int exponent, int rounding);
}
