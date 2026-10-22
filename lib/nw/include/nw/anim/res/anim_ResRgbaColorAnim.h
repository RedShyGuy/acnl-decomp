#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResMemberAnim.h"

namespace nw {
namespace anim {
namespace res {
class ResRgbaColorAnim : public ResMemberAnim
{
public:
    // (inline)
    explicit ResRgbaColorAnim(const void* pData = nullptr) : ResMemberAnim(pData) {}
    const ResRgbaColorAnimData* ptr() const { return reinterpret_cast<const ResRgbaColorAnimData*>(m_pData); }

    // writes the animated components at the frame to the target; returns result with their bits
    u32 EvaluateResult(void* pTarget, u32 result, f32 frame) const;
};
} // namespace res
} // namespace anim
} // namespace nw
