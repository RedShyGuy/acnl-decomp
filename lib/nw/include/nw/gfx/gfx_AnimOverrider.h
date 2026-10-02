#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AnimBlender.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13AnimOverriderE @ 0x008D05C8
// vtable 0x00902494 (vptr 0x0090249C), offset_to_top 0, 9 entries
class AnimOverrider : public ::nw::gfx::AnimBlender
{
public:
    AnimOverrider(); // ctor candidate(s) 0x004F2AD8 (unverified)
    virtual ~AnimOverrider(); // 0x00492A38 slot 0x00 | slot vf_0x00 of nw::gfx::AnimBlender
    virtual void vf_0x04(); // 0x004929FC slot 0x04 | virtual slot, introduced by nw::gfx::AnimBlender
    virtual void GetRuntimeTypeInfo() const; // 0x00738898 slot 0x08 | slot vf_0x08 of nw::gfx::AnimBlender
    virtual void vf_0x18(); // 0x007388A4 slot 0x18 | virtual slot, introduced by nw::gfx::AnimBlender
};
} // namespace gfx
} // namespace nw
