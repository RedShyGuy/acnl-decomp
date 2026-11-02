#include "g3d/dDirectionalFragmentLight.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "ssys/ma/ma_Math.h"

namespace g3d {
namespace {
// the direction of a new light: (-1, -2, -1) normalized
const f32 DEFAULT_DIRECTION_XZ = -0.408248290f;
const f32 DEFAULT_DIRECTION_Y = -0.816496581f;
} // namespace

// 0x004F0EA0 slot 0x14 (name is ours)
bool g3d::DirectionalFragmentLight::Initialize(nw::os::IAllocator* allocator)
{
    if (!FragmentLight::Initialize(allocator)) {
        return false;
    }
    m_ResLight.ptr()->lightKind = nw::gfx::res::ResFragmentLightData::LIGHT_KIND_DIRECTIONAL;
    nn::math::VEC3 direction(DEFAULT_DIRECTION_XZ, DEFAULT_DIRECTION_Y, DEFAULT_DIRECTION_XZ);
    ssys::ma::NormalizeIfPossible(&direction);
    m_pLight->m_Direction = direction;
    return true;
}

// 0x004F0F28 slot 0x00
// 0x004F0F04 (deleting dtor)
g3d::DirectionalFragmentLight::~DirectionalFragmentLight()
{
    // (only the base is destroyed)
}

} // namespace g3d
