#include "nw/anim/res/anim_ResHemiSphereLightMember.h"

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

// 0x00744BE0 | nintendogs:bytes [tier A]
void nw::anim::res::ResHemiSphereLightMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResHemiSphereLightData* pLight = static_cast<gfx::res::ResHemiSphereLightData*>(pTarget);
    switch (GetField()) {
    case 0:
        pLight->groundColor = *static_cast<const ut::FloatColor*>(pValue);
        break;
    case 1:
        pLight->skyColor = *static_cast<const ut::FloatColor*>(pValue);
        break;
    case 2:
        pLight->direction = Normalize(*static_cast<const nn::math::VEC3*>(pValue));
        break;
    case 3:
        pLight->lerpFactor = *static_cast<const f32*>(pValue);
        break;
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
