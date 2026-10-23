#include "nw/anim/res/anim_ResFragmentLightMember.h"

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

// 0x00744780 | nintendogs:callseq [tier A]
void nw::anim::res::ResFragmentLightMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResFragmentLight light(pTarget);
    gfx::res::ResFragmentLightData* pLight = light.ptr();
    switch (GetField()) {
    case 0:
        pLight->ambient = *static_cast<const ut::FloatColor*>(pValue);
        pLight->ambientU8 = pLight->ambient.ToPicaU32();
        break;
    case 1:
        pLight->diffuse = *static_cast<const ut::FloatColor*>(pValue);
        pLight->diffuseU8 = pLight->diffuse.ToPicaU32();
        break;
    case 2:
        pLight->specular0 = *static_cast<const ut::FloatColor*>(pValue);
        pLight->specular0U8 = pLight->specular0.ToPicaU32();
        break;
    case 3:
        pLight->specular1 = *static_cast<const ut::FloatColor*>(pValue);
        pLight->specular1U8 = pLight->specular1.ToPicaU32();
        break;
    case 4:
        pLight->direction = Normalize(*static_cast<const nn::math::VEC3*>(pValue));
        break;
    case 5:
        pLight->distanceAttenuationStart = *static_cast<const f32*>(pValue);
        light.SetDistanceAttenuation(pLight->distanceAttenuationStart, pLight->distanceAttenuationEnd);
        break;
    case 6:
        pLight->distanceAttenuationEnd = *static_cast<const f32*>(pValue);
        light.SetDistanceAttenuation(pLight->distanceAttenuationStart, pLight->distanceAttenuationEnd);
        break;
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
