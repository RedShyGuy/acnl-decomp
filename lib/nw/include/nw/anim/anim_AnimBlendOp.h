#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim11AnimBlendOpE @ 0x008D0B80
class AnimBlendOp
{
public:
    AnimBlendOp(); // ctor address unknown
    void ApplyFloatVector(void*, const nw::anim::AnimResult*, int) const; // 0x00743220 | nintendogs:bytes [tier A]
    void BlendFloatVector(nw::anim::AnimResult*, const nw::anim::AnimResult*, float, int) const; // 0x00743274 | nintendogs:bytes [tier A]
    void OverrideFloatVector(nw::anim::AnimResult*, const nw::anim::AnimResult*, int, unsigned) const; // 0x007432F8 | nintendogs:bytes [tier B]
    void ConvertFloatVectorToAnimResult(nw::anim::AnimResult*, const void*, int) const; // 0x007433C0 | nintendogs:bytes [tier A]
};
} // namespace anim
} // namespace nw
