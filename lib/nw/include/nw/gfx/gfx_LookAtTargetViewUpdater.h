#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23LookAtTargetViewUpdaterE @ 0x008D073C
// vtable 0x0090283C (vptr 0x00902844), offset_to_top 0, 7 entries
class LookAtTargetViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    LookAtTargetViewUpdater(); // ctor candidate(s) 0x004A1D30, 0x004A1E08 (unverified)
    virtual void vf_0x00(); // 0x004A204C slot 0x00 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    virtual void vf_0x04(); // 0x004A2018 slot 0x04 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    virtual void vf_0x08(); // 0x0073C484 slot 0x08 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    virtual void Update(nn::math::MTX34*, const nn::math::MTX34&, const nn::math::VEC3&); // 0x004A1E54 slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x004A1D28 slot 0x10 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    virtual void vf_0x14(); // 0x0073C47C slot 0x14 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    virtual void vf_0x18(); // 0x0073C490 slot 0x18 | virtual slot, introduced by nw::gfx::LookAtTargetViewUpdater
    void Create(nw::os::IAllocator*); // 0x004A1D30 | nintendogs:bytes [tier A]
    void Create(nw::os::IAllocator*, nw::gfx::res::ResLookAtTargetViewUpdater); // 0x004A1E08 | nintendogs:callseq-callee [tier A]
};
} // namespace gfx
} // namespace nw
