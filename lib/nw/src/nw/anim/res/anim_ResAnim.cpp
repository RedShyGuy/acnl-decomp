#include "nw/anim/res/anim_ResAnim.h"

#include "nw/anim/res/anim_ResTextureAnim.h"

namespace nw {
namespace anim {
namespace res {
// 0x001385DC | nintendogs:bytes [tier A]
void nw::anim::res::ResAnim::Cleanup()
{
    for (int i = 0; i < GetMemberAnimCount(); ++i) {
        ResMemberAnim memberAnim = GetMemberAnim(i);
        if (memberAnim.GetPrimitiveType() != PRIMITIVE_TEXTURE) {
            continue;
        }
        ResTextureAnim textureAnim(memberAnim.ptr());
        for (int j = 0; j < textureAnim.GetTextureCount(); ++j) {
            textureAnim.GetTexture(j)->target.m_Offset = 0;
        }
    }
}

// 0x004D6124 | nintendogs:bytes [tier A]
u32 nw::anim::res::ResAnim::Setup(os::IAllocator* pAllocator, gfx::res::ResGraphicsFile graphicsFile)
{
    u32 result = gfx::RESULT_SUCCESS;
    for (int i = 0; i < GetMemberAnimCount(); ++i) {
        ResMemberAnim memberAnim = GetMemberAnim(i);
        result |= memberAnim.GetPrimitiveType() == PRIMITIVE_TEXTURE ? ResTextureAnim(memberAnim.ptr()).Setup(pAllocator, graphicsFile) : 0;
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
