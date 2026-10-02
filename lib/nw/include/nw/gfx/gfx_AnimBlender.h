#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AnimObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx11AnimBlenderE @ 0x008D0568
// vtable 0x00902374 (vptr 0x0090237C), offset_to_top 0, 9 entries
class AnimBlender : public ::nw::gfx::AnimObject
{
public:
    AnimBlender(); // ctor address unknown
    virtual ~AnimBlender(); // 0x0048C59C slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0048C560 slot 0x04 | virtual slot, introduced by nw::gfx::AnimBlender
    virtual void GetRuntimeTypeInfo() const; // 0x00737868 slot 0x08 | slot vf_0x08 of nw::gfx::AnimBlender
    virtual void TryBind(nw::gfx::AnimGroup*); // 0x0048C554 slot 0x0C | slot vf_0x0C of nw::gfx::AnimBlender
    virtual void vf_0x10(); // 0x0048C548 slot 0x10 | virtual slot, introduced by nw::gfx::AnimBlender
    virtual void UpdateFrame(); // 0x0048C4F0 slot 0x14 | mk7dlp:bytes
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void HasMemberAnim(int) const; // 0x007377F8 slot 0x1C | nintendogs:bytes
    virtual void UpdateCache(); // 0x0048C498 slot 0x20 | mk7dlp:bytes
};
} // namespace gfx
} // namespace nw
