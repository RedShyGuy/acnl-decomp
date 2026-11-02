#include "g3d/dPointFragmentLight.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "ssys/ma/ma_Math.h"

namespace g3d {
// 0x004F0904 slot 0x14 (name is ours)
bool g3d::PointFragmentLight::Initialize(nw::os::IAllocator* allocator)
{
    if (!FragmentLight::Initialize(allocator)) {
        return false;
    }
    m_ResLight.ptr()->lightKind = nw::gfx::res::ResFragmentLightData::LIGHT_KIND_POINT;
    SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
    return true;
}

// 0x004F0978 slot 0x00
// 0x004F0954 (deleting dtor)
g3d::PointFragmentLight::~PointFragmentLight()
{
    // (only the base is destroyed)
}

} // namespace g3d
