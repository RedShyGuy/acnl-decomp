#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim15AnimBlendOpBoolE @ 0x008D0B94
// vtable 0x00903598 (vptr 0x009035A0), offset_to_top 0, 7 entries
class AnimBlendOpBool : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpBool(); // ctor address unknown
    virtual ~AnimBlendOpBool(); // 0x004D518C slot 0x00 | slot vf_0x00 of nw::anim::AnimBlendOpBool
    virtual void vf_0x04(); // 0x004D5188 slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpBool
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x00743480 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x007434D0 slot 0x10 | nintendogs:bytes
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x00743474 slot 0x14 | slot vf_0x14 of nw::anim::AnimBlendOpBool
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x00743460 slot 0x18 | nintendogs:bytes
};
} // namespace anim
} // namespace nw
