#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23SceneEnvironmentSettingE @ 0x008D0748
// vtable 0x00902860 (vptr 0x00902868), offset_to_top 0, 3 entries
class SceneEnvironmentSetting : public ::nw::gfx::SceneObject
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    SceneEnvironmentSetting(); // ctor candidate(s) 0x004A2888 (unverified)
    virtual void vf_0x00(); // 0x004A2934 slot 0x00 | virtual slot, introduced by nw::gfx::SceneEnvironmentSetting
    virtual void vf_0x04(); // 0x004A2928 slot 0x04 | virtual slot, introduced by nw::gfx::SceneEnvironmentSetting
    virtual void vf_0x08(); // 0x0073C498 slot 0x08 | virtual slot, introduced by nw::gfx::SceneEnvironmentSetting
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, nw::gfx::res::ResSceneEnvironmentSetting, nw::gfx::SceneEnvironmentSetting::Description); // 0x004A208C | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
