#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20AnimBlendOpTransformE @ 0x008D06F4
// vtable 0x009027B8 (vptr 0x009027C0), offset_to_top 0, 7 entries
class AnimBlendOpTransform : public ::nw::gfx::TransformAnimBlendOp
{
public:
    AnimBlendOpTransform(); // ctor address unknown
    virtual ~AnimBlendOpTransform(); // 0x004A0DF0 slot 0x00 | slot vf_0x00 of nw::gfx::AnimBlendOpTransform
    virtual void vf_0x04(); // 0x004A0DE0 slot 0x04 | virtual slot, introduced by nw::gfx::AnimBlendOpTransform
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0073B0D4 slot 0x08 | nintendogs:bytes
    virtual void PostBlend(nw::anim::AnimResult*, const float*) const; // 0x0073B234 slot 0x0C | nintendogs:callseq
    virtual void vf_0x10(); // 0x0073B160 slot 0x10 | nintendogs:callseq
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x0073B9B4 slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x0073B7B4 slot 0x18 | nintendogs:bytes
};
} // namespace gfx
} // namespace nw
