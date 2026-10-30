// The classification of floating point numbers in the ARM C library (fpclassify): the library uses the base
// procedure call standard, so a double arrives in r0/r1 (pcs("aapcs")).

#include "decomp.h"
#include "types.h"

namespace {
// the results of __ARM_fpclassify (names are ours): bit 2 = mantissa not 0, bit 0 = exponent not 0, bit 1 =
// exponent all ones
const int CLASS_ZERO = 0;
const int CLASS_NORMAL_MANTISSA_ZERO = 1;
const int CLASS_NORMAL = 5;
} // namespace

// 0x002F8CE0 | nintendogs:bytes [tier A]
extern "C" __attribute__((pcs("aapcs"))) int __ARM_fpclassify(f64 value)
{
    union {
        f64 value;
        u32 words[2];
    } bits;
    bits.value = value;
    u32 high = bits.words[1] << 1;
    int result = (bits.words[0] | high << 11) != 0 ? 4 : CLASS_ZERO;
    if (high >> 21 != 0) {
        result |= 1;
    }
    if (high >> 21 == 0x7FF) {
        result |= 2;
    }
    if (result == CLASS_NORMAL_MANTISSA_ZERO) {
        result = CLASS_NORMAL;
    }
    return result;
}

// 0x0077A6F4 | nintendogs:bytes [tier A]
extern "C" __attribute__((pcs("aapcs"))) int __ARM_fpclassifyf(f32 value)
{
    union {
        f32 value;
        u32 word;
    } bits;
    bits.value = value;
    u32 high = bits.word << 1;
    int result = high << 8 != 0 ? 4 : CLASS_ZERO;
    if (high >> 24 != 0) {
        result |= 1;
    }
    if ((~high & 0xFF000000) == 0) {
        result |= 2;
    }
    if (result == CLASS_NORMAL_MANTISSA_ZERO) {
        result = CLASS_NORMAL;
    }
    return result;
}
