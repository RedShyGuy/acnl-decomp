#include "nw/anim/res/anim_ResVertexLightMember.h"

#include <cmath>

#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace anim {
namespace res {
namespace {
// (inline) the vector scaled to length 1 (name is ours)
inline nn::math::VEC3 Normalize(const nn::math::VEC3& v)
{
    const f32 scale = 1.0f / std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return nn::math::VEC3(v.x * scale, v.y * scale, v.z * scale);
}
} // namespace

// 0x007446B0 | nintendogs:bytes-fuzzy [tier A]
void nw::anim::res::ResVertexLightMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResVertexLightData* pLight = static_cast<gfx::res::ResVertexLightData*>(pTarget);
    switch (GetField()) {
    case 0:
        pLight->ambient = *static_cast<const ut::FloatColor*>(pValue);
        break;
    case 1:
        pLight->diffuse = *static_cast<const ut::FloatColor*>(pValue);
        break;
    case 2:
        pLight->direction = Normalize(*static_cast<const nn::math::VEC3*>(pValue));
        break;
    case 3:
        pLight->distanceAttenuationConstant = *static_cast<const f32*>(pValue);
        break;
    case 4:
        pLight->distanceAttenuationLinear = *static_cast<const f32*>(pValue);
        break;
    case 5:
        pLight->distanceAttenuationQuadratic = *static_cast<const f32*>(pValue);
        break;
    case 6:
        pLight->spotExponent = *static_cast<const f32*>(pValue);
        break;
    case 7:
        pLight->spotCutoffAngle = *static_cast<const f32*>(pValue);
        break;
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
