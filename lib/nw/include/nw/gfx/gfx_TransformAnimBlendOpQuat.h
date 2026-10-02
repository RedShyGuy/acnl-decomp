#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx24TransformAnimBlendOpQuatE @ 0x008D076C
// vtable 0x009028B0 (vptr 0x009028B8), offset_to_top 0, 7 entries
class TransformAnimBlendOpQuat : public ::nw::gfx::TransformAnimBlendOp
{
public:
    TransformAnimBlendOpQuat(); // ctor address unknown
    virtual ~TransformAnimBlendOpQuat(); // 0x004A3B1C slot 0x00 | slot vf_0x00 of nw::gfx::TransformAnimBlendOpQuat
    virtual void vf_0x04(); // 0x004A3B18 slot 0x04 | virtual slot, introduced by nw::gfx::TransformAnimBlendOpQuat
    virtual void Blend(nw::anim::AnimResult*, float*, const nw::anim::AnimResult*, const float*) const; // 0x0073C4CC slot 0x08 | nintendogs:callseq
    virtual void PostBlend(nw::anim::AnimResult*, const float*) const; // 0x0073C51C slot 0x0C | slot vf_0x0C of nw::gfx::TransformAnimBlendOpQuat
    virtual void Override(nw::anim::AnimResult*, const nw::anim::AnimResult*) const; // 0x0073B440 slot 0x10 | slot vf_0x10 of nw::gfx::TransformAnimBlendOpQuat
    virtual void Apply(void*, const nw::anim::AnimResult*) const; // 0x0073B9B4 slot 0x14 | nintendogs:bytes
    virtual void ConvertToAnimResult(nw::anim::AnimResult*, const void*) const; // 0x0073B7B4 slot 0x18 | nintendogs:bytes
};
} // namespace gfx
} // namespace nw
