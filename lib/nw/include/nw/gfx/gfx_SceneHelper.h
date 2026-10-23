#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace gfx {
class Camera;

// Helpers of the scene updaters (the class name and CalculateDepth are from the reference symbols).
class SceneHelper
{
public:
    // the depth of a point in the clip space of the camera, 0 to 1
    static f32 CalculateDepth(const nn::math::VEC3& position, const nn::math::MTX34& worldMatrix, const Camera& camera); // 0x0048DA5C | nintendogs:bytes [tier B]
};
} // namespace gfx
} // namespace nw
