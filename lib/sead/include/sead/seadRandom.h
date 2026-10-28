#pragma once

// sead::Random - a xorshift random number generator like pead::Random (the member names and the
// overloads of init and getU32 are ours)

#include "decomp.h"

namespace sead {
class Random
{
public:
    // seeded with the system tick count (the function of the binary is pead::Random::init)
    void init();
    void init(u32 seed); // 0x0055D5A4 | nintendogs:bytes [tier A]
    // the state itself (all zero is replaced by the state of seed 0)
    void init(u32 x, u32 y, u32 z, u32 w); // 0x0055D5FC (name is ours)
    u32 getU32(); // 0x0055D640 | mk7dlp:callgraph [tier C]
    // a value in [0, ceil) (name is ours)
    u32 getU32(u32 ceil) { return static_cast<u32>((static_cast<u64>(getU32()) * ceil) >> 32); }

    u32 mX; // 0x0
    u32 mY; // 0x4
    u32 mZ; // 0x8
    u32 mW; // 0xC
};
ASSERT_SIZE(Random, 0x10);
} // namespace sead
