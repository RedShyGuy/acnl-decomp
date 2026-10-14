#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// A Mersenne twister (MT19937 with the seeding by 69069; the default seed is 4357). The names of
// the members, the constants and the functions without a reference name are ours.
class RandomNumberGenerator
{
public:
    static const u32 N = 624;
    static const u32 M = 397;
    static const u32 MATRIX_A = 0x9908B0DF;
    static const u32 UPPER_MASK = 0x80000000;
    static const u32 LOWER_MASK = 0x7FFFFFFF;
    static const u32 DEFAULT_SEED = 4357;

    // (inline)
    RandomNumberGenerator()
    {
        m_Mag01[0] = 0;
        m_Mag01[1] = MATRIX_A;
        SetSeed(1);
    }
    // (inline) a seed of 0 is the default seed
    void SetSeed(u32 seed)
    {
        if (seed == 0) {
            seed = DEFAULT_SEED;
        }
        m_State[0] = seed;
        for (m_Index = 1; m_Index < N; m_Index++) {
            m_State[m_Index] = 69069 * m_State[m_Index - 1];
        }
    }
    // a number below max (0 for 0)
    u32 GetRandomNumber(unsigned int max); // 0x0039A7E4 | mk7dlp:bytes-fuzzy [tier A]

    u32 m_State[N]; // 0x000
    u32 m_Index;    // 0x9C0
    u32 m_Mag01[2]; // 0x9C4
};
ASSERT_SIZE(RandomNumberGenerator, 0x9CC);
} // namespace nex
} // namespace nn
