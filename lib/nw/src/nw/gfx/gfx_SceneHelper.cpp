#include "nw/gfx/gfx_SceneHelper.h"

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_Camera.h"

namespace nw {
namespace gfx {
// 0x0048DA5C | nintendogs:bytes [tier B]
f32 nw::gfx::SceneHelper::CalculateDepth(const nn::math::VEC3& position, const nn::math::MTX34& worldMatrix, const Camera& camera)
{
    nn::math::VEC3 v;
    nn::math::ARMv6::VEC3TransformAsm(&v, &worldMatrix, &position);
    nn::math::ARMv6::VEC3TransformAsm(&v, &camera.m_ViewMatrix, &v);
    const nn::math::MTX44& p = camera.m_ProjectionMatrix;
    const f32 z = p.m[2][1] * v.y + p.m[2][2] * v.z + (p.m[2][3] + p.m[2][0] * v.x);
    const f32 w = p.m[3][1] * v.y + p.m[3][2] * v.z + (p.m[3][3] + p.m[3][0] * v.x);
    const f32 depth = -z / w;
    if (depth > 1.0f) {
        return 1.0f;
    }
    if (depth < 0.0f) {
        return 0.0f;
    }
    return depth;
}

} // namespace gfx
} // namespace nw
