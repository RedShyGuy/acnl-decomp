// The double helpers of the math library of the ARM C library: frexp with the base procedure call standard
// (doubles in r0/r1), the narrowing of a double to a float and ldexp, both through pointers (for strtod and
// the printf family). The structure of frexp is fdlibm's.

#include "decomp.h"
#include "types.h"
#include "clib/clib_Btod.h"

namespace {
// the values of errno (names are ours)
const int ERROR_RANGE = 2;

union DoubleWords {
    f64 value;
    u32 words[2];                                   // low, high
};

inline u32 GetHighWord(f64 value)
{
    DoubleWords bits;
    bits.value = value;
    return bits.words[1];
}

inline f64 SetHighWord(f64 value, u32 high)
{
    DoubleWords bits;
    bits.value = value;
    bits.words[1] = high;
    return bits.value;
}
} // namespace

namespace clib {
// +infinity (HUGE_VAL), read by the Thumb code of scanf
// 0x00945410 (name is ours)
extern const DoubleWords s_Infinity = {0, 0x7FF00000};
} // namespace clib

extern "C" {

void __set_errno(int value);
f64 __mathlib_dbl_underflow();
__attribute__((pcs("aapcs"))) f64 __ARM_scalbn(f64 value, int exponent);

// the floating point status (rounding mode in bits 22-23): without a floating point environment always 0 (round to
// nearest); the printf conversion asks it with (0, 0)
// 0x002F89EC (name is ours)
u32 __fp_status(u32 mask, u32 flags)
{
    return 0;
}

// 0x007A7408 (name is ours)
DECOMP_NOINLINE __attribute__((pcs("aapcs"))) f64 __softfp_frexp(f64 x, int* exponent)
{
    DoubleWords bits;
    bits.value = x;
    s32 high = bits.words[1];
    s32 magnitude = high & 0x7FFFFFFF;
    *exponent = 0;
    if (magnitude >= 0x7FF00000 || (bits.words[0] | magnitude) == 0) {
        return x;                                   // infinite, NaN or 0
    }
    if (magnitude < 0x100000) {
        // a denormal: scaled by 2^54
        x *= 0x1p54;
        high = GetHighWord(x);
        magnitude = high & 0x7FFFFFFF;
        *exponent = -54;
    }
    *exponent += (magnitude >> 20) - 1022;
    return SetHighWord(x, (high & 0x800FFFFF) | 0x3FE00000);
}

// *result = the float nearest to *value; ERANGE when it overflows or underflows to 0
// 0x002F96F8 | fefates:bytes [tier B]
void __mathlib_narrow(f32* result, const f64* value)
{
    f64 x = *value;
    int exponent;
    __softfp_frexp(x, &exponent);
    exponent += 126;
    f32 narrow;
    if (exponent <= 0 && x != 0.0) {
        // (armlink replaced the calls that save and restore the floating point status around the conversion)
        narrow = static_cast<f32>(x);
        if (narrow == 0.0f) {
            __set_errno(ERROR_RANGE);
        }
    } else if (exponent < 255) {
        narrow = static_cast<f32>(x);
    } else {
        __set_errno(ERROR_RANGE);
        narrow = x >= 0.0 ? __builtin_inff() : -__builtin_inff();
    }
    *result = narrow;
}

// *value *= 2^exponent; an overflow gives a signed infinity, an underflow to 0 a signed 0 (both ERANGE)
// 0x007A0B78 | fefates:bytes [tier B]
void __support_ldexp(f64* value, int exponent)
{
    f64 x = *value;
    if ((~(GetHighWord(x) >> 20) & 0x7FF) != 0 && x != 0.0) {
        x = __ARM_scalbn(x, exponent);
        if ((GetHighWord(x) >> 20) << 21 == 0xFFE00000) {
            __set_errno(ERROR_RANGE);
            f64 huge = 0x1p769;
            DoubleWords bits;
            bits.value = huge * huge;
            bits.words[1] = (bits.words[1] & 0x7FFFFFFF) | (GetHighWord(x) & 0x80000000);
            x = bits.value;
        } else if (x == 0.0) {
            __set_errno(ERROR_RANGE);
            DoubleWords bits;
            bits.value = __mathlib_dbl_underflow();
            bits.words[1] = (bits.words[1] & 0x7FFFFFFF) | (GetHighWord(x) & 0x80000000);
            x = bits.value;
        }
    }
    *value = x;
}

} // extern "C"
