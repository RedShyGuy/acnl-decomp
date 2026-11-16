#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nw/math/math_AABB.h"
#include "nw/math/math_PLANE.h"
#include "nw/math/math_SPHERE.h"

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

    // whether the sphere (in world space) may be inside the view volume: tested against the near and far plane and the four
    // planes through the camera (inline: the copy lies in the area of the inline functions; the name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IntersectSphere(const SPHERE& sphere) const // 0x0074599C
    {
        const nn::math::VEC3& c = sphere.center;
        f32 z = m_Camera.m[2][0] * c.x + m_Camera.m[2][1] * c.y + m_Camera.m[2][2] * c.z + m_Camera.m[2][3];
        if (z - sphere.radius > m_Near) {
            return false;
        }
        if (z + sphere.radius < m_Far) {
            return false;
        }
        f32 x = m_Camera.m[0][0] * c.x + m_Camera.m[0][1] * c.y + m_Camera.m[0][2] * c.z + m_Camera.m[0][3];
        if (sphere.radius < x * m_ViewPlanes[VIEW_PLANE_LEFT].m_Normal.x + z * m_ViewPlanes[VIEW_PLANE_LEFT].m_Normal.z) {
            return false;
        }
        if (sphere.radius < x * m_ViewPlanes[VIEW_PLANE_RIGHT].m_Normal.x + z * m_ViewPlanes[VIEW_PLANE_RIGHT].m_Normal.z) {
            return false;
        }
        f32 y = m_Camera.m[1][0] * c.x + m_Camera.m[1][1] * c.y + m_Camera.m[1][2] * c.z + m_Camera.m[1][3];
        if (sphere.radius < y * m_ViewPlanes[VIEW_PLANE_TOP].m_Normal.y + z * m_ViewPlanes[VIEW_PLANE_TOP].m_Normal.z) {
            return false;
        }
        return y * m_ViewPlanes[VIEW_PLANE_BOTTOM].m_Normal.y + z * m_ViewPlanes[VIEW_PLANE_BOTTOM].m_Normal.z <= sphere.radius;
    }

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
