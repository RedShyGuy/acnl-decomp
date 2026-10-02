#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx28TransformAnimBlendOpAccScaleE @ 0x008D079C
// vtable 0x00902928 (vptr 0x00902930), offset_to_top 0, 7 entries
class TransformAnimBlendOpAccScale : public ::nw::gfx::TransformAnimBlendOp
{
public:
    TransformAnimBlendOpAccScale(); // ctor address unknown
    virtual ~TransformAnimBlendOpAccScale(); // 0x004A476C slot 0x00 | slot vf_0x00 of nw::gfx::TransformAnimBlendOpAccScale
    virtual void vf_0x04(); // 0x004A4768 slot 0x04 | virtual slot, introduced by nw::gfx::TransformAnimBlendOpAccScale
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0073C554 slot 0x08 | nintendogs:callseq
    virtual void PostBlend(nw::anim::AnimResult*, const float*) const; // 0x0073C5AC slot 0x0C | nintendogs:callseq
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x0073C5A4 slot 0x10 | slot vf_0x10 of nw::gfx::TransformAnimBlendOpAccScale
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x0073B9B4 slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x0073B7B4 slot 0x18 | nintendogs:bytes
};
} // namespace gfx
} // namespace nw
