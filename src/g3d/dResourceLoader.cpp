#include "g3d/dResourceLoader.h"
#include <cstring>
#include "g3d/g3d_System.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResShader.h"

namespace g3d {
namespace {
// the shader whose materials get the shader program of their texture coordinators
const char* const DEFAULT_SHADER_NAME = "DefaultShader";
// the bit of RESULT_SHADER_NOT_FOUND in the setup result
const u32 SHADER_NOT_FOUND_BIT = nw::gfx::res::RESULT_SHADER_NOT_FOUND & ~nw::gfx::RESULT_FAILURE_MASK;
} // namespace

// 0x002F7210 (name is ours)
void g3d::ResourceLoader::Unload()
{
    Cleanup();
    m_LoadSplit.Reset();
}

// 0x00317C38 | libgarden
bool g3d::ResourceLoader::LoadAsync(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    bool result = m_LoadSplit.Read(path, heap, alignment);
    if (result && m_pBuffer == NULL) {
        m_pBuffer = m_LoadSplit.m_pData;
        m_GraphicsFile = nw::gfx::res::ResGraphicsFile(m_LoadSplit.m_pData);
    }
    return result;
}

// 0x00317C70 | libgarden
void g3d::ResourceLoader::InitializeCgfx(nw::os::IAllocator* allocator, ResourceLoader* additional, bool setupSystemShaders)
{
    nw::gfx::res::ResGraphicsFile graphicsFile = (additional != NULL) ? additional->m_GraphicsFile : m_GraphicsFile;
    {
        graphic::CurrentSetter setter(m_pHeaps[2], m_pHeaps[0], m_pHeaps[1]);
        m_SetupResult = m_GraphicsFile.Setup(allocator, graphicsFile);
    }
    const nw::gfx::res::ResGraphicsFileData* file = m_GraphicsFile.ptr();
    for (s32 i = 0; i < file->models.count; i++) {
        const nw::gfx::res::ResModelData* model = static_cast<const nw::gfx::res::ResModelData*>(file->models.GetData(i));
        for (s32 j = 0; j < model->materials.count; j++) {
            nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(j));
            if (model->typeInfo != nw::gfx::res::ResModelData::TYPE && model->typeInfo != nw::gfx::res::ResSkeletalModelData::TYPE) {
                continue;
            }
            const nw::gfx::res::ResReferenceShaderData* shader = static_cast<const nw::gfx::res::ResReferenceShaderData*>(
                nw::gfx::res::DynamicCastData<nw::gfx::res::ResShaderData>(material->shader.ToPtr(),
                                                                            nw::gfx::res::ResShader::TYPE_REFERENCE));
            if (shader == NULL || std::strcmp(shader->path.To<char>(), DEFAULT_SHADER_NAME) != 0) {
                continue;
            }
            // (image 0x0090CD4C)
            const s32 programs[4] = {3, 2, 1, 0};
            material->shaderProgramDescriptionIndex = programs[material->textureCoordinatorCount];
        }
    }
    if (setupSystemShaders && (m_SetupResult & SHADER_NOT_FOUND_BIT)) {
        m_GraphicsFile.Setup(allocator, s_pShaderResource->m_GraphicsFile);
    }
    ResolveLookupTables(false);
}

// 0x004EB42C | libgarden
void g3d::ResourceLoader::Read(u32 textureArea, u32 vertexArea, u32 indexArea)
{
    for (u32 i = 0; i < static_cast<u32>(m_GraphicsFile.ptr()->models.count); i++) {
        SetupModelBuffers(static_cast<const nw::gfx::res::ResModelData*>(m_GraphicsFile.ptr()->models.GetData(i)), vertexArea,
                          indexArea);
    }
    for (u32 i = 0; i < static_cast<u32>(m_GraphicsFile.ptr()->textures.count); i++) {
        SetupTexture(static_cast<const nw::gfx::res::ResTextureData*>(m_GraphicsFile.ptr()->textures.GetData(i)), textureArea);
    }
}

// 0x004EB570 (name is ours)
void g3d::ResourceLoader::ResolveLookupTables(bool resolveAll)
{
    for (s32 i = 0; i < m_GraphicsFile.ptr()->models.count; i++) {
        const nw::gfx::res::ResModelData* model = static_cast<const nw::gfx::res::ResModelData*>(m_GraphicsFile.ptr()->models.GetData(i));
        for (s32 j = 0; j < model->materials.count; j++) {
            nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(j));
            nw::gfx::res::ResFragmentShaderData* fragmentShader = material->fragmentShader.To<nw::gfx::res::ResFragmentShaderData>();
            if (fragmentShader == NULL) {
                continue;
            }
            nw::gfx::res::ResFragmentLightingTableData* table =
                fragmentShader->lightingTable.To<nw::gfx::res::ResFragmentLightingTableData>();
            if (table == NULL) {
                continue;
            }
            for (s32 k = 0; k < 6; k++) {
                ResolveLookupTable(table->samplers[k].To<nw::gfx::res::ResLightingLookupTableData>(), resolveAll);
            }
        }
    }
}

// 0x004EEBBC (name is ours)
bool g3d::ResourceLoader::IsLoadFailed()
{
    if (!m_LoadSplit.IsFinished()) {
        return false;
    }
    return !m_LoadSplit.IsSucceeded();
}

// 0x004EEBE4 | libgarden
bool g3d::ResourceLoader::Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    bool result = m_LoadSplit.Load(path, heap, alignment);
    if (result && m_pBuffer == NULL) {
        m_pBuffer = m_LoadSplit.m_pData;
        m_GraphicsFile = nw::gfx::res::ResGraphicsFile(m_LoadSplit.m_pData);
    }
    return result;
}

// 0x004EEC1C | libgarden
g3d::ResourceLoader::ResourceLoader()
{
    // (only the base and the members are constructed)
}

// 0x00318014 slot 0x00
// 0x004EEC5C (deleting dtor)
g3d::ResourceLoader::~ResourceLoader()
{
    // (only the members and the base are destroyed)
}

} // namespace g3d
