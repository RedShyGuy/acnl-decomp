#include "nw/anim/res/anim_ResProjectionUpdaterMember.h"

#include "nw/gfx/res/gfx_ResCamera.h"

namespace nw {
namespace anim {
namespace res {
// 0x00744C6C | nintendogs:bytes-fuzzy [tier A]
void nw::anim::res::ResProjectionUpdaterMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResCameraProjectionUpdaterData* pUpdater = static_cast<gfx::res::ResCameraProjectionUpdaterData*>(pTarget);
    switch (pUpdater->typeInfo) {
    case gfx::res::ResPerspectiveProjectionUpdaterData::TYPE: {
        gfx::res::ResPerspectiveProjectionUpdaterData* pPerspective = static_cast<gfx::res::ResPerspectiveProjectionUpdaterData*>(pUpdater);
        const f32 value = *static_cast<const f32*>(pValue);
        switch (GetField()) {
        case 0:
            pPerspective->near = value;
            break;
        case 1:
            pPerspective->far = value;
            break;
        case 2:
            pPerspective->fovy = value;
            break;
        case 3:
            pPerspective->aspectRatio = value;
            break;
        default:
            break;
        }
        break;
    }
    case gfx::res::ResOrthoProjectionUpdaterData::TYPE: {
        gfx::res::ResOrthoProjectionUpdaterData* pOrtho = static_cast<gfx::res::ResOrthoProjectionUpdaterData*>(pUpdater);
        switch (GetField()) {
        case 0:
            pOrtho->near = *static_cast<const f32*>(pValue);
            break;
        case 1:
            pOrtho->far = *static_cast<const f32*>(pValue);
            break;
        case 3:
            pOrtho->aspectRatio = *static_cast<const f32*>(pValue);
            break;
        case 4:
            pOrtho->height = *static_cast<const f32*>(pValue);
            break;
        case 5:
            pOrtho->center = *static_cast<const nn::math::VEC2*>(pValue);
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
