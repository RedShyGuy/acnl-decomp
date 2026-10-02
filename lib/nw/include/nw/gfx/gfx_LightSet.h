#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx8LightSetE @ 0x008D07FC
// vtable 0x00902A84 (vptr 0x00902A8C), offset_to_top 0, 3 entries
class LightSet : public ::nw::gfx::GfxObject
{
public:
    LightSet(); // ctor candidate(s) 0x004AEA3C, 0x004AEC1C (unverified)
    virtual ~LightSet(); // 0x004AEE60 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004AEDF0 slot 0x04 | virtual slot, introduced by nw::gfx::LightSet
    virtual void vf_0x08(); // 0x0073C87C slot 0x08 | virtual slot, introduced by nw::gfx::LightSet
};
} // namespace gfx
} // namespace nw
