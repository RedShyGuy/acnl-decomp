#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// ctor address unknown
nw::anim::AnimBlendOp::AnimBlendOp()
{
}

// 0x00743220 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOp::ApplyFloatVector(void*, const nw::anim::AnimResult*, int) const
{
}

// 0x00743274 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOp::BlendFloatVector(nw::anim::AnimResult*, const nw::anim::AnimResult*, float, int) const
{
}

// 0x007432F8 | nintendogs:bytes [tier B]
void nw::anim::AnimBlendOp::OverrideFloatVector(nw::anim::AnimResult*, const nw::anim::AnimResult*, int, unsigned) const
{
}

// 0x007433C0 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOp::ConvertFloatVectorToAnimResult(nw::anim::AnimResult*, const void*, int) const
{
}

} // namespace anim
} // namespace nw
