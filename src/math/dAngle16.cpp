#include "math/dAngle16.h"

namespace math {
// 0x008AF140
extern const f32 NEG_PI = -3.14159274f;
// 0x008AF144
extern const f32 PI = 3.14159274f;
// 0x008AF148
extern const f32 ANGLE16_TO_DEG = 360.0f / 65536.0f;
// 0x008AF14C
extern const f32 ANGLE16_TO_RAD = 9.58738019e-05f;
// 0x008AF150
extern const f32 DEG_TO_RAD = 0.0174532924f;
// 0x008AF154
extern const f32 DEG_TO_ANGLE16 = 65536.0f / 360.0f;
// 0x008AF158
extern const f32 RAD_TO_ANGLE16 = 10430.3779f;
// 0x008AF15C
extern const f32 RAD_TO_DEG = 57.2957764f;

// 0x005667A8 | libgarden [tier A]
math::Angle16 math::Angle16::Atan2(float, float)
{
}

} // namespace math

#include "stage/stage_Extern.h"

// (names are ours)
// 0x00AE6864
extern const nn::math::VEC3 VEC3_ZERO(0.0f, 0.0f, 0.0f);
// 0x00AE6870
extern const nn::math::VEC3 VEC3_ONE(1.0f, 1.0f, 1.0f);
// 0x00976C90
extern const nn::math::VEC2 VEC2_ZERO(0.0f, 0.0f);
// 0x00976C0E
extern const math::Angle16Vec3 ANGLE16_VEC3_ZERO = {0, 0, 0};
