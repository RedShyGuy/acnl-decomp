#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CameraViewUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17RotateViewUpdaterE @ 0x008D06B8
// vtable 0x0090274C (vptr 0x00902754), offset_to_top 0, 7 entries
class RotateViewUpdater : public ::nw::gfx::CameraViewUpdater
{
public:
    RotateViewUpdater(); // ctor candidate(s) 0x0049E138 (unverified)
    virtual void vf_0x00(); // 0x0049E998 slot 0x00 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    virtual void vf_0x04(); // 0x0049E964 slot 0x04 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    virtual void vf_0x08(); // 0x0073A88C slot 0x08 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    virtual void Update(nn::math::MTX34*, const nn::math::MTX34&, const nn::math::VEC3&); // 0x0049E184 slot 0x0C | nintendogs:bytes
    virtual void vf_0x10(); // 0x0049E130 slot 0x10 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    virtual void vf_0x14(); // 0x0073A884 slot 0x14 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    virtual void vf_0x18(); // 0x0073A898 slot 0x18 | virtual slot, introduced by nw::gfx::RotateViewUpdater
    void Create(nw::os::IAllocator*, nw::gfx::res::ResRotateViewUpdater); // 0x0049E138 | nintendogs:callseq-callee [tier A]
};
} // namespace gfx
} // namespace nw
