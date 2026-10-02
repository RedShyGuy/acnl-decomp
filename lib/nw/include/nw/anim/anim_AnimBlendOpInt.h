#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim14AnimBlendOpIntE @ 0x008D0B88
// vtable 0x00903574 (vptr 0x0090357C), offset_to_top 0, 7 entries
class AnimBlendOpInt : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpInt(); // ctor address unknown
    virtual void vf_0x00(); // 0x004D5184 slot 0x00 | virtual slot, introduced by nw::anim::AnimBlendOpInt
    virtual void vf_0x04(); // 0x004D5180 slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpInt
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0074341C slot 0x08 | slot vf_0x08 of nw::anim::AnimBlendOpTexture
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void vf_0x10(); // 0x0074344C slot 0x10 | virtual slot, introduced by nw::anim::AnimBlendOpInt
    virtual void vf_0x14(); // 0x00743440 slot 0x14 | virtual slot, introduced by nw::anim::AnimBlendOpInt
    virtual void vf_0x18(); // 0x0074342C slot 0x18 | virtual slot, introduced by nw::anim::AnimBlendOpInt
};
} // namespace anim
} // namespace nw
