#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nw/math/math_AABB.h"
#include "nw/math/math_PLANE.h"

namespace nw {
namespace math {
// The view volume of a camera: the planes in camera and in world space and the box around it.
// The code lies behind PLANE::Set in the math object; the game sets it from its cameras. The
// class, function and member names are ours.
class Frustum
{
public:
    // the planes in camera space (through the camera position)
    enum ViewPlane
    {
        VIEW_PLANE_LEFT,
        VIEW_PLANE_RIGHT,
        VIEW_PLANE_TOP,
        VIEW_PLANE_BOTTOM,
        VIEW_PLANE_COUNT
    };
    // the planes in world space
    enum WorldPlane
    {
        WORLD_PLANE_LEFT,
        WORLD_PLANE_RIGHT,
        WORLD_PLANE_NEAR,
        WORLD_PLANE_FAR,
        WORLD_PLANE_TOP,
        WORLD_PLANE_BOTTOM,
        WORLD_PLANE_COUNT
    };

    // a perspective view: the vertical angle of view in degrees and the aspect ratio
    void Set(f32 fovyDeg, f32 aspect, f32 nearClip, f32 farClip, const nn::math::MTX34& camera); // 0x004D9D10 (name is ours)
    // the near rectangle (top, bottom, left, right at nearClip)
    void Set(f32 top, f32 bottom, f32 left, f32 right, f32 nearClip, f32 farClip, const nn::math::MTX34& camera); // 0x004D9D88 (name is ours)

    nn::math::MTX34 m_Camera;                  // 0x00
    PLANE m_ViewPlanes[VIEW_PLANE_COUNT];      // 0x30
    f32 m_Near;                                // 0x70 (-nearClip: the z of the near plane in camera space)
    f32 m_Far;                                 // 0x74 (-farClip)
    AABB m_Box;                                // 0x78 (the corners in world space)
    PLANE m_WorldPlanes[WORLD_PLANE_COUNT];    // 0x90
};
ASSERT_OFFSET(Frustum, m_ViewPlanes, 0x30);
ASSERT_OFFSET(Frustum, m_Box, 0x78);
ASSERT_OFFSET(Frustum, m_WorldPlanes, 0x90);
ASSERT_SIZE(Frustum, 0xF0);
} // namespace math
} // namespace nw
