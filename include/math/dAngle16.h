#pragma once

#include "decomp.h"

namespace math {
class Angle16
{
public:
    void Atan2(float, float); // 0x005667A8 | libgarden [tier A]

    u16 value; // 0x0 (name after libgarden)
};

// a rotation as three angles (name is ours)
struct Angle16Vec3
{
    constexpr Angle16Vec3() : x{0}, y{0}, z{0} {}
    constexpr Angle16Vec3(u16 ax, u16 ay, u16 az) : x{ax}, y{ay}, z{az} {}

    Angle16 x; // 0x0
    Angle16 y; // 0x2
    Angle16 z; // 0x4
};
ASSERT_SIZE(Angle16Vec3, 0x6);
} // namespace math
