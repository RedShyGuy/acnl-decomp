#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx24FrustumProjectionUpdaterE @ 0x008D0760
// vtable 0x0090288C (vptr 0x00902894), offset_to_top 0, 7 entries
class FrustumProjectionUpdater : public ::nw::gfx::CameraProjectionUpdater
{
public:
    FrustumProjectionUpdater(); // ctor candidate(s) 0x004A384C (unverified)
    virtual void vf_0x00(); // 0x004A3AD8 slot 0x00 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    virtual void vf_0x04(); // 0x004A3AA4 slot 0x04 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    virtual void vf_0x08(); // 0x0073C4B8 slot 0x08 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    virtual void Update(nn::math::MTX44*, nn::math::MTX34*); // 0x004A38B4 slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x004A3844 slot 0x10 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    virtual void vf_0x14(); // 0x0073C4B0 slot 0x14 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    virtual void vf_0x18(); // 0x0073C4C4 slot 0x18 | virtual slot, introduced by nw::gfx::FrustumProjectionUpdater
    void Create(nw::os::IAllocator*, nw::gfx::res::ResFrustumProjectionUpdater); // 0x004A384C | nintendogs:callseq-callee [tier A]
};
} // namespace gfx
} // namespace nw
