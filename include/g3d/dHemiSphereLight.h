#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace gfx {
class HemiSphereLight;
} // namespace gfx
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace g3d {
// a hemisphere light (names are ours)
// RTTI N3g3d15HemiSphereLightE @ 0x008D0CF8
// vtable 0x00903A6C (vptr 0x00903A74), offset_to_top 0, 5 entries
class HemiSphereLight : public ::g3d::BaseLight
{
public:
    HemiSphereLight(); // 0x004EF260
    virtual ~HemiSphereLight(); // 0x004EF2A4 slot 0x00
    // 0x004EF294 (deleting dtor)
    void Update() override; // 0x004EF25C slot 0x08
    // a light from above (ground 0.2, sky 0.8, lerp 0.5) in the memory of allocator (NULL: the one of the engine);
    // always true
    virtual bool Initialize(nw::os::IAllocator* allocator); // 0x004EF178 slot 0x10

    // (all always true)
    bool SetSkyColor(const nw::ut::FloatColor& color); // 0x004EF0A8
    bool SetDirection(const nn::math::VEC3& direction); // 0x004EF0C0
    bool SetLerpFactor(f32 lerpFactor); // 0x004EF150
    bool SetGroundColor(const nw::ut::FloatColor& color); // 0x004EF160
    DECOMP_EMIT_COPY nw::gfx::res::ResHemiSphereLight& GetResLight() { return m_ResLight; } // 0x004EF254

    nw::gfx::res::ResHemiSphereLight m_ResLight;   // 0x0C
    nw::gfx::HemiSphereLight* m_pLight;            // 0x10
};
ASSERT_SIZE(HemiSphereLight, 0x14);
} // namespace g3d
