#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim18AnimBlendOpVector2E @ 0x008D0BB8
// vtable 0x00903604 (vptr 0x0090360C), offset_to_top 0, 7 entries
class AnimBlendOpVector2 : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpVector2(); // ctor address unknown
    virtual ~AnimBlendOpVector2(); // 0x004D5210 slot 0x00 | slot vf_0x00 of nw::anim::AnimBlendOpVector2
    virtual void vf_0x04(); // 0x004D520C slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpVector2
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x00743260 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x007435DC slot 0x10 | nintendogs:bytes
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x00743218 slot 0x14 | slot vf_0x14 of nw::anim::AnimBlendOpVector2
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x007433B8 slot 0x18 | slot vf_0x18 of nw::anim::AnimBlendOpVector2
};
} // namespace anim
} // namespace nw
