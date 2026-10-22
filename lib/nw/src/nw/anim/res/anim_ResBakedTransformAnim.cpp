#include "nw/anim/res/anim_ResBakedTransformAnim.h"

#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace anim {
namespace res {
// 0x004D5E2C | nintendogs:bytes-fuzzy [tier A]
void nw::anim::res::ResBakedTransformAnim::ApplyBakedFlags(gfx::CalculatedTransform* pTransform, u32 flags)
{
    const u32 FLAGS_ROTATE_TRANSLATE_ZERO = gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO | gfx::CalculatedTransform::FLAG_IS_TRANSLATE_ZERO;
    const u32 FLAGS_IDENTITY = gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO | gfx::CalculatedTransform::FLAG_IS_SCALE_ONE;
    const u32 FLAGS_TRIVIAL = gfx::CalculatedTransform::FLAG_IS_IDENTITY | gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO |
                              gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO | gfx::CalculatedTransform::FLAG_IS_TRANSLATE_ZERO |
                              gfx::CalculatedTransform::FLAG_IS_SCALE_ONE | gfx::CalculatedTransform::FLAG_IS_UNIFORM_SCALE;

    u32 trivialFlags = flags & ~(gfx::CalculatedTransform::FLAG_IS_IDENTITY | gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO);
    if ((flags & FLAGS_ROTATE_TRANSLATE_ZERO) == FLAGS_ROTATE_TRANSLATE_ZERO) {
        trivialFlags |= gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO;
    }
    // (the given flags, not the ones just computed)
    if ((flags & FLAGS_IDENTITY) == FLAGS_IDENTITY) {
        trivialFlags |= gfx::CalculatedTransform::FLAG_IS_IDENTITY;
    }
    pTransform->m_Flags = (pTransform->m_Flags & ~FLAGS_TRIVIAL) | trivialFlags;
}

} // namespace res
} // namespace anim
} // namespace nw
