#pragma once

#include "decomp.h"

namespace math {
// The random numbers of the game (no RTTI; names are ours): a word of state, each step (state * 0x19660D + 0x3C6EF35F)
// with the high word of the product added back.
class Random
{
public:
    // (a copy of the inline constructor; constexpr: g_Random is built at compile time)
    DECOMP_EMIT_COPY constexpr explicit Random(u32 seed) : m_State(seed) {} // 0x005669D4

    u32 GetU32(); // 0x00566974
    // a value in [0, 1) from the upper 23 bits of the state (inline; the copy lies in the shared code)
    DECOMP_EMIT_COPY DECOMP_NOINLINE f32 GetF32() // 0x0013C938
    {
        Step();
        u32 bits = 0x3F800000 | (m_State >> 9);
        f32 value;
        __builtin_memcpy(&value, &bits, sizeof(value));
        return value - 1.0f;
    }
    // a value in [0, max)
    s32 GetS32(s32 max); // 0x005669B0

    u32 m_State; // 0x0

private:
    void Step()
    {
        const u64 value = static_cast<u64>(m_State) * 0x19660D + 0x3C6EF35F;
        m_State = static_cast<u32>(value) + static_cast<u32>(value >> 32);
    }
};
ASSERT_SIZE(Random, 0x4);

// the random numbers of the game (name is ours)
extern Random g_Random;
// a value in [0, max) from g_Random (armlink removed a call at the start)
f32 GetRandomF32(f32 max); // 0x005666F4
} // namespace math
