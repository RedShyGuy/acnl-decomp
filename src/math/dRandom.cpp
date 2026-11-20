#include "math/dRandom.h"

namespace math {
// 0x00976408
Random g_Random(0);

// 0x005666F4
f32 GetRandomF32(f32 max)
{
    return g_Random.GetF32() * max;
}

// 0x00566974
u32 math::Random::GetU32()
{
    Step();
    return m_State;
}

// 0x005669B0
s32 math::Random::GetS32(s32 max)
{
    return static_cast<s32>(GetF32() * static_cast<f32>(max));
}
} // namespace math
