#pragma once

#include "decomp.h"
#include "g3d/dBaseLight.h"
#include "g3d/dResourceLoader.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace gfx {
class FragmentLight;
} // namespace gfx
} // namespace nw

namespace g3d {
// a fragment light: made from a graphics file of its own, from a resource or with default values (names are ours)
// RTTI N3g3d13FragmentLightE @ 0x008D0CB8
// vtable 0x009039E0 (vptr 0x009039E8), offset_to_top 0, 7 entries
class FragmentLight : public ::g3d::BaseLight
{
public:
    FragmentLight(); // 0x004EDA84
    virtual ~FragmentLight(); // 0x004EDADC slot 0x00
    // 0x004EDAB8 (deleting dtor)
    void Update() override; // 0x004EDA80 slot 0x08
    // destroys the light and cleans the file up
    void Finalize() override; // 0x004EDA64 slot 0x0C
    // the light resource of a graphics file (the first light of the file); false if it could not be loaded
    virtual bool Load(const sead::SafeStringBase<char>& path, ssys::ma::Allocator* allocator); // 0x004ED620 slot 0x10
    // the light of the loaded resource, else a white light (ambient 0.2, diffuse 0.8) in the memory of allocator
    // (NULL: the one of the engine); always true
    virtual bool Initialize(nw::os::IAllocator* allocator); // 0x004ED8F4 slot 0x14
    // the light of a resource; always true
    virtual bool Create(nw::gfx::res::ResFragmentLight resource, nw::os::IAllocator* allocator); // 0x004ED834 slot 0x18

    // (all always true)
    bool SetPosition(const nn::math::VEC3& position); // 0x004ED6CC
    bool SetDirection(const nn::math::VEC3& direction); // 0x004ED6DC
    bool SetAmbient(const nw::ut::FloatColor& color); // 0x004ED720
    bool SetDiffuse(const nw::ut::FloatColor& color); // 0x004ED754
    bool SetSpecular0(const nw::ut::FloatColor& color); // 0x004ED788
    bool SetSpecular1(const nw::ut::FloatColor& color); // 0x004ED7BC
    bool SetDistanceAttenuationEnd(f32 end); // 0x004ED7F0
    bool SetDistanceAttenuationStart(f32 start); // 0x004ED814
    DECOMP_EMIT_COPY nw::gfx::res::ResFragmentLight& GetResLight() { return m_ResLight; } // 0x004EDA5C

    ResourceLoader m_ResourceLoader;                 // 0x00C
    nw::gfx::res::ResFragmentLight m_ResLight;       // 0x114
    nw::gfx::FragmentLight* m_pLight;                // 0x118
};
ASSERT_SIZE(FragmentLight, 0x11C);
} // namespace g3d
