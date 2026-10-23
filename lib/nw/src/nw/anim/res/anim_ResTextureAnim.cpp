#include "nw/anim/res/anim_ResTextureAnim.h"

namespace nw {
namespace anim {
namespace res {
// 0x004D547C | nintendogs:bytes [tier A]
u32 nw::anim::res::ResTextureAnim::Setup(os::IAllocator* /*pAllocator*/, gfx::res::ResGraphicsFile graphicsFile)
{
    u32 result = gfx::RESULT_SUCCESS;
    for (int i = 0; i < GetTextureCount(); ++i) {
        if (GetTexture(i)->target.ToPtr() != nullptr) {
            continue;
        }
        gfx::res::ReferenceTarget<gfx::res::ResTexture> target =
            gfx::res::GetReferenceTextureTarget(gfx::res::ResReferenceTexture(GetTexture(i)), graphicsFile);
        if (!target.isFound) {
            result |= gfx::res::RESULT_TEXTURE_NOT_FOUND;
        }
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
