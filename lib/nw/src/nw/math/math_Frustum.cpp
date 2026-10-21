#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/math/math_Frustum.h"

namespace nw {
namespace math {
// 0x004D9D10 (name is ours)
void nw::math::Frustum::Set(f32 fovyDeg, f32 aspect, f32 nearClip, f32 farClip, const nn::math::MTX34& camera)
{
    // the angle index of half the angle of view (256 is a full turn)
    f32 sin;
    f32 cos;
    nn::math::SinCosFIdx(&sin, &cos, fovyDeg * 0.5f * (256.0f / 360.0f));
    f32 tan = sin / cos;
    f32 top = tan * nearClip;
    f32 bottom = -(tan * nearClip);
    f32 right = top * aspect;
    f32 left = -right;
    Set(top, bottom, left, right, nearClip, farClip, camera);
}

// 0x004D9D88 (name is ours)
void nw::math::Frustum::Set(f32 top, f32 bottom, f32 left, f32 right, f32 nearClip, f32 farClip, const nn::math::MTX34& camera)
{
    nn::math::MTX34 invCamera;
    nn::math::ARMv6::MTX34InverseAsm(&invCamera, &camera);
    nn::math::ARMv6::MTX34CopyAsm(&m_Camera, &camera);

    // the corners of the near and the far rectangle in camera space
    f32 ratio = farClip / nearClip;
    nn::math::VEC3 corners[8] = {
        nn::math::VEC3(left, top, -nearClip),
        nn::math::VEC3(right, top, -nearClip),
        nn::math::VEC3(right, bottom, -nearClip),
        nn::math::VEC3(left, bottom, -nearClip),
        nn::math::VEC3(left * ratio, top * ratio, -farClip),
        nn::math::VEC3(right * ratio, top * ratio, -farClip),
        nn::math::VEC3(right * ratio, bottom * ratio, -farClip),
        nn::math::VEC3(left * ratio, bottom * ratio, -farClip),
    };
    nn::math::VEC3 origin(0.0f, 0.0f, 0.0f);
    m_Near = -nearClip;
    m_Far = -farClip;
    m_ViewPlanes[VIEW_PLANE_LEFT].Set(&origin, &corners[3], &corners[0]);
    m_ViewPlanes[VIEW_PLANE_RIGHT].Set(&origin, &corners[1], &corners[2]);
    m_ViewPlanes[VIEW_PLANE_TOP].Set(&origin, &corners[0], &corners[1]);
    m_ViewPlanes[VIEW_PLANE_BOTTOM].Set(&origin, &corners[2], &corners[3]);

    // into world space
    for (s32 i = 0; i < 8; i++) {
        nn::math::ARMv6::VEC3TransformAsm(&corners[i], &invCamera, &corners[i]);
    }
    nn::math::ARMv6::VEC3TransformAsm(&origin, &invCamera, &origin);
    m_Box.Set(corners, 8);
    m_WorldPlanes[WORLD_PLANE_LEFT].Set(&origin, &corners[3], &corners[0]);
    m_WorldPlanes[WORLD_PLANE_RIGHT].Set(&origin, &corners[1], &corners[2]);
    m_WorldPlanes[WORLD_PLANE_NEAR].Set(&corners[0], &corners[1], &corners[2]);
    m_WorldPlanes[WORLD_PLANE_FAR].Set(&corners[4], &corners[7], &corners[6]);
    m_WorldPlanes[WORLD_PLANE_TOP].Set(&origin, &corners[0], &corners[1]);
    m_WorldPlanes[WORLD_PLANE_BOTTOM].Set(&origin, &corners[2], &corners[3]);
}

} // namespace math
} // namespace nw
