#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResMemberAnim.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResTexture.h"

namespace nw {
namespace anim {
namespace res {
// the binary resource of a texture pattern animation (the class name and Setup are from the
// reference symbols)
class ResTextureAnim
{
public:
    // (inline)
    explicit ResTextureAnim(const void* pData = nullptr) : m_pData(static_cast<const ResTextureAnimData*>(pData)) {}
    const ResTextureAnimData* ptr() const { return m_pData; }
    s32 GetTextureCount() const { return m_pData->textureCount; }
    // (inline) the referenced texture at index
    gfx::res::ResReferenceTextureData* GetTexture(int index) const
    {
        return const_cast<gfx::res::ResReferenceTextureData*>(
            static_cast<const gfx::res::ResReferenceTextureData*>(m_pData->textures.To<ut::Offset>()[index].ToPtr()));
    }

    // the textures resolved by name
    u32 Setup(os::IAllocator* pAllocator, gfx::res::ResGraphicsFile graphicsFile); // 0x004D547C | nintendogs:bytes [tier A]

    const ResTextureAnimData* m_pData; // 0x0
};
ASSERT_SIZE(ResTextureAnim, 0x4);
} // namespace res
} // namespace anim
} // namespace nw
