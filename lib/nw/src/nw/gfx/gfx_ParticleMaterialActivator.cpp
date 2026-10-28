#include "nw/gfx/gfx_ParticleMaterialActivator.h"

#include "nw/gfx/internal/gfx_ActivateMaterial.h"

#include <new>

namespace nw {
namespace gfx {
// 0x0097F484
const TypeInfo nw::gfx::ParticleMaterialActivator::s_TypeInfo(&IMaterialActivator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FD38 (static initializer)

// 0x004A4390 slot 0x00
// 0x004A438C (deleting dtor)
nw::gfx::ParticleMaterialActivator::~ParticleMaterialActivator()
{
    // empty (in the original too)
}

// 0x004A3B58 slot 0x0C | nintendogs:callseq
void nw::gfx::ParticleMaterialActivator::Activate(RenderContext* pRenderContext, const Material* pMaterial)
{
    using namespace internal::activation;

    // particles use the material resource, without lights; nothing to do for the material sent last
    res::ResMaterial material = pMaterial->GetOriginal();
    const Material* pActive = pRenderContext->m_pActiveMaterial;
    if (pActive != nullptr && pActive->GetResSceneObject().ptr() == pMaterial->GetResSceneObject().ptr()) {
        return;
    }
    const SceneEnvironment& environment = pRenderContext->m_SceneEnvironment;
    const ShaderProgram* pProgram = pRenderContext->m_ShaderProgram.Get();
    RenderContext::MaterialHash hash = pRenderContext->m_MaterialHash;
    res::ResMaterialData* pData = material.ptr();

    // the fog
    u32 fogMode = 0;
    if ((pData->flags & res::ResMaterialData::FLAG_FOG) && environment.m_pFog != nullptr) {
        fogMode = FOG_MODE_FOG;
    }
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = fogMode;
    pCommand[1] = Header(REG_TEXENV_UPDATE_BUFFER, 0x1);
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;

    u32 rasterizationHash = pData->rasterizationHash;
    if (rasterizationHash == 0 || hash.m_RasterizationHash != rasterizationHash) {
        PutRasterization(pData);
        hash.m_RasterizationHash = rasterizationHash;
    }

    u32 coordinatorsHash = pData->textureCoordinatorsHash;
    if (hash.m_TextureCoordinatorsHash != coordinatorsHash) {
        internal::MaterialState::ActivateParticleTextureCoordinators(pRenderContext, pProgram, material);
        hash.m_TextureCoordinatorsHash = coordinatorsHash;
    }

    // the first texture mapper alone
    u32 mappersHash = pData->textureMappersHash;
    if (mappersHash == 0 || hash.m_TextureMappersHash != mappersHash) {
        const res::ResPixelBasedTextureMapperData* pMapper = pData->textureMappers[0].To<res::ResPixelBasedTextureMapperData>();
        if (pMapper != nullptr) {
            std::memcpy(nn::gx::detail::s_pCommandBufferCurrent, pMapper->commands, pMapper->commandSize);
            nn::gx::detail::s_pCommandBufferCurrent += pMapper->commandSize / sizeof(u32);
        }
        pCommand = nn::gx::detail::s_pCommandBufferCurrent;
        pCommand[0] = 1 | TEXUNIT_CONFIG_CLEAR_CACHE;
        pCommand[1] = Header(REG_TEXUNIT_CONFIG, 0x5);
        nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
        hash.m_TextureMappersHash = mappersHash;
    }

    u32 colorHash = pData->materialColorHash;
    bool isColorChanged = colorHash == 0 || hash.m_MaterialColorHash != colorHash;
    res::ResFragmentShaderData* pShader = pData->fragmentShader.To<res::ResFragmentShaderData>();
    u32 combinersHash = pData->textureCombinersHash;
    if (combinersHash == 0 || hash.m_TextureCombinersHash != combinersHash || isColorChanged) {
        PutTextureCombiners(pShader, pData->materialColor);
        hash.m_TextureCombinersHash = combinersHash;
    }

    PutCommand(pData->fragmentShader.To<res::ResFragmentShaderData>()->bufferCommand, 2);

    u32 operationHash = pData->fragmentOperationHash;
    if (operationHash == 0 || hash.m_FragmentOperationHash != operationHash) {
        PutFragmentOperation(pData);
        hash.m_FragmentOperationHash = operationHash;
    }

    pRenderContext->m_MaterialHash = hash;
}

// 0x004A3B20 | nintendogs [tier C]
nw::gfx::ParticleMaterialActivator* nw::gfx::ParticleMaterialActivator::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(ParticleMaterialActivator), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) ParticleMaterialActivator(pAllocator);
}

} // namespace gfx
} // namespace nw
