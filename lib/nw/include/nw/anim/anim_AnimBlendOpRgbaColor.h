#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim20AnimBlendOpRgbaColorE @ 0x008D0BD8
// vtable 0x0090365C (vptr 0x00903664), offset_to_top 0, 7 entries
class AnimBlendOpRgbaColor : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpRgbaColor(); // ctor address unknown
    virtual ~AnimBlendOpRgbaColor(); // 0x004D5268 slot 0x00 | slot vf_0x00 of nw::anim::AnimBlendOpRgbaColor
    virtual void vf_0x04(); // 0x004D5264 slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpRgbaColor
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x007436E4 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x00743768 slot 0x10 | nintendogs:bytes
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x0074368C slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x00743630 slot 0x18 | nintendogs:bytes
};
} // namespace anim
} // namespace nw
