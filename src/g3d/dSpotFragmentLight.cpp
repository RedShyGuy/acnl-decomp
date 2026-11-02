#include "g3d/dSpotFragmentLight.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "ssys/ma/ma_Math.h"

namespace g3d {
namespace {
// the direction of a new light: (-1, -2, -1) normalized
const f32 DEFAULT_DIRECTION_XZ = -0.408248290f;
const f32 DEFAULT_DIRECTION_Y = -0.816496581f;
// the distance attenuation of a new spot light
const f32 DEFAULT_ATTENUATION_END = 1000.0f;
} // namespace

// 0x004F00B0 slot 0x14 (name is ours)
bool g3d::SpotFragmentLight::Initialize(nw::os::IAllocator* allocator)
{
    if (!FragmentLight::Initialize(allocator)) {
        return false;
    }
    m_ResLight.ptr()->lightKind = nw::gfx::res::ResFragmentLightData::LIGHT_KIND_SPOT;
    nn::math::VEC3 direction(DEFAULT_DIRECTION_XZ, DEFAULT_DIRECTION_Y, DEFAULT_DIRECTION_XZ);
    ssys::ma::NormalizeIfPossible(&direction);
    m_pLight->m_Direction = direction;
    SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
    m_ResLight.SetDistanceAttenuation(0.0f, DEFAULT_ATTENUATION_END);
    return true;
}

// 0x004F0174 slot 0x00
// 0x004F0150 (deleting dtor)
g3d::SpotFragmentLight::~SpotFragmentLight()
{
    // (only the base is destroyed)
}

} // namespace g3d
