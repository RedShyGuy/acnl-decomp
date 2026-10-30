// The float functions of the math library of the ARM C library (__hardfp_<name>: the variants that take and
// return floats in VFP registers). They follow the structure of fdlibm: the float is read as its bit pattern,
// special cases are found from the exponent; "HUGE + x > 0" raises the inexact flag.

#include "decomp.h"
#include "types.h"

namespace {
const f32 HUGE = 1.0e30f;

inline s32 GetFloatWord(f32 value)
{
    union {
        f32 value;
        s32 word;
    } bits;
    bits.value = value;
    return bits.word;
}

inline f32 SetFloatWord(s32 word)
{
    union {
        f32 value;
        s32 word;
    } bits;
    bits.word = word;
    return bits.value;
}

// the values of errno (names are ours)
const int ERROR_DOMAIN = 1;
const int ERROR_RANGE = 2;

// log10(1 + i / 8) for i = 0..7 in two parts (high: 12 bits of mantissa)
// 0x008C94A8
extern const f32 s_Log10TableHigh[8] = {
    0.0f, 0x1.a3p-5f, 0x1.8cep-4f, 0x1.1b2p-3f, 0x1.68ap-3f, 0x1.afcp-3f, 0x1.f1ap-3f, 0x1.178p-2f,
};
// 0x008C94C8
extern const f32 s_Log10TableLow[8] = {
    0.0f, 0x1.53ac14p-18f, 0x1.183886p-16f, 0x1.e71ecap-15f, 0x1.445b06p-18f, 0x1.3e3a24p-15f, 0x1.deeb66p-15f,
    0x1.b4a7a4p-15f,
};
// 2^(i / 4) for i = 0..3: high part (12 bits of mantissa), low part, whole value
// 0x008C9478
extern const f32 s_Exp2QuarterHigh[4] = {0x1.0p+0f, 0x1.306p+0f, 0x1.6a0p+0f, 0x1.ae8p+0f};
// 0x008C9488
extern const f32 s_Exp2QuarterLow[4] = {0.0f, 0x1.fc1464p-13f, 0x1.3cccfep-13f, 0x1.3f32b6p-13f};
// 0x008C9498
extern const f32 s_Exp2Quarter[4] = {0x1.0p+0f, 0x1.306fe0p+0f, 0x1.6a09e6p+0f, 0x1.ae89fap+0f};

// ln(1 + i / 8) for i = 0..7 in two parts (high: 12 bits of mantissa)
// 0x008C94E8
extern const f32 s_LogTableHigh[8] = {
    0.0f, 0x1.e26p-4f, 0x1.c8ep-3f, 0x1.46p-2f, 0x1.9f2p-2f, 0x1.f12p-2f, 0x1.1e8p-1f, 0x1.41cp-1f,
};
// 0x008C9508
extern const f32 s_LogTableLow[8] = {
    0.0f, 0x1.076e2ap-16f, 0x1.f7c79ap-15f, 0x1.8bc21cp-14f, 0x1.23ecc0p-14f, 0x1.1ebf5ep-15f, 0x1.7d79c2p-15f,
    0x1.8fe846p-13f,
};
// 16 * log2(1 + i / 16) for i = 0..15 in two parts (powf)
struct Log2Entry
{
    f32 high;
    f32 low;
};
// 0x008C9528
extern const Log2Entry s_Log2Sixteenths[16] = {
    {0.0f, 0.0f},
    {0x1.66p+0f, 0x1.fb7d64p-11f},
    {0x1.5cp+1f, 0x1.a39fbep-15f},
    {0x1.fcp+1f, -0x1.f4a37ep-10f},
    {0x1.49cp+2f, -0x1.87b432p-10f},
    {0x1.91cp+2f, -0x1.15db84p-12f},
    {0x1.d68p+2f, -0x1.583f9ap-11f},
    {0x1.0c2p+3f, -0x1.f5fe54p-10f},
    {0x1.2b8p+3f, 0x1.a39fbep-16f},
    {0x1.49ap+3f, 0x1.e12f34p-11f},
    {0x1.66ap+3f, 0x1.1c8f12p-18f},
    {0x1.828p+3f, 0x1.3ab7cep-14f},
    {0x1.9d6p+3f, -0x1.30158p-12f},
    {0x1.b74p+3f, 0x1.291eaap-10f},
    {0x1.d06p+3f, -0x1.8125b4p-10f},
    {0x1.e88p+3f, 0x1.8d66c4p-10f},
};
// 2^(i / 16) for i = 0..15: high part (12 bits of mantissa), low part, whole value (powf)
// 0x008C95A8
extern const f32 s_Exp2SixteenthHigh[16] = {
    0x1.0p+0f, 0x1.0b4p+0f, 0x1.172p+0f, 0x1.238p+0f, 0x1.306p+0f, 0x1.3dep+0f, 0x1.4bep+0f, 0x1.5aap+0f,
    0x1.6ap+0f, 0x1.7ap+0f, 0x1.8acp+0f, 0x1.9c4p+0f, 0x1.ae8p+0f, 0x1.c18p+0f, 0x1.d58p+0f, 0x1.ea4p+0f,
};
// 0x008C95E8
extern const f32 s_Exp2SixteenthLow[16] = {
    0.0f, 0x1.586cfap-12f, 0x1.7078fap-13f, 0x1.e9b9d6p-14f, 0x1.fc1464p-13f, 0x1.4c9824p-13f, 0x1.dad536p-12f,
    0x1.07dd48p-12f, 0x1.3cccfep-13f, 0x1.1473ecp-12f, 0x1.ca8456p-13f, 0x1.230548p-13f, 0x1.3f32b6p-13f,
    0x1.9bdd86p-12f, 0x1.8dcfbap-16f, 0x1.5f454ap-13f,
};
// 0x008C9628
extern const f32 s_Exp2Sixteenth[16] = {
    0x1.0p+0f, 0x1.0b5586p+0f, 0x1.172b84p+0f, 0x1.2387a6p+0f, 0x1.306fe0p+0f, 0x1.3dea64p+0f, 0x1.4bfdaep+0f,
    0x1.5ab07ep+0f, 0x1.6a09e6p+0f, 0x1.7a1148p+0f, 0x1.8ace54p+0f, 0x1.9c4918p+0f, 0x1.ae89fap+0f, 0x1.c199bep+0f,
    0x1.d5818ep+0f, 0x1.ea4afap+0f,
};

// value rounded to 12 bits of mantissa (the high part of a number split in two)
inline f32 RoundTo12Bits(f32 value)
{
    return SetFloatWord(static_cast<s32>((static_cast<u32>(GetFloatWord(value)) + 0x800) >> 12 << 12));
}

// the bits of 2 / pi, from the binary point on (one zero word in front)
// 0x008C9870
extern const u32 s_TwoOverPiBits[8] = {
    0x00000000, 0xA2F9836E, 0x4E441529, 0xFC2757D1, 0xF534DDC0, 0xDB629599, 0x3C439041, 0xFE5163AB,
};
} // namespace

extern "C" {

void __set_errno(int value);
f32 __mathlib_rredf2(f32 x, int* quadrant, u32 word);
__attribute__((pcs("aapcs"))) f32 __softfp_sqrtf(f32 x);
__attribute__((pcs("aapcs"))) u32 _ffixu(f32 value);
f32 __mathlib_flt_underflow();
__attribute__((pcs("aapcs"))) int __ARM_fpclassifyf(f32 value);
// the IEEE remainder of fplib (float arguments in integer registers)
__attribute__((pcs("aapcs"))) f32 _frem(f32 x, f32 y);

// 0x0077AD5C | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_acosf(f32 x)
{
    u32 word = GetFloatWord(x);
    f32 zero = 0.0f;
    if ((word << 1) + 0x9A000000 > 0x19000000) {
        // |x| outside [2^-25, 1]
        if (word << 1 < 0x66000000) {
            return 0x1.921fb6p+0f;                  // pi / 2
        }
        if (word << 1 > 0xFF000000) {
            return x + x;                           // NaN
        }
        __set_errno(ERROR_DOMAIN);
        return zero / zero;
    }
    // acos(x) = high + low - (t + t * z * P(z)): pi/2 - asin(x) for |x| <= 0.5, else through
    // asin(sqrt((1 - |x|) / 2))
    f32 z;
    f32 t;
    f32 high;
    f32 low;
    if (word << 1 > 0x7E000000) {
        z = (1.0f - __builtin_fabsf(x)) * 0.5f;
        f32 root = __softfp_sqrtf(z);
        if ((word & 0x80000000) == 0) {
            high = zero;
            low = zero;
        } else {
            root = -root;
            high = 0x1.92p+1f;
            low = 0x1.fb5444p-11f;
        }
        t = root * -2.0f;
    } else {
        z = x * x;
        high = 0x1.92p+0f;
        low = 0x1.fb5444p-12f;
        t = x;
    }
    f32 result = (low - t) - t * z * (0x1.55555ep-3f + z * (0x1.332636p-4f + z * (0x1.70ce3cp-5f + z * (0x1.b1158ep-6f +
                                                                                                    z * 0x1.39fc48p-5f))));
    return result + high;
}

// 0x0077AE88 | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_asinf(f32 x)
{
    u32 word = GetFloatWord(x);
    f32 zero = 0.0f;
    if ((word << 1) + 0x8E000000 > 0x0D000000) {
        // |x| outside [2^-13, 1]
        if (word << 1 < 0x72000000) {
            if (__ARM_fpclassifyf(x) == 4) {
                __mathlib_flt_underflow();          // a denormal x: the underflow flag
            }
            return x;
        }
        if (word << 1 > 0xFF000000) {
            return x + x;                           // NaN
        }
        __set_errno(ERROR_DOMAIN);
        return zero / zero;
    }
    // asin(x) = high + low + (t + t * z * P(z)); for |x| > 0.5 through asin(sqrt((1 - |x|) / 2))
    f32 z;
    f32 t;
    f32 high;
    f32 low;
    if (word << 1 <= 0x7E000000) {
        low = zero;
        high = zero;
        z = x * x;
        t = x;
    } else {
        z = (1.0f - __builtin_fabsf(x)) * 0.5f;
        f32 root = __softfp_sqrtf(z);
        if ((word & 0x80000000) != 0) {
            high = -0x1.92p+0f;
            low = -0x1.fb5444p-12f;
            root = -root;
        } else {
            high = 0x1.92p+0f;
            low = 0x1.fb5444p-12f;
        }
        t = root * -2.0f;
    }
    f32 result = (low + t) + t * z * (0x1.55555ep-3f + z * (0x1.332636p-4f + z * (0x1.70ce3cp-5f + z * (0x1.b1158ep-6f +
                                                                                                   z * 0x1.39fc48p-5f))));
    return result + high;
}

// 0x0077AFC0 | nintendogs:bytes [tier A]
f32 __hardfp_atan2f(f32 y, f32 x)
{
    u32 yWord = GetFloatWord(y);
    u32 xWord = GetFloatWord(x);
    if ((yWord << 1) + 0xE8000000 >= 0xE5000000 || (xWord << 1) + 0xE8000000 >= 0xE5000000) {
        // a zero, denormal, very large, infinite or NaN argument
        if (yWord << 1 > 0xFF000000 || xWord << 1 > 0xFF000000) {
            return y + x;                           // NaN
        }
        if (((yWord | xWord) << 1) == 0) {
            xWord |= 0x7F800000;                    // atan2(+-0, +-0) as atan2(+-0, +-inf)
            x = SetFloatWord(xWord);
        } else if (yWord << 1 == 0xFF000000 && xWord << 1 == 0xFF000000) {
            yWord &= ~0x40000000;                   // both infinite: as +-1
            y = SetFloatWord(yWord);
            xWord &= ~0x40000000;
            x = SetFloatWord(xWord);
        } else if (yWord << 1 == 0xFF000000 || xWord << 1 == 0) {
            yWord |= 0x7F800000;                    // the result is +-pi/2: only the words take part
            xWord &= 0x80000000;
        } else if ((((xWord << 1) ^ (yWord << 1)) & 0x80000000) == 0) {
            // both small or both large: scaled into the normal range
            f32 scale = static_cast<s32>(xWord << 1) >= 0 ? 0x1p32f : 0x1p-32f;
            y *= scale;
            x *= scale;
            yWord = GetFloatWord(y);
            xWord = GetFloatWord(x);
        }
    }
    s32 difference = static_cast<s32>((yWord << 1) >> 24) - static_cast<s32>((xWord << 1) >> 24);
    if (difference > 27) {
        return (yWord & 0x80000000) != 0 ? -0x1.921fb6p+0f : 0x1.921fb6p+0f;
    }
    if (difference < -26) {
        if ((xWord & 0x80000000) != 0) {
            return (yWord & 0x80000000) != 0 ? -0x1.921fb6p+1f : 0x1.921fb6p+1f;
        }
        f32 quotient = y / x;
        if (__ARM_fpclassifyf(quotient) == 4) {
            __mathlib_flt_underflow();              // a denormal result: the underflow flag
        }
        return quotient;
    }
    // atan2 = high + low + atan(numerator / denominator) with |numerator| <= |denominator|
    f32 numerator;
    f32 denominator;
    u32 numeratorWord;
    u32 denominatorWord;
    f32 high;
    f32 low;
    if (yWord << 1 > xWord << 1) {
        // |y| > |x|: +-pi/2 + atan(x / -y)
        numerator = x;
        denominator = -y;
        if ((yWord & 0x80000000) != 0) {
            high = -0x1.92p+0f;
            low = -0x1.fb5444p-12f;
        } else {
            high = 0x1.92p+0f;
            low = 0x1.fb5444p-12f;
        }
        denominatorWord = yWord ^ 0x80000000;
        numeratorWord = xWord;
    } else {
        numerator = y;
        denominator = x;
        if ((xWord & 0x80000000) == 0) {
            low = 0.0f;
            high = low;
        } else if ((yWord & 0x80000000) != 0) {
            high = -0x1.92p+1f;                     // x < 0: +-pi + atan(y / x)
            low = -0x1.fb5444p-11f;
        } else {
            high = 0x1.92p+1f;
            low = 0x1.fb5444p-11f;
        }
        numeratorWord = yWord;
        denominatorWord = xWord;
    }
    f32 t;
    if ((denominatorWord - numeratorWord) << 1 < 0x1000000) {
        // a ratio of magnitude 1/2..1: atan(r) = atan(1/2) + atan((r - 1/2) / (1 + r / 2))
        f32 half;
        if (((numeratorWord ^ denominatorWord) & 0x80000000) != 0) {
            half = -0.5f;
            low -= 0x1.9c1586p-16f;
            high -= 0x1.dacp-2f;
        } else {
            half = 0.5f;
            low += 0x1.9c1586p-16f;
            high += 0x1.dacp-2f;
        }
        t = (numerator - half * denominator) / (denominator + numerator * half);
    } else {
        t = numerator / denominator;
    }
    f32 z = t * t;
    low += t * z * (-0x1.55555p-2f + z * (0x1.9990c2p-3f + z * (-0x1.236a1ep-3f + z * (0x1.ab711ep-4f +
                                                                                      z * -0x1.cb5a5ap-5f))));
    return low + t + high;
}

// 0x0077B230 | nintendogs:bytes [tier A]
f32 __hardfp_ceilf(f32 x)
{
    s32 word = GetFloatWord(x);
    s32 exponent = (static_cast<u32>(word << 1) >> 24) - 127;
    if (exponent >= 23) {
        return x;                                   // integral, infinite or NaN
    }
    if (exponent < 0) {
        // |x| < 1: -0 for negative x, 1 for positive x
        if (HUGE + x > 0.0f) {
            if (word < 0) {
                word = static_cast<s32>(0x80000000);
            } else if (word != 0) {
                word = 0x3F800000;
            }
        }
    } else {
        u32 fraction = 0x7FFFFF >> exponent;
        if ((word & fraction) == 0) {
            return x;
        }
        if (HUGE + x > 0.0f) {
            if (word > 0) {
                word += 0x800000 >> exponent;
            }
            word &= ~fraction;
        }
    }
    return SetFloatWord(word);
}

// 0x0077B66C | nintendogs:bytes [tier A]
f32 __hardfp_floorf(f32 x)
{
    s32 word = GetFloatWord(x);
    s32 exponent = (static_cast<u32>(word << 1) >> 24) - 127;
    if (exponent >= 23) {
        return x;                                   // integral, infinite or NaN
    }
    if (exponent < 0) {
        // |x| < 1: 0 for positive x, -1 for negative x (-0 stays)
        if (HUGE + x > 0.0f) {
            if (word >= 0) {
                word = 0;
            } else if (word != static_cast<s32>(0x80000000)) {
                word = static_cast<s32>(0xBF800000);
            }
        }
    } else {
        u32 fraction = 0x7FFFFF >> exponent;
        if ((word & fraction) == 0) {
            return x;
        }
        if (HUGE + x > 0.0f) {
            if (word < 0) {
                word += 0x800000 >> exponent;
            }
            word &= ~fraction;
        }
    }
    return SetFloatWord(word);
}

} // extern "C"

namespace {
// x reduced to [-pi/4, pi/4] and its quadrant (sinf, cosf and tanf; inlined into each); the quadrant is -1 for
// a tiny x (|x| < 2^-25, returned as it is), infinity and NaN
DECOMP_ALWAYS_INLINE f32 ReduceArgument(f32 x, int* quadrant)
{
    u32 word = GetFloatWord(x);
    if (word << 1 < 0x7E921FB6) {
        *quadrant = word << 1 < 0x73000000 ? -1 : 0;   // |x| < pi/4
        return x;
    }
    if ((word & 0x7FFFFFFF) >= 0x46490E49) {
        return __mathlib_rredf2(x, quadrant, word);    // |x| >= 12868: Payne-Hanek
    }
    // Cody-Waite: x - k * pi/2 with pi/2 in four parts; k rounded by adding 2^23
    f32 t = x * 0x1.45f306p-1f;
    f32 k;
    if ((word & 0x80000000) != 0) {
        k = (t - 0x1p23f) + 0x1p23f;
    } else {
        k = (t + 0x1p23f) - 0x1p23f;
    }
    *quadrant = static_cast<s32>(k) & 3;
    x -= k * 0x1.92p+0f;
    x -= k * 0x1.fb4p-12f;
    x -= k * 0x1.444p-24f;
    x -= k * 0x1.68c234p-39f;
    return x;
}

// sin of a reduced x
DECOMP_ALWAYS_INLINE f32 SinPolynomial(f32 x, f32 z)
{
    return x + x * ((-0x1.55554p-3f + z * (0x1.1105b4p-7f - z * 0x1.98da66p-13f)) * z);
}

// cos of a reduced x
DECOMP_ALWAYS_INLINE f32 CosPolynomial(f32 z)
{
    return 1.0f + z * (-0x1.ffffbap-2f + z * (0x1.553f94p-5f + z * -0x1.647572p-10f));
}
} // namespace

extern "C" {

// 0x0077C730 | fefates:bytes [tier B]
f32 __mathlib_rredf2(f32 x, int* quadrant, u32 word)
{
    if (word << 1 >= 0xFF000000) {
        *quadrant = -1;                             // infinite or NaN
        return x;
    }
    // the mantissa times the bits of 2/pi around its exponent (96 bits): the quadrant and the fraction
    u32 mantissa = 0x80000000 | word << 8;
    s32 exponent = ((word << 1) >> 24) - 120;
    s32 shift = exponent & 31;
    const u32* bits = &s_TwoOverPiBits[exponent >> 5];
    // (a shift by 32 gives 0 on ARM)
    u32 high = bits[0] << shift | bits[1] >> (32 - shift);
    u32 middle = bits[1] << shift | bits[2] >> (32 - shift);
    u32 low = bits[2] << shift | bits[3] >> (32 - shift);
    u64 productHigh = static_cast<u64>(high) * mantissa;
    u64 productMiddle = static_cast<u64>(middle) * mantissa;
    u64 productLow = static_cast<u64>(low) * mantissa;
    u64 sum = (productLow >> 32) + static_cast<u32>(productMiddle);
    u32 word0 = static_cast<u32>(sum);
    sum = (sum >> 32) + (productMiddle >> 32) + static_cast<u32>(productHigh);
    u32 word1 = static_cast<u32>(sum);
    u32 word2 = static_cast<u32>(sum >> 32) + static_cast<u32>(productHigh >> 32);
    // the quadrant rounded to nearest; the fraction as a signed fixed point number in three parts
    u32 nearest = (word2 + 32) >> 6;
    *quadrant = nearest;
    f32 part0 = static_cast<f32>(word0) * 0x1p-38f;
    f32 part2 = static_cast<f32>(static_cast<s32>(word2 << 26 | (word1 >> 19) << 13));
    f32 part1 = static_cast<f32>(word1 << 13) * 0x1p-19f;
    f32 head = SetFloatWord((GetFloatWord(part2 + part1 + part0) + 0x800) >> 12 << 12);
    f32 tail = part0 - ((head - part2) - part1);
    f32 result = head * 0x1.fb5444p-44f + tail * 0x1.921fb6p-32f + head * 0x1.92p-32f;
    if ((word & 0x80000000) != 0) {
        *quadrant = 0x10000000 - nearest;
        return -result;
    }
    return result;
}

// 0x0077B2C0 | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_cosf(f32 x)
{
    int quadrant;
    x = ReduceArgument(x, &quadrant);
    if (quadrant < 0) {
        if (static_cast<u32>(GetFloatWord(x) << 1) > 0xFF000000) {
            return x + x;                           // NaN
        }
        if (static_cast<u32>(GetFloatWord(x) << 1) != 0xFF000000) {
            return 1.0f;                            // tiny
        }
        __set_errno(ERROR_DOMAIN);
        f32 zero = 0.0f;
        return zero / zero;
    }
    f32 result;
    if ((quadrant & 1) != 0) {
        result = SinPolynomial(x, x * x);
        if ((quadrant & 2) != 0) {
            return result;
        }
    } else {
        result = CosPolynomial(x * x);
        if ((quadrant & 2) == 0) {
            return result;
        }
    }
    return -result;
}

// 0x0077C38C | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_sinf(f32 x)
{
    int quadrant;
    x = ReduceArgument(x, &quadrant);
    if (quadrant < 0) {
        if (static_cast<u32>(GetFloatWord(x) << 1) > 0xFF000000) {
            return x + x;                           // NaN
        }
        if (static_cast<u32>(GetFloatWord(x) << 1) != 0xFF000000) {
            if (__ARM_fpclassifyf(x) == 4) {
                __mathlib_flt_underflow();              // a denormal x: the underflow flag
            }
            return x;                               // tiny
        }
        __set_errno(ERROR_DOMAIN);
        f32 zero = 0.0f;
        return zero / zero;
    }
    f32 z = x * x;
    f32 result = (quadrant & 1) != 0 ? CosPolynomial(z) : SinPolynomial(x, z);
    if ((quadrant & 2) != 0) {
        return -result;
    }
    return result;
}

// 0x0077C554 | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_tanf(f32 x)
{
    int quadrant;
    x = ReduceArgument(x, &quadrant);
    if (quadrant < 0) {
        if (static_cast<u32>(GetFloatWord(x) << 1) > 0xFF000000) {
            return x + x;                           // NaN
        }
        if (static_cast<u32>(GetFloatWord(x) << 1) != 0xFF000000) {
            if (__ARM_fpclassifyf(x) == 4) {
                __mathlib_flt_underflow();
            }
            return x;                               // tiny
        }
        __set_errno(ERROR_DOMAIN);
        f32 zero = 0.0f;
        return zero / zero;
    }
    f32 z = x * x;
    f32 result = x + x * ((0x1.555452p-2f +
                           z * (0x1.11426ap-3f +
                                z * (0x1.b36b58p-5f + z * (0x1.9ced14p-6f + z * (0x1.0cda18p-9f + z * 0x1.45d376p-7f))))) *
                          z);
    if ((quadrant & 1) != 0) {
        return -1.0f / result;                      // tan(x + pi/2) = -1 / tan(x)
    }
    return result;
}

// 0x0077B43C | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_expf(f32 x)
{
    s32 word = GetFloatWord(x);
    bool isLarge = false;
    if (static_cast<u32>((word << 1) + 0x99000000) > 0x1E500000) {
        // |x| outside [2^-24, 85.25]
        if ((static_cast<u32>(~word << 1) >> 24) == 0) {
            // infinite or NaN: exp(-inf) = 0
            if (word != static_cast<s32>(0xFF800000)) {
                return x + x;
            }
            return 0.0f;
        }
        if (static_cast<u32>(word << 1) < 0x67000000) {
            return 1.0f;                            // |x| < 2^-24
        }
        if (static_cast<u32>(word << 1) > 0x85A00000) {
            __set_errno(ERROR_RANGE);
            if (word >= 0) {
                f32 huge = 0x1p97f;
                return huge * huge;                 // overflow
            }
            return __mathlib_flt_underflow();
        }
        isLarge = true;                             // the result may overflow or be denormal
    }
    // x = k * ln(2) / 4 + r: exp(x) = 2^(k >> 2) * 2^((k & 3) / 4) * exp(r)
    s32 k = static_cast<s32>(x * 0x1.715476p+2f + (word < 0 ? -0.5f : 0.5f));
    u32 index = k & 3;
    f32 kf = static_cast<f32>(k);
    f32 r = x - kf * 0x1.62ep-3f;
    r = r - kf * 0x1.0bfbe8p-17f;
    f32 p = (0x1.fffffep-1f + r * (0x1.fffffep-2f + r * (0x1.557624p-3f + r * 0x1.557624p-5f))) * r;
    f32 result = s_Exp2QuarterLow[index] + s_Exp2Quarter[index] * p + s_Exp2QuarterHigh[index];
    s32 n = k >> 2;
    if (!isLarge) {
        // 2^n built through its bit pattern (the integer conversion of (n << 23) + 0x3F800000)
        return SetFloatWord(static_cast<s32>(static_cast<u32>(static_cast<f32>(n) * 0x1p23f + 0x1.fcp29f))) *
               result;
    }
    // two steps of scaling: 2^(n / 2) and 2^(n - n / 2)
    s32 half = n / 2;
    result = SetFloatWord(0x3F800000 + ((n - half) << 23)) * (SetFloatWord(0x3F800000 + (half << 23)) * result);
    s32 resultWord = GetFloatWord(result);
    if (resultWord == 0) {
        __set_errno(ERROR_RANGE);
        return __mathlib_flt_underflow();
    }
    if (resultWord == 0x7F800000) {
        __set_errno(ERROR_RANGE);
        f32 huge = 0x1p97f;
        return huge * huge;
    }
    if (__ARM_fpclassifyf(result) == 4) {
        __mathlib_flt_underflow();                  // a denormal result: the underflow flag
    }
    return result;
}

// 0x0077B708 | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_fmodf(f32 x, f32 y)
{
    // an infinite x or a y of 0 has no result (unless the other one is NaN)
    s32 xWord = GetFloatWord(x);
    f32 zero = 0.0f;
    if (static_cast<u32>(0x7F800000 - (xWord & 0x7FFFFFFF)) >> 31 != 0 ||
        static_cast<u32>(0x7F800000 - (GetFloatWord(y) & 0x7FFFFFFF)) >> 31 != 0 ||
        (static_cast<u32>(xWord << 1) != 0xFF000000 && static_cast<u32>(GetFloatWord(y) << 1) != 0)) {
        // the IEEE remainder lies in [-|y|/2, |y|/2]; fmod has the sign of x
        f32 result = _frem(x, y);
        s32 word = GetFloatWord(result);
        if ((~(static_cast<u32>(word) >> 23) & 0xFF) == 0) {
            return result;
        }
        if ((xWord & 0x80000000) == (word & 0x80000000)) {
            return result;
        }
        return result - SetFloatWord((word & 0x80000000) | (GetFloatWord(y) & 0x7FFFFFFF));
    }
    __set_errno(ERROR_DOMAIN);
    return zero / zero;
}

// 0x0077B7E4 (name is ours)
f32 __hardfp_log10f(f32 x)
{
    s32 word = GetFloatWord(x);
    s32 exponentAdjust = 0;
    if (static_cast<u32>(word - 0x800000) >= 0x7F000000) {
        // not a positive normal number
        if (static_cast<u32>(word << 1) > 0xFF000000) {
            return x + x;                           // NaN
        }
        if (word == 0x7F800000) {
            return x;                               // +inf
        }
        f32 zero = 0.0f;
        if (x < zero) {
            __set_errno(ERROR_DOMAIN);
            return zero / zero;
        }
        if (x == zero) {
            __set_errno(ERROR_RANGE);
            return -1.0f / zero;
        }
        // a denormal: scaled by 2^23
        x *= 0x1p23f;
        exponentAdjust = -23;
        word = GetFloatWord(x);
    }
    // x = 2^exponent * m with m in [0.75, 1.5); c = 1 + index / 8 is the nearest eighth
    s32 exponent = ((word + 0x80000) >> 23) - 127;
    u32 index = (static_cast<u32>(word << 9) + 0x10000000) >> 29;
    f32 m = SetFloatWord(word - (exponent << 23));
    f32 c = SetFloatWord(0x3F800000 + (index << 20));
    s32 n = exponentAdjust + exponent;
    if ((n | index) == 0) {
        // x close to 1: a polynomial in f = x - 1
        f32 f = m - 1.0f;
        f32 result = f * f * (-0x1.bcb7b0p-3f + f * (0x1.287c5ap-3f + f * (-0x1.bcf2e4p-4f + f * 0x1.529f24p-4f)));
        result += f * 0x1.96f62ap-7f;
        result += f * 0x1.b0p-2f;
        return result;
    }
    // log10(m / c) = 2 / ln(10) * atanh(s) with s = (m - c) / (m + c)
    f32 s = (m - c) / (m + c);
    f32 z = s * s;
    f32 p = s * z * (0x1.287a76p-2f + z * 0x1.6405bcp-3f);
    f32 high = s_Log10TableHigh[index] + static_cast<f32>(n) * 0x1.344p-2f;
    f32 low = s_Log10TableLow[index] + static_cast<f32>(n) * 0x1.3509f8p-18f;
    return high + (low + p + s * 0x1.bcb7b2p-1f);
}

// 0x0077B9B8 | nintendogs:callgraph [tier A]
f32 __hardfp_logf(f32 x)
{
    s32 word = GetFloatWord(x);
    s32 exponentAdjust = 0;
    if (static_cast<u32>(word - 0x800000) >= 0x7F000000) {
        // not a positive normal number
        if (static_cast<u32>(word << 1) > 0xFF000000) {
            return x + x;                           // NaN
        }
        if (word == 0x7F800000) {
            return x;                               // +inf
        }
        f32 zero = 0.0f;
        if (x < zero) {
            __set_errno(ERROR_DOMAIN);
            return zero / zero;
        }
        if (x == zero) {
            __set_errno(ERROR_RANGE);
            return -1.0f / zero;
        }
        // a denormal: scaled by 2^23
        x *= 0x1p23f;
        exponentAdjust = -23;
        word = GetFloatWord(x);
    }
    // x = 2^exponent * m with m in [0.75, 1.5); c = 1 + index / 8 is the nearest eighth
    s32 exponent = ((word + 0x80000) >> 23) - 127;
    u32 index = (static_cast<u32>(word << 9) + 0x10000000) >> 29;
    f32 m = SetFloatWord(word - (exponent << 23));
    f32 c = SetFloatWord(0x3F800000 + (index << 20));
    s32 n = exponentAdjust + exponent;
    if ((n | index) == 0) {
        // x close to 1: a polynomial in f = x - 1
        f32 f = m - 1.0f;
        return f + f * f * (-0x1.fffffep-2f + f * (0x1.555782p-2f + f * (-0x1.002214p-2f + f * 0x1.85da42p-3f)));
    }
    // ln(m / c) = 2 * atanh(s) with s = (m - c) / (m + c)
    f32 s = (m - c) / (m + c);
    f32 z = s * s;
    f32 p = s * z * (0x1.555554p-1f + z * 0x1.99e2ccp-2f);
    f32 high = s_LogTableHigh[index] + static_cast<f32>(n) * 0x1.62ep-1f;
    f32 low = s_LogTableLow[index] + static_cast<f32>(n) * 0x1.0bfbe8p-15f;
    return high + (low + p + s * 2.0f);
}

// 0x0077BB78 (name is ours)
f32 __hardfp_modff(f32 x, f32* integralPart)
{
    s32 word = GetFloatWord(x);
    if (static_cast<u32>(word << 1) > 0xFF000000) {
        *integralPart = x;                          // NaN
        return x;
    }
    s32 sign = word & 0x80000000;
    s32 exponent = (static_cast<u32>(word << 1) >> 24) - 127;
    if (exponent < 23) {
        if (exponent < 0) {
            *integralPart = SetFloatWord(word & 0x80000000);
            return x;
        }
        u32 fraction = 0x7FFFFF >> exponent;
        if ((word & fraction) != 0) {
            f32 integral = SetFloatWord(word & ~fraction);
            *integralPart = integral;
            return x - integral;
        }
    }
    // integral (or infinite): the fraction is a signed 0
    *integralPart = x;
    return SetFloatWord(sign);
}

// 0x0077BC18 | nintendogs:bytes-fuzzy [tier A]
f32 __hardfp_powf(f32 x, f32 y)
{
    u32 yWord = GetFloatWord(y);
    s32 exponentAdjust = 0;
    f32 sign = 1.0f;                                // -1 for a negative x and an odd integer y
    f32 zero = 0.0f;
    u32 xWord = GetFloatWord(x);
    // the usual case: a positive normal x and 2^-96 <= |y| < 2^127
    bool isXNormal = xWord - 0x800000 < 0x7F000000;
    u32 yRange = (yWord << 1) + 0x2000000;
    if (isXNormal && yRange >= 0x40000000) {
        goto compute;
    }
    if (yRange < 0x40000000) {
        if (yRange >= 0x2000000) {
            // |y| < 2^-96: as +-2^-64 (the result is 1 rounded); pow(x, 0) = 1
            if (y != zero) {
                yWord = (yWord & 0x80000000) | 0x1F800000;
                y = SetFloatWord(yWord);
            } else if ((xWord << 1) != 0 && 0xFF000000 > (xWord << 1)) {
                return 1.0f;
            }
        } else if (yRange < 0x1000000) {
            y = SetFloatWord(yWord - 0x800000);     // 2^127 <= |y|: halved (the result overflows anyway)
        } else {
            goto special;                           // y infinite or NaN
        }
    }
    if (xWord + 0x7F800000 < 0x7F000000) {
        goto negative;                              // a negative normal x
    }
    if ((xWord << 1) != 0 && (xWord << 1) < 0x1000000) {
        // a denormal x: scaled by 2^27
        exponentAdjust = -27;
        x *= 0x1p27f;
        xWord = GetFloatWord(x);
        if ((xWord & 0x80000000) != 0) {
            goto negative;
        }
        goto compute;
    }
    if (xWord - 0x800000 < 0x7F000000) {
        goto compute;
    }
    goto special;

negative:
    // a negative x: y must be an integer, the result is negative for an odd y
    x = __builtin_fabsf(x);
    xWord = GetFloatWord(x);
    {
        s32 exponent = static_cast<s32>((yWord << 1) >> 24);
        if (exponent < 127) {
            goto domainError;
        }
        if (exponent < 151) {
            u32 bit = 1u << (150 - exponent);
            if (((bit - 1) & yWord) != 0) {
                goto domainError;
            }
            if ((yWord & bit) != 0) {
                sign = -1.0f;
            }
        }
    }
    goto compute;

domainError:
    __set_errno(ERROR_DOMAIN);
    return zero / zero;

special: {
    // zeros, infinities and NaNs
    u32 xMagnitude = xWord & 0x7FFFFFFF;
    u32 yMagnitude = yWord & 0x7FFFFFFF;
    if (xMagnitude - 0x7F800001 < 0x3FFFFF || 0x3FFFFF > yMagnitude - 0x7F800001) {
        return x + y;                               // a signaling NaN
    }
    if (xWord == 0x3F800000 || (yWord << 1) == 0) {
        return 1.0f;
    }
    if (xMagnitude > 0x7F800000 || yMagnitude > 0x7F800000) {
        return x + y;                               // a quiet NaN
    }
    if (xWord == 0x7F800000) {
        return (yWord & 0x80000000) != 0 ? zero : __builtin_inff();
    }
    // y: 0 not an integer, 1 odd, 2 even
    int yKind;
    s32 exponent = static_cast<s32>((yWord << 1) >> 24);
    if (exponent < 127) {
        yKind = 0;
    } else if (exponent >= 151) {
        yKind = 2;
    } else {
        u32 bit = 1u << (150 - exponent);
        if (((bit - 1) & yWord) != 0) {
            yKind = 0;
        } else if ((bit & yWord) != 0) {
            yKind = 1;
        } else {
            yKind = 2;
        }
    }
    if (xWord == 0xFF800000) {
        if ((yWord & 0x80000000) != 0) {
            return yKind == 1 ? -0.0f : zero;
        }
        return yKind == 1 ? -__builtin_inff() : __builtin_inff();
    }
    if (xWord == 0) {
        if ((yWord & 0x80000000) != 0) {
            goto poleError;
        }
        return zero;
    }
    if (xWord == 0x80000000) {
        if (yWord == 0x7F800000) {
            return zero;
        }
        if (yWord != 0xFF800000) {
            if ((yWord & 0x80000000) == 0) {
                return yKind == 1 ? -0.0f : zero;
            }
            if (yKind == 1) {
                __set_errno(ERROR_RANGE);
                return 1.0f / x;                    // -inf
            }
        }
        goto poleError;
    }
    if (xWord == 0xBF800000) {
        return 1.0f;                                // pow(-1, +-inf)
    }
    // y infinite: |x| > 1 with y > 0 or |x| < 1 with y < 0 overflows
    if (((xWord << 1) >= 0x7F000000) != (static_cast<s32>(yWord) >= 0)) {
        return zero;
    }
    return __builtin_inff();
}

poleError:
    __set_errno(ERROR_RANGE);
    return 1.0f / zero;

compute: {
    // 16 * log2(x) in two parts
    f32 high;
    f32 low;
    if (xWord + 0xC0801000 <= 0x2000) {
        // x close to 1: from f = x - 1
        f32 f = static_cast<f32>(static_cast<f64>(x) - 1.0);
        f32 square = -(f * f) * (0.5f - f * (0x1.555556p-2f - f * 0.25f));
        f32 a = f * 0x1.716p+4f;
        f32 b = f * -0x1.7135a8p-9f + square * 0x1.715476p+4f;
        high = RoundTo12Bits(a + b);
        low = (a - high) + b;
    } else {
        // x = 2^exponent * m, c = 1 + index / 16 near m: log2(m / c) = 2 / ln(2) * atanh(u), u = (m - c) / (m + c)
        u32 rounded = (xWord + 0x40000) >> 19;
        u32 index = rounded & 15;
        s32 exponent = static_cast<s32>(rounded << 20 >> 24) - 127;
        f32 c = 1.0f + static_cast<f32>(static_cast<s32>(index)) * 0.0625f;
        f32 m = SetFloatWord(xWord - (exponent << 23));
        s32 n = exponent + exponentAdjust;
        f32 sum = m + c;
        f32 difference = m - c;
        f32 sumHigh = RoundTo12Bits(sum);
        f32 sumLow = (c - sumHigh) + m;
        f32 inverse = 1.0f / sum;
        f32 uHigh = RoundTo12Bits(difference * inverse);
        f32 uLow = (difference - uHigh * sumHigh - uHigh * sumLow) * inverse;
        f32 u = uHigh + uLow;
        f32 u2 = u * u;
        f32 p = (0x1.555556p-2f + u2 * 0x1.99c4fcp-3f) * u2;
        f32 tLow = uLow + p * u;
        f32 tHigh = RoundTo12Bits(uHigh + tLow);
        tLow = (uHigh - tHigh) + tLow;
        f32 a = tHigh * 0x1.716p+5f;
        f32 b = tHigh * -0x1.7135a8p-8f + tLow * 0x1.715476p+5f;
        f32 abHigh = RoundTo12Bits(a + b);
        f32 abLow = (a - abHigh) + b;
        const Log2Entry& entry = s_Log2Sixteenths[index];
        f32 nf = static_cast<f32>(n << 4);
        high = RoundTo12Bits(abLow + entry.low + abHigh + entry.high + nf);
        low = abLow - ((((high - nf) - entry.high) - abHigh) - entry.low);
    }
    // times y (split in two), then 2^(k / 16) * 2^(r / 16)
    f32 yHigh = RoundTo12Bits(y);
    f32 yLow = y - yHigh;
    f32 a = high * yHigh;
    f32 b = high * yLow + low * (yHigh + yLow);
    f32 productHigh = RoundTo12Bits(a + b);
    f32 productLow = (a - productHigh) + b;
    f32 total = productHigh + productLow;
    s32 k = static_cast<s32>((total < zero ? -0.5f : 0.5f) + total);
    f32 kHigh = static_cast<f32>(k >> 4);
    u32 fraction = k & 15;
    f32 r = (productHigh - static_cast<f32>(k)) + productLow;
    f32 poly = (0x1.62e43p-5f + r * (0x1.ebff92p-11f + r * 0x1.c6b3f8p-17f)) * r;
    f32 result = s_Exp2SixteenthLow[fraction] + s_Exp2Sixteenth[fraction] * poly + s_Exp2SixteenthHigh[fraction];
    if (__builtin_fabsf(kHigh) < 126.0f) {
        // 2^kHigh built through its bit pattern
        return SetFloatWord(_ffixu((kHigh + 127.0f) * 0x1p23f)) * (sign * result);
    }
    f32 huge = 0x1p97f;
    if (__builtin_fabsf(kHigh) < 192.0f) {
        // in two steps: 2^(n / 2) and 2^(n - n / 2)
        s32 n = static_cast<s32>(kHigh);
        s32 half = n / 2;
        result = SetFloatWord(0x3F800000 + (half << 23)) * (sign * result) * SetFloatWord(0x3F800000 + ((n - half) << 23));
        if (static_cast<u32>(GetFloatWord(result) << 1) == 0xFF000000) {
            goto overflow;
        }
        if (static_cast<u32>(GetFloatWord(result) << 1) == 0) {
            goto underflow;
        }
        if (__ARM_fpclassifyf(result) == 4) {
            __mathlib_flt_underflow();              // a denormal result: the underflow flag
        }
        return result;
    }
    if (kHigh >= zero) {
        goto overflow;
    }
underflow:
    __set_errno(ERROR_RANGE);
    return SetFloatWord((GetFloatWord(__mathlib_flt_underflow()) & 0x7FFFFFFF) | (GetFloatWord(sign) & 0x80000000));
overflow:
    __set_errno(ERROR_RANGE);
    return SetFloatWord((GetFloatWord(huge * huge) & 0x7FFFFFFF) | (GetFloatWord(sign) & 0x80000000));
}
}

// sqrtf with the base procedure call standard (the float in r0): EDOM for a negative x
// 0x007C30E0 (name is ours)
__attribute__((pcs("aapcs"))) f32 __softfp_sqrtf(f32 x)
{
    f32 result = __builtin_sqrtf(x);
    if (static_cast<u32>(0x7F800000 - (GetFloatWord(result) & 0x7FFFFFFF)) >> 31 != 0 &&
        static_cast<u32>(0x7F800000 - (GetFloatWord(x) & 0x7FFFFFFF)) >> 31 == 0) {
        __set_errno(ERROR_DOMAIN);
    }
    return result;
}

// the result of an underflow: a tiny number squared (raises the underflow and inexact flags)
// 0x0077C708 | nintendogs:bytes [tier A]
f64 __mathlib_dbl_underflow()
{
    f64 tiny = 0x1p-767;
    return tiny * tiny;
}

// 0x0077C720 | nintendogs:callgraph [tier A]
DECOMP_NOIPA f32 __mathlib_flt_underflow()
{
    f32 tiny = 0x1p-95f;
    asm("" : "+t"(tiny));                           // (computed at run time: GCC would fold it to 0)
    return tiny * tiny;
}

} // extern "C"
