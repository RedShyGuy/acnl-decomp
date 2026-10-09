#pragma once

#include "decomp.h"

// Helpers for writing PICA command buffers (names are ours). The original had them inline in the
// gr headers; ARMCC expanded them everywhere and kept no out-of-line copies (Float32ToFloat24,
// which has one, is in CTR_Api.h).
//
// A command is the parameter word followed by the header word (3dbrew "GPU/Internal Registers"):
// bits 0-15 register id, 16-19 byte mask, 20-27 number of extra parameters, 31 consecutive
// registers. Commands are padded to 8 bytes.

namespace nn {
namespace gr {
namespace CTR {
namespace detail {
const u32 DUMMY_COMMAND = 0xEAD0FEAD; // padding word

inline u32 CommandHeader(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

inline u32 BitsOf(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    return bits.u;
}

inline bool IsInfOrNan(u32 bits)
{
    return ((bits << 1) >> 24) == 0xFF;
}

// 1.5.10 float
inline u32 Float32ToFloat16(f32 value)
{
    const u32 bits = BitsOf(value);
    const u32 sign = bits >> 31;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 112;
        if (exponent < 0) {
            return sign << 15;
        }
    }
    return static_cast<u16>(((bits & 0x7FFFFF) >> 13) | (exponent << 10) | (sign << 15));
}

// 1.7.23 float
inline u32 Float32ToFloat31(f32 value)
{
    const u32 bits = BitsOf(value);
    const u32 sign = bits >> 31;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
        if (exponent < 0) {
            return sign << 30;
        }
    }
    return (bits & 0x7FFFFF) | (exponent << 23) | (sign << 30);
}

// [0, 1] to a 24 bit fraction
inline u32 Float32ToUnsignedFix24(f32 value)
{
    if (value <= 0.0f || IsInfOrNan(BitsOf(value))) {
        return 0;
    }
    value *= 16777216.0f;
    if (value >= 16777216.0f) {
        return 0xFFFFFF;
    }
    return static_cast<u32>(value);
}

// [0, 1] to a 16 bit fraction
inline u32 Float32ToUnsignedFix16(f32 value)
{
    if (value <= 0.0f || IsInfOrNan(BitsOf(value))) {
        return 0;
    }
    value *= 65536.0f;
    if (value >= 65536.0f) {
        return 0xFFFF;
    }
    return static_cast<u32>(value);
}

// signed 5.8 fixed point (two's complement in 13 bits)
inline u32 Float32ToFix13Fraction8(f32 value)
{
    if (value == 0.0f || IsInfOrNan(BitsOf(value))) {
        return 0;
    }
    f32 fixed = (value + 16.0f) * 256.0f;
    if (fixed < 0.0f) {
        fixed = 0.0f;
    } else if (fixed >= 8192.0f) {
        fixed = 8191.0f;
    }
    if (fixed < 4096.0f) {
        return static_cast<u32>(fixed + 4096.0f);
    }
    return static_cast<u32>(fixed - 4096.0f);
}

// [0, 1] to a byte, rounded
inline u8 Float32ToUnsignedByte(f32 value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    return static_cast<u8>(static_cast<u32>(value * 255.0f + 0.5f));
}

inline u32 PackColor(u8 r, u8 g, u8 b, u8 a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}
} // namespace detail
} // namespace CTR
} // namespace gr
} // namespace nn
