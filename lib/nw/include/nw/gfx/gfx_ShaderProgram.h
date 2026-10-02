#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13ShaderProgramE @ 0x008D061C
// vtable 0x00902554 (vptr 0x0090255C), offset_to_top 0, 2 entries
class ShaderProgram : public ::nw::gfx::GfxObject
{
public:
    ShaderProgram(); // ctor candidate(s) 0x0049777C (unverified)
    virtual void vf_0x00(); // 0x00497804 slot 0x00 | virtual slot, introduced by nw::gfx::ShaderProgram
    virtual void vf_0x04(); // 0x00497800 slot 0x04 | virtual slot, introduced by nw::gfx::ShaderProgram
    void ActivateDescription(nw::gfx::res::ResShaderProgramDescription); // 0x004975B8 | nintendogs:bytes [tier B]
    ShaderProgram(nw::os::IAllocator*); // 0x0049777C | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
