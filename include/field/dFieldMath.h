#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

// Helpers of the field in other game files (names are ours).

// the height of a triangle at the x/z position of a point (a, b, c: the corners)
f32 GetTriangleHeight(const nn::math::VEC3& pos, const nn::math::VEC3& a, const nn::math::VEC3& b,
                      const nn::math::VEC3& c); // 0x006F7360
// the height of the water surface (32)
f32 GetWaterSurfaceHeight(); // 0x006F9A5C
