#pragma once

// pead::Random - a xorshift random number generator (the class and init are from the nintendogs
// symbols, there for sead). The member names, getU32 and the inline functions are ours.

#include "decomp.h"

namespace pead {
class Random
{
public:
    Random() { init(); }

    // seeded with the system tick count
    void init(); // 0x0053BEF0 | nintendogs:bytes [tier A]

    void init(u32 seed)
    {
        mX = (seed ^ (seed >> 30)) * 0x6C078965 + 1;
        mY = (mX ^ (mX >> 30)) * 0x6C078965 + 2;
        mZ = (mY ^ (mY >> 30)) * 0x6C078965 + 3;
        mW = (mZ ^ (mZ >> 30)) * 0x6C078965 + 4;
    }

    u32 getU32(); // 0x0053BF64
    // a value in [0, ceil)
    u32 getU32(u32 ceil) { return static_cast<u32>((static_cast<u64>(getU32()) * ceil) >> 32); }

    u32 mX; // 0x0
    u32 mY; // 0x4
    u32 mZ; // 0x8
    u32 mW; // 0xC
};
} // namespace pead
