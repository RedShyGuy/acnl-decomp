#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim18AnimBlendOpTextureE @ 0x008D0BAC
// vtable 0x009035E0 (vptr 0x009035E8), offset_to_top 0, 7 entries
class AnimBlendOpTexture : public ::nw::anim::AnimBlendOp
{
public:
    AnimBlendOpTexture(); // ctor address unknown
    virtual void vf_0x00(); // 0x004D5208 slot 0x00 | virtual slot, introduced by nw::anim::AnimBlendOpTexture
    virtual void vf_0x04(); // 0x004D5204 slot 0x04 | virtual slot, introduced by nw::anim::AnimBlendOpTexture
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0074341C slot 0x08 | slot vf_0x08 of nw::anim::AnimBlendOpTexture
    virtual void vf_0x0C(); // 0x00743424 slot 0x0C | slot vf_0x0C of nw::anim::AnimBlendOpBool
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x007435B0 slot 0x10 | nintendogs:bytes
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x00743590 slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x00743564 slot 0x18 | nintendogs:bytes
};
} // namespace anim
} // namespace nw
