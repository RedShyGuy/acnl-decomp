#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim16AnimBlendOpFloatE @ 0x008D0BA0
// vtable 0x009035BC (vptr 0x009035C4), offset_to_top 0, 7 entries
class AnimBlendOpFloat : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpFloat(); // ctor address unknown
    virtual void vf_0x00(); // 0x004D5200 slot 0x00 | virtual slot, introduced by nw::anim::AnimBlendOpFloat
    virtual void vf_0x04(); // 0x004D51FC slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpFloat
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x00743504 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void vf_0x10(); // 0x00743550 slot 0x10 | virtual slot, introduced by nw::anim::AnimBlendOpFloat
    virtual void vf_0x14(); // 0x007434F8 slot 0x14 | virtual slot, introduced by nw::anim::AnimBlendOpFloat
    virtual void vf_0x18(); // 0x007434E4 slot 0x18 | virtual slot, introduced by nw::anim::AnimBlendOpFloat
};
} // namespace anim
} // namespace nw
