#include "nw/anim/anim_AnimBlendOp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// ctor address unknown
nw::gfx::TransformAnimBlendOp::TransformAnimBlendOp()
{
}

// 0x0073B2D8 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendTranslate(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const
{
}

// 0x0073B364 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendRotateMatrix(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const
{
}

// 0x0073B448 | nintendogs:bytes [tier B]
void nw::gfx::TransformAnimBlendOp::OverrideTransform(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, unsigned) const
{
}

// 0x0073B668 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendScaleAccurate(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const
{
}

// 0x0073B71C | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendScaleStandard(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const
{
}

// 0x0073B7D4 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::BlendRotateQuaternion(nw::gfx::CalculatedTransform*, const nw::gfx::CalculatedTransform*, float) const
{
}

// 0x0073B970 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimBlendOp::PostBlendAccurateScale(nw::gfx::CalculatedTransform*) const
{
}

} // namespace gfx
} // namespace nw
