#include "nw/gfx/res/gfx_ResGraphicsFile.h"

#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/gfx/res/gfx_ResLookupTableSet.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResTexture.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// (inline) the animations of a dictionary
inline void CleanupAnims(const ResDicInfo& anims)
{
    for (int i = 0; i < anims.count; ++i) {
        anim::res::ResAnim anim(anims.GetData(i));
        if (anim.IsValid()) {
            anim.Cleanup();
        }
    }
}
} // namespace

// 0x00131E5C | nintendogs:bytes [tier A]
void nw::gfx::res::ResGraphicsFile::Cleanup()
{
    // the reverse order of Setup
    for (int i = 0; i < m_pData->models.count; ++i) {
        ResModel model(m_pData->models.GetData(i));
        if (model.IsValid()) {
            model.Cleanup();
        }
    }
    CleanupAnims(m_pData->lightAnims);
    CleanupAnims(m_pData->cameraAnims);
    CleanupAnims(m_pData->visibilityAnims);
    CleanupAnims(m_pData->textureAnims);
    CleanupAnims(m_pData->skeletalAnims);
    for (int i = 0; i < m_pData->textures.count; ++i) {
        ResTexture texture(m_pData->textures.GetData(i));
        if (texture.IsValid()) {
            texture.Cleanup();
        }
    }
    for (int i = 0; i < m_pData->shaders.count; ++i) {
        ResShader shader(m_pData->shaders.GetData(i));
        if (shader.IsValid()) {
            shader.Cleanup();
        }
    }
    for (int i = 0; i < m_pData->fogs.count; ++i) {
        ResFog fog(m_pData->fogs.GetData(i));
        if (fog.IsValid()) {
            fog.Cleanup();
        }
    }
    for (int i = 0; i < m_pData->lights.count; ++i) {
        ResLight light(m_pData->lights.GetData(i));
        if (light.IsValid()) {
            light.Cleanup();
        }
    }
    for (int i = 0; i < m_pData->luts.count; ++i) {
        ResLookupTableSet luts(m_pData->luts.GetData(i));
        if (luts.IsValid()) {
            luts.Cleanup();
        }
    }
}

// 0x004A6D94 | nintendogs:callseq [tier A]
u32 nw::gfx::res::ResGraphicsFile::Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    for (int i = 0; i < m_pData->luts.count; ++i) {
        result |= ResLookupTableSet(m_pData->luts.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->lights.count; ++i) {
        result |= ResLight(m_pData->lights.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->fogs.count; ++i) {
        result |= ResFog(m_pData->fogs.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->shaders.count; ++i) {
        result |= ResShader(m_pData->shaders.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->textures.count; ++i) {
        result |= ResTexture(m_pData->textures.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->skeletalAnims.count; ++i) {
        result |= anim::res::ResAnim(m_pData->skeletalAnims.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->textureAnims.count; ++i) {
        result |= anim::res::ResAnim(m_pData->textureAnims.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->visibilityAnims.count; ++i) {
        result |= anim::res::ResAnim(m_pData->visibilityAnims.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->cameraAnims.count; ++i) {
        result |= anim::res::ResAnim(m_pData->cameraAnims.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->lightAnims.count; ++i) {
        result |= anim::res::ResAnim(m_pData->lightAnims.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    for (int i = 0; i < m_pData->models.count; ++i) {
        result |= ResModel(m_pData->models.GetData(i)).Setup(pAllocator, graphicsFile);
    }
    return result;
}

// 0x004A6CEC (name is ours)
void* nw::gfx::res::ResGraphicsFile::GetTextures(int index) const
{
    return ptr()->textures.GetData(index);
}

// 0x004A6D24 (name is ours)
void* nw::gfx::res::ResGraphicsFile::GetSkeletalAnims(const char* name) const
{
    return ptr()->skeletalAnims.GetData(name);
}

} // namespace res
} // namespace gfx
} // namespace nw
