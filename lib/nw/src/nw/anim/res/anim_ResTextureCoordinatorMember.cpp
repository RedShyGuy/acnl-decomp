#include "nw/anim/res/anim_ResTextureCoordinatorMember.h"

#include "nw/gfx/res/gfx_ResTextureCoordinatorData.h"

namespace nw {
namespace anim {
namespace res {
// 0x00744D5C | nintendogs:bytes [tier A]
void nw::anim::res::ResTextureCoordinatorMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResTextureCoordinatorData* pCoordinator = static_cast<gfx::res::ResTextureCoordinatorData*>(pTarget);
    switch (GetField()) {
    case 0: {
        const nn::math::VEC2& scale = *static_cast<const nn::math::VEC2*>(pValue);
        if (pCoordinator->scale.x != scale.x || pCoordinator->scale.y != scale.y) {
            pCoordinator->scale = scale;
            pCoordinator->isDirty = true;
        }
        break;
    }
    case 1: {
        const f32 rotate = *static_cast<const f32*>(pValue);
        if (pCoordinator->rotate != rotate) {
            pCoordinator->rotate = rotate;
            pCoordinator->isDirty = true;
        }
        break;
    }
    case 2: {
        const nn::math::VEC2& translate = *static_cast<const nn::math::VEC2*>(pValue);
        if (pCoordinator->translate.x != translate.x || pCoordinator->translate.y != translate.y) {
            pCoordinator->translate = translate;
            pCoordinator->isDirty = true;
        }
        break;
    }
    default:
        break;
    }
    // (the matrix is made again in any case)
    pCoordinator->isDirty = true;
}

} // namespace res
} // namespace anim
} // namespace nw
