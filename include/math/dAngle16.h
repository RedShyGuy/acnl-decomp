#pragma once

#include "decomp.h"

namespace math {
class Angle16
{
public:
    static Angle16 Atan2(float, float); // 0x005667A8 | libgarden [tier A]

    u16 value; // 0x0 (name after libgarden)
};

// the conversions between radians, degrees and 16-bit angles (0x10000 = a full turn); the game reads them from memory
// (names are ours)
extern const f32 NEG_PI;
extern const f32 PI;
extern const f32 ANGLE16_TO_DEG;
extern const f32 ANGLE16_TO_RAD;
extern const f32 DEG_TO_RAD;
extern const f32 DEG_TO_ANGLE16;
extern const f32 RAD_TO_ANGLE16;
extern const f32 RAD_TO_DEG;

// a rotation as three angles (name is ours; an aggregate: the stage definitions are arrays of it)
struct Angle16Vec3
{
    Angle16 x; // 0x0
    Angle16 y; // 0x2
    Angle16 z; // 0x4
};
ASSERT_SIZE(Angle16Vec3, 0x6);
} // namespace math
