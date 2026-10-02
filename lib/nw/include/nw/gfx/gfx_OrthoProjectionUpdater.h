#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx22OrthoProjectionUpdaterE @ 0x008D0718
// vtable 0x009027DC (vptr 0x009027E4), offset_to_top 0, 7 entries
class OrthoProjectionUpdater : public ::nw::gfx::CameraProjectionUpdater
{
public:
    OrthoProjectionUpdater(); // ctor candidate(s) 0x004A0E08, 0x004A0F24 (unverified)
    virtual void vf_0x00(); // 0x004A11BC slot 0x00 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    virtual void vf_0x04(); // 0x004A1188 slot 0x04 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    virtual void vf_0x08(); // 0x0073B9DC slot 0x08 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    virtual void Update(nn::math::MTX44*, nn::math::MTX34*); // 0x004A0F8C slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x004A0E00 slot 0x10 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    virtual void vf_0x14(); // 0x0073B9D4 slot 0x14 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    virtual void vf_0x18(); // 0x0073B9E8 slot 0x18 | virtual slot, introduced by nw::gfx::OrthoProjectionUpdater
    void Create(nw::os::IAllocator*); // 0x004A0E08 | nintendogs:bytes [tier B]
    void Create(nw::os::IAllocator*, nw::gfx::res::ResOrthoProjectionUpdater); // 0x004A0F24 | nintendogs:callseq-callee [tier A]
};
} // namespace gfx
} // namespace nw
