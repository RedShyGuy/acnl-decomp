#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx28TransformAnimBlendOpStandardE @ 0x008D07A8
// vtable 0x0090294C (vptr 0x00902954), offset_to_top 0, 7 entries
class TransformAnimBlendOpStandard : public ::nw::gfx::TransformAnimBlendOp
{
public:
    TransformAnimBlendOpStandard(); // ctor address unknown
    virtual ~TransformAnimBlendOpStandard(); // 0x004A4774 slot 0x00 | slot vf_0x00 of nw::gfx::TransformAnimBlendOpStandard
    virtual void vf_0x04(); // 0x004A4770 slot 0x04 | virtual slot, introduced by nw::gfx::TransformAnimBlendOpStandard
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0073C5CC slot 0x08 | nintendogs:callseq
    virtual void PostBlend(nw::anim::AnimResult*, const float*) const; // 0x0073C624 slot 0x0C | slot vf_0x0C of nw::gfx::TransformAnimBlendOpStandard
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x0073C61C slot 0x10 | slot vf_0x10 of nw::gfx::TransformAnimBlendOpStandard
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x0073B9B4 slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x0073B7B4 slot 0x18 | nintendogs:bytes
};
} // namespace gfx
} // namespace nw
