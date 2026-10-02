#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20AimTargetViewUpdaterE @ 0x008D06E8
// vtable 0x00902794 (vptr 0x0090279C), offset_to_top 0, 7 entries
class AimTargetViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    AimTargetViewUpdater(); // ctor candidate(s) 0x004A0A50 (unverified)
    virtual void vf_0x00(); // 0x004A0DA0 slot 0x00 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    virtual void vf_0x04(); // 0x004A0D6C slot 0x04 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    virtual void vf_0x08(); // 0x0073B0C0 slot 0x08 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    virtual void Update(nn::math::MTX34*, const nn::math::MTX34&, const nn::math::VEC3&); // 0x004A0A9C slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x004A0A48 slot 0x10 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    virtual void vf_0x14(); // 0x0073B0B8 slot 0x14 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    virtual void vf_0x18(); // 0x0073B0CC slot 0x18 | virtual slot, introduced by nw::gfx::AimTargetViewUpdater
    void Create(nw::os::IAllocator*, nw::gfx::res::ResAimTargetViewUpdater); // 0x004A0A50 | nintendogs:callseq-callee [tier A]
};
} // namespace gfx
} // namespace nw
