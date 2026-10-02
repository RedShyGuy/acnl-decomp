#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20TransformAnimBlendOpE @ 0x008D0700
class TransformAnimBlendOp : public ::nw::anim::AnimBlendOp
{
public:
    TransformAnimBlendOp(); // ctor address unknown
    void BlendTranslate(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const; // 0x0073B2D8 | nintendogs:bytes [tier A]
    void BlendRotateMatrix(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const; // 0x0073B364 | nintendogs:bytes [tier A]
    void OverrideTransform(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, unsigned) const; // 0x0073B448 | nintendogs:bytes [tier B]
    void BlendScaleAccurate(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const; // 0x0073B668 | nintendogs:bytes [tier A]
    void BlendScaleStandard(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const; // 0x0073B71C | nintendogs:bytes [tier A]
    void BlendRotateQuaternion(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const; // 0x0073B7D4 | nintendogs:bytes [tier A]
    void PostBlendAccurateScale(nw::gfx::CalculatedTransform*) const; // 0x0073B970 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
