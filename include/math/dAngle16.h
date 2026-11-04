#pragma once

#include "decomp.h"

namespace math {
class Angle16
{
public:
    static Angle16 Atan2(float, float); // 0x005667A8 | libgarden [tier A]

    u16 value; // 0x0 (name after libgarden)
};

// a rotation as three angles (name is ours; an aggregate: the stage definitions are arrays of it)
struct Angle16Vec3
{
    Angle16 x; // 0x0
    Angle16 y; // 0x2
    Angle16 z; // 0x4
};
ASSERT_SIZE(Angle16Vec3, 0x6);
} // namespace math
