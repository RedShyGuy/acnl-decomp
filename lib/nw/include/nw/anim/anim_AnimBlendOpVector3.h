#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim18AnimBlendOpVector3E @ 0x008D0BC4
// vtable 0x00903628 (vptr 0x00903630), offset_to_top 0, 7 entries
class AnimBlendOpVector3 : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpVector3(); // ctor address unknown
    virtual ~AnimBlendOpVector3(); // 0x004D5218 slot 0x00 | slot vf_0x00 of nw::anim::AnimBlendOpVector3
    virtual void vf_0x04(); // 0x004D5214 slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpVector3
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x00743604 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x00743618 slot 0x10 | nintendogs:bytes
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x007435FC slot 0x14 | slot vf_0x14 of nw::anim::AnimBlendOpVector3
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x007435F4 slot 0x18 | slot vf_0x18 of nw::anim::AnimBlendOpVector3
};
} // namespace anim
} // namespace nw
