#include "nn/nex/nex_RandomNumberGenerator.h"

namespace nn {
namespace nex {
// 0x0039A7E4 | mk7dlp:bytes-fuzzy [tier A]
u32 nn::nex::RandomNumberGenerator::GetRandomNumber(unsigned int max)
{
    if (max == 0) {
        return 0;
    }
    if (m_Index >= N) {
        // (never seeded)
        if (m_Index == N + 1) {
            SetSeed(DEFAULT_SEED);
        }
        u32 i;
        for (i = 0; i < N - M; i++) {
            u32 y = (m_State[i] & UPPER_MASK) | (m_State[i + 1] & LOWER_MASK);
            m_State[i] = m_State[i + M] ^ (y >> 1) ^ m_Mag01[y & 1];
        }
        for (; i < N - 1; i++) {
            u32 y = (m_State[i] & UPPER_MASK) | (m_State[i + 1] & LOWER_MASK);
            m_State[i] = m_State[i + (M - N)] ^ (y >> 1) ^ m_Mag01[y & 1];
        }
        u32 y = (m_State[N - 1] & UPPER_MASK) | (m_State[0] & LOWER_MASK);
        m_State[N - 1] = m_State[M - 1] ^ (y >> 1) ^ m_Mag01[y & 1];
        m_Index = 0;
    }
    u32 y = m_State[m_Index++];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= y >> 18;
    return y % max;
}

} // namespace nex
} // namespace nn
