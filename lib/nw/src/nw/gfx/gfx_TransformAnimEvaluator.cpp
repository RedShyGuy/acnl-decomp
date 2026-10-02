#include "nw/gfx/gfx_BaseAnimEvaluator.h"
#include "nw/anim/res/anim_ResTransformAnim.h"
#include "nw/anim/res/anim_ResBakedTransformAnim.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_TransformAnimEvaluator.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004A1A68 (unverified)
nw::gfx::TransformAnimEvaluator::TransformAnimEvaluator()
{
}

// 0x004A1C2C slot 0x00 | slot vf_0x00 of nw::gfx::BaseAnimEvaluator
nw::gfx::TransformAnimEvaluator::~TransformAnimEvaluator()
{
}

// 0x004A1B40 slot 0x04 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::vf_0x04()
{
}

// 0x0073C190 slot 0x08 | slot vf_0x08 of nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::GetRuntimeTypeInfo() const
{
}

// 0x004A1A28 slot 0x0C | nintendogs:bytes
void nw::gfx::TransformAnimEvaluator::TryBind(nw::gfx::AnimGroup*)
{
}

// 0x0073C474 slot 0x18 | slot vf_0x18 of nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::GetResult(void*, int) const
{
}

// 0x0073B9F0 slot 0x1C | nintendogs:bytes
void nw::gfx::TransformAnimEvaluator::HasMemberAnim(int) const
{
}

// 0x004A15D8 slot 0x20 | nintendogs:bytes
void nw::gfx::TransformAnimEvaluator::UpdateCache()
{
}

// 0x004A11FC slot 0x24 | mk7dlp:bytes
void nw::gfx::TransformAnimEvaluator::ChangeAnim(nw::anim::res::ResAnim)
{
}

// 0x0073BA44 slot 0x28 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::vf_0x28()
{
}

// 0x0073C464 slot 0x2C | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::vf_0x2C()
{
}

// 0x004A1664 slot 0x30 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
void nw::gfx::TransformAnimEvaluator::vf_0x30()
{
}

// 0x004A0058 | nintendogs:callgraph [tier A]
void nw::gfx::TransformAnimEvaluator::UpdateFlags(nw::gfx::CalculatedTransform*) const
{
}

// 0x004A1814 | nintendogs:callgraph [tier A]
void nw::gfx::TransformAnimEvaluator::ResetNoAnimMember(nw::gfx::AnimGroup*, nw::anim::res::ResAnim)
{
}

// 0x0073BA54 | nintendogs:bytes [tier A]
void nw::gfx::TransformAnimEvaluator::GetResultCommon(void*, int, bool) const
{
}

// 0x0073BBE4 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::TransformAnimEvaluator::EvaluateMemberAnim(nw::gfx::CalculatedTransform*, nw::anim::res::ResTransformAnim, float, const nn::math::Transform3*, bool) const
{
}

// 0x0073C19C | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::TransformAnimEvaluator::EvaluateMemberBakedAnim(nw::gfx::CalculatedTransform*, nw::anim::res::ResBakedTransformAnim, float, const nn::math::Transform3*, bool) const
{
}

} // namespace gfx
} // namespace nw
