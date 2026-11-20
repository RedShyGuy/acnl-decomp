#pragma once

#include "decomp.h"

// eight flags in a byte (no RTTI; name is ours): bits outside of 0..7 are ignored (the copies of the inline functions lie
// in the shared code)
struct BitFlag8
{
    BitFlag8() : m_Bits(0) {}

    DECOMP_EMIT_COPY DECOMP_NOINLINE inline bool Test(u32 bit) const; // 0x00317B54
    DECOMP_EMIT_COPY DECOMP_NOINLINE inline void Set(u32 bit, bool on); // 0x00317BE8

    u8 m_Bits; // 0x0
};
ASSERT_SIZE(BitFlag8, 0x1);

inline bool BitFlag8::Test(u32 bit) const
{
    if (bit >= 8) {
        return false;
    }
    return ((&m_Bits)[bit >> 3] >> (bit & 7)) & 1;
}

inline void BitFlag8::Set(u32 bit, bool on)
{
    u8* bits = &(&m_Bits)[bit >> 3];
    const u8 mask = 1 << (bit & 7);
    if (on) {
        if (bit < 8) {
            *bits |= mask;
        }
    } else {
        if (bit < 8) {
            *bits &= ~mask;
        }
    }
}
