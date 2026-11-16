#include "field/dFieldMath.h"

#include "ssys/ma/ma_Math.h"

// 0x006F7360 (name is ours)
f32 GetTriangleHeight(const nn::math::VEC3& pos, const nn::math::VEC3& a, const nn::math::VEC3& b,
                      const nn::math::VEC3& c)
{
    nn::math::VEC3 ab(b.x - a.x, b.y - a.y, b.z - a.z);
    nn::math::VEC3 bc(c.x - b.x, c.y - b.y, c.z - b.z);
    nn::math::VEC3 normal(ab.y * bc.z - ab.z * bc.y, ab.z * bc.x - ab.x * bc.z, ab.x * bc.y - ab.y * bc.x);
    ssys::ma::NormalizeIfPossible(&normal);
    return a.y - ((pos.x - a.x) * normal.x + (pos.z - a.z) * normal.z) / normal.y;
}

// 0x006F9A5C (name is ours)
f32 GetWaterSurfaceHeight()
{
    return 32.0f;
}
