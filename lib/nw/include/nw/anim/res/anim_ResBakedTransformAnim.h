#pragma once

#include "decomp.h"
#include "nn/math/math_Quaternion.h"
#include "nn/math/math_Vector3.h"
#include "nw/anim/res/anim_ResAnimCurve.h"
#include "nw/anim/res/anim_ResMemberAnim.h"

namespace nw {
namespace gfx {
class CalculatedTransform;
} // namespace gfx

namespace anim {
namespace res {
// A transform animation baked per frame: the rotation as quaternions, the translation and the
// scale as vectors (names are ours). A constant part is read at frame 0; a missing one keeps the
// original value.
struct ResBakedTransformAnimData
{
    enum Flag
    {
        FLAG_CONSTANT_TRANSLATE = 1 << 0,
        FLAG_CONSTANT_ROTATE = 1 << 1,
        FLAG_CONSTANT_SCALE = 1 << 2,
        FLAG_NOT_EXIST_TRANSLATE = 1 << 3,
        FLAG_NOT_EXIST_ROTATE = 1 << 4,
        FLAG_NOT_EXIST_SCALE = 1 << 5,
    };

    u32 flags;                // 0x00
    ut::Offset path;          // 0x04
    u32 primitiveType;        // 0x08
    ut::Offset rotateCurve;   // 0x0C ResBakedCurveData<QUAT>
    ut::Offset translateCurve; // 0x10 ResBakedCurveData<VEC3_>
    ut::Offset scaleCurve;    // 0x14 ResBakedCurveData<VEC3_>
};
ASSERT_SIZE(ResBakedTransformAnimData, 0x18);

// the baked animation of a gfx::CalculatedTransform
class ResBakedTransformAnim : public ResMemberAnim
{
public:
    // (inline)
    explicit ResBakedTransformAnim(const void* pData = nullptr) : ResMemberAnim(pData) {}
    const ResBakedTransformAnimData* ptr() const { return reinterpret_cast<const ResBakedTransformAnimData*>(m_pData); }

    // the flags the curves gave (the trivial parts) into the transform, with the composite ones
    static void ApplyBakedFlags(gfx::CalculatedTransform* pTransform, u32 flags); // 0x004D5E2C | nintendogs:bytes-fuzzy [tier A]
};
} // namespace res
} // namespace anim
} // namespace nw
