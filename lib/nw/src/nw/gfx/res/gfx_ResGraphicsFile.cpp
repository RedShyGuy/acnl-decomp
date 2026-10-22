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
// 0x00131E5C | nintendogs:bytes [tier A]
void nw::gfx::res::ResGraphicsFile::Cleanup()
{
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

} // namespace res
} // namespace gfx
} // namespace nw
