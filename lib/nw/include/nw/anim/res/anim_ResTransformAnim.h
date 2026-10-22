#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResMemberAnim.h"

namespace nw {
namespace anim {
namespace res {
// The curves of a transform: scale x, y, z, rotation x, y, z (radians), one unused, translation x, y, z
// (names are ours). Component i is constant with bit 6 + i and not animated with bit 16 + i: then
// the original value is used.
struct ResTransformAnimData
{
    enum Flag
    {
        CONSTANT_SHIFT = 6,
        NOT_EXIST_SHIFT = 16,
    };
    enum Component
    {
        COMPONENT_SCALE_X = 0,
        COMPONENT_ROTATE_X = 3,
        COMPONENT_TRANSLATE_X = 7,
    };

    u32 flags;                       // 0x00
    ut::Offset path;                 // 0x04
    u32 primitiveType;               // 0x08
    ResAnimComponent components[10]; // 0x0C
};
ASSERT_SIZE(ResTransformAnimData, 0x34);

// the animation of a gfx::CalculatedTransform
class ResTransformAnim : public ResMemberAnim
{
public:
    // (inline)
    explicit ResTransformAnim(const void* pData = nullptr) : ResMemberAnim(pData) {}
    const ResTransformAnimData* ptr() const { return reinterpret_cast<const ResTransformAnimData*>(m_pData); }

    // pTarget: a gfx::CalculatedTransform that is built anew; pOriginalValue: the nn::math::Transform3
    // of the components that are not animated; returns result

    u32 EvaluateResult(void* pTarget, u32 result, f32 frame, const void* pOriginalValue) const;
};
} // namespace res
} // namespace anim
} // namespace nw
