#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimEvaluator.h"

namespace g3d {
// RTTI N3g3d27MorphTransformAnimEvaluatorE @ 0x008D0D48
// vtable 0x00903B58 (vptr 0x00903B60), offset_to_top 0, 13 entries
class MorphTransformAnimEvaluator : public ::nw::gfx::TransformAnimEvaluator
{
public:
    MorphTransformAnimEvaluator(); // ctor address unknown
    virtual ~MorphTransformAnimEvaluator(); // 0x004F1018 slot 0x00 | slot vf_0x00 of nw::gfx::BaseAnimEvaluator
    virtual void vf_0x04(); // 0x004F1014 slot 0x04 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void GetRuntimeTypeInfo() const; // 0x00747458 slot 0x08 | slot vf_0x08 of nw::gfx::BaseAnimEvaluator
    virtual void TryBind(nw::gfx::AnimGroup*); // 0x004F0FF4 slot 0x0C | slot vf_0x0C of nw::gfx::BaseAnimEvaluator
    virtual void UpdateFrame(); // 0x004F0F48 slot 0x14 | slot vf_0x14 of nw::gfx::BaseAnimEvaluator
    virtual void GetResult(void*, int) const; // 0x00747464 slot 0x18 | slot vf_0x18 of nw::gfx::BaseAnimEvaluator
};
} // namespace g3d
