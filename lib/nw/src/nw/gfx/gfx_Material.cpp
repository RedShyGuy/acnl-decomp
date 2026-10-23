#include "nw/gfx/gfx_Material.h"

#include <new>

#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"

namespace nw {
namespace gfx {
// 0x0097F478
const TypeInfo nw::gfx::Material::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x00796750 (static initializer)

// 0x004AEECC | nintendogs:callgraph [tier A]
u32 nw::gfx::Material::CreateBuffers(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    const u32 bufferOption = m_pOwner->GetBufferOption();
    if (m_BufferCount != 0) {
        void* pBuffer = pAllocator->Alloc(m_BufferCount * sizeof(res::ResMaterialData*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<res::ResMaterialData*> buffers(pBuffer, m_BufferCount, pAllocator);
            m_Buffers = buffers;
        }
        for (int i = 0; i < m_BufferCount; ++i) {
            if (result & RESULT_FAILURE_MASK) {
                break;
            }
            CopyResult<res::ResMaterial> copy = CopyResMaterial(pAllocator, bufferOption);
            if (copy.result & RESULT_FAILURE_MASK) {
                DestroyResMaterial(pAllocator, copy.resource);
                result |= RESULT_FAILURE_MASK;
            } else {
                m_Buffers.PushBackFast(copy.resource.ptr());
            }
        }
    }
    return result;
}

// 0x004AF064 | nintendogs:callseq [tier A]
nw::gfx::Material::CopyResult<nw::gfx::res::ResMaterial> nw::gfx::Material::CopyResMaterial(os::IAllocator* pAllocator, u32 bufferOption)
{
    const res::ResMaterialData* pOriginal = GetOriginal().ptr();
    CopyResult<res::ResMaterial> copy;
    void* pMemory = pAllocator->Alloc(sizeof(res::ResMaterialData), 4);
    if (pMemory == nullptr) {
        copy.resource = res::ResMaterial(nullptr);
        copy.result = RESULT_FAILURE_MASK;
        return copy;
    }
    res::ResMaterialData* pCopy = new (pMemory) res::ResMaterialData(*GetOriginal().ptr());

    // the parts that are not copied: no name, no objects of the original, the hashes reset
    pCopy->name.Set(nullptr);
    pCopy->shader.Set(nullptr);
    pCopy->userUniforms.count = 0;
    pCopy->userUniforms.array.Set(nullptr);
    pCopy->textureMappers[0].Set(nullptr);
    pCopy->textureMappers[1].Set(nullptr);
    pCopy->textureMappers[2].Set(nullptr);
    pCopy->proceduralTextureMapper.Set(nullptr);
    pCopy->fragmentShader.Set(nullptr);
    pCopy->userDataCount = 0;
    pCopy->userDataDic.Set(nullptr);
    pCopy->combinerBufferHash = 0;
    pCopy->fragmentLightingHash = 0;
    pCopy->fragmentLightingTableHash = 0;
    pCopy->fragmentLightingTableHashSeed = 0;
    pCopy->fragmentOperationHash = 0;
    pCopy->materialColorHash = 0;
    pCopy->rasterizationHash = 0;
    pCopy->userUniformsHash = 0;
    pCopy->shadingHash = 0;
    pCopy->textureCombinersHash = 0;
    pCopy->textureCoordinatorsHash = 0;
    pCopy->textureMappersHash = 0;
    pCopy->textureSamplersHash = 0;

    if (bufferOption & BUFFER_USER_UNIFORMS) {
        pCopy->userUniforms.count = pOriginal->userUniforms.count;
        ut::Offset* pUniforms = nullptr;
        if (pOriginal->userUniforms.count != 0) {
            pUniforms = static_cast<ut::Offset*>(pAllocator->Alloc(pOriginal->userUniforms.count * sizeof(ut::Offset), 4));
            if (pUniforms == nullptr) {
                goto failure;
            }
            pCopy->userUniforms.array.Set(pUniforms);
        }
        for (int i = 0; i < pOriginal->userUniforms.count; ++i) {
            pUniforms[i].Set(nullptr);
        }
        for (int i = 0; i < pOriginal->userUniforms.count; ++i) {
            const res::ResUserUniformData* pUniform =
                static_cast<const res::ResUserUniformData*>(pOriginal->userUniforms.GetData(i));
            const int valueCount = res::ResUserUniformData::GetValueCount(pUniform->type);
            void* pUniformMemory = pAllocator->Alloc(res::USER_UNIFORM_HEADER_SIZE + valueCount * sizeof(f32), 4);
            if (pUniformMemory == nullptr) {
                goto failure;
            }
            res::ResUserUniformData* pUniformCopy = new (pUniformMemory) res::ResUserUniformData(*pUniform);
            for (int k = 0; k < valueCount; ++k) {
                pUniformCopy->values[k] = pUniform->values[k];
            }
            pUniformCopy->name = pUniform->name;
            pUniforms[i].Set(pUniformCopy);
        }
    }

    if (bufferOption & BUFFER_TEXTURE_MAPPERS) {
        for (int i = 0; i < 3; ++i) {
            res::ResTextureMapper mapper(pOriginal->textureMappers[i].ToPtr());
            if (mapper.IsValid()) {
                res::ResTextureMapper clone = mapper.CloneDynamic(pAllocator);
                if (!clone.IsValid()) {
                    goto failure;
                }
                pCopy->textureMappers[i].Set(clone.ptr());
            }
        }
    }

    if (bufferOption & BUFFER_PROCEDURAL_TEXTURE_MAPPER) {
        res::ResTextureMapper mapper(pOriginal->proceduralTextureMapper.ToPtr());
        if (mapper.IsValid()) {
            res::ResTextureMapper clone = mapper.CloneDynamic(pAllocator);
            if (!clone.IsValid()) {
                goto failure;
            }
            pCopy->proceduralTextureMapper.Set(clone.ptr());
        }
    }

    if (bufferOption & BUFFER_FRAGMENT_SHADER) {
        const res::ResFragmentShaderData* pShader = pOriginal->fragmentShader.To<res::ResFragmentShaderData>();
        if (pShader != nullptr) {
            void* pShaderMemory = pAllocator->Alloc(sizeof(res::ResFragmentShaderData), 4);
            if (pShaderMemory == nullptr) {
                goto failure;
            }
            res::ResFragmentShaderData* pShaderCopy = new (pShaderMemory) res::ResFragmentShaderData(*pShader);
            pCopy->fragmentShader.Set(pShaderCopy);
            pShaderCopy->lightingTable.Set(nullptr);
            if (bufferOption & BUFFER_FRAGMENT_LIGHTING_TABLE) {
                res::ResFragmentLightingTable table(pShader->lightingTable.ToPtr());
                if (table.ptr() != nullptr) {
                    CopyResult<res::ResFragmentLightingTable> tableCopy = CopyResFragmentLightingTable(pAllocator, table);
                    pShaderCopy->lightingTable.Set(tableCopy.resource.ptr());
                    if (tableCopy.result & RESULT_FAILURE_MASK) {
                        goto failure;
                    }
                }
            }
        }
    }

    copy.resource = res::ResMaterial(pCopy);
    copy.result = RESULT_SUCCESS;
    return copy;

failure:
    copy.resource = res::ResMaterial(pCopy);
    copy.result = RESULT_FAILURE_MASK;
    return copy;
}

// 0x004AF5C4 | nintendogs:bytes [tier A]
void nw::gfx::Material::DestroyResMaterial(os::IAllocator* pAllocator, res::ResMaterial resource)
{
    if (!resource.IsValid()) {
        return;
    }
    resource.Cleanup();
    DestroyResFragmentShader(pAllocator, res::ResFragmentShader(resource.ptr()->fragmentShader.ToPtr()));
    if (resource.ptr()->proceduralTextureMapper.ToPtr() != nullptr) {
        res::ResTextureMapper mapper(resource.ptr()->proceduralTextureMapper.ToPtr());
        mapper.DestroyDynamic();
    }
    for (int i = 0; i < 3; ++i) {
        if (resource.ptr()->textureMappers[i].ToPtr() != nullptr) {
            res::ResPixelBasedTextureMapper mapper(resource.ptr()->textureMappers[i].ToPtr());
            mapper.DestroyDynamic();
        }
    }
    if (resource.ptr()->userUniforms.array.ToPtr() != nullptr) {
        for (int i = 0; i < resource.ptr()->userUniforms.count; ++i) {
            void* pUniform = resource.ptr()->userUniforms.GetData(i);
            if (pUniform != nullptr) {
                pAllocator->Free(pUniform);
            }
        }
        pAllocator->Free(resource.ptr()->userUniforms.array.ToPtr());
    }
    pAllocator->Free(resource.ptr());
}

// 0x004AF74C (name is ours)
void* nw::gfx::Material::GetAnimTargetObject(const anim::res::ResAnimGroupMember& member, res::ResMaterial resource) const
{
    anim::res::ResMaterialMemberData* pMember =
        const_cast<anim::res::ResMaterialMemberData*>(static_cast<const anim::res::ResMaterialMemberData*>(member.ptr()));
    res::ResMaterialData* pMaterial = resource.ptr();
    switch (pMember->memberType) {
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MATERIAL_COLOR:
        pMember->pObject = pMaterial;
        return &pMaterial->materialColor;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER:
        pMember->pObject = pMaterial;
        return pMaterial->textureMappers[pMember->index].ToPtr();
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER:
        pMember->pObject = pMaterial;
        return pMaterial->textureMappers[pMember->index].ToPtr();
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_BLEND_OPERATION:
        pMember->pObject = pMaterial;
        return &pMaterial->fragmentOperationMode;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_COORDINATOR:
        pMember->pObject = pMaterial;
        return &pMaterial->textureCoordinators[pMember->index];
    default:
        return nullptr;
    }
}

// 0x004AF818 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Material::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResMaterial resource, int bufferCount, u32 bufferOption)
{
    pSize->Add(sizeof(Material));
    pSize->Add(sizeof(res::ResMaterialData*) * bufferCount);
    if (bufferCount <= 0) {
        return;
    }

    // one buffer
    os::MemorySizeCalculator size(pSize->GetAlignment());
    size.Add(sizeof(res::ResMaterialData));
    if (bufferOption & BUFFER_USER_UNIFORMS) {
        const int count = resource.ptr()->userUniforms.count;
        size.Add(count * sizeof(ut::Offset));
        for (int i = 0; i < count; ++i) {
            const res::ResUserUniformData* pUniform =
                static_cast<const res::ResUserUniformData*>(resource.ptr()->userUniforms.GetData(i));
            size.Add(res::USER_UNIFORM_HEADER_SIZE + res::ResUserUniformData::GetValueCount(pUniform->type) * sizeof(f32));
        }
    }
    if (bufferOption & BUFFER_TEXTURE_MAPPERS) {
        for (int i = 0; i < 3; ++i) {
            res::ResTextureMapper mapper(resource.ptr()->textureMappers[i].ToPtr());
            if (mapper.IsValid()) {
                mapper.GetMemorySizeForCloneInternal(&size);
            }
        }
    }
    if (bufferOption & BUFFER_PROCEDURAL_TEXTURE_MAPPER) {
        res::ResTextureMapper mapper(resource.ptr()->proceduralTextureMapper.ToPtr());
        if (mapper.IsValid()) {
            mapper.GetMemorySizeForCloneInternal(&size);
        }
    }
    if (bufferOption & BUFFER_FRAGMENT_SHADER) {
        const res::ResFragmentShaderData* pShader = resource.ptr()->fragmentShader.To<res::ResFragmentShaderData>();
        if (pShader != nullptr) {
            size.Add(sizeof(res::ResFragmentShaderData));
            if (bufferOption & BUFFER_FRAGMENT_LIGHTING_TABLE) {
                const res::ResFragmentLightingTableData* pTable = pShader->lightingTable.To<res::ResFragmentLightingTableData>();
                if (pTable != nullptr) {
                    size.Add(sizeof(res::ResFragmentLightingTableData));
                    for (int i = 0; i < 6; ++i) {
                        if (pTable->samplers[i].ToPtr() != nullptr) {
                            size.Add(sizeof(res::ResLightingLookupTableData));
                            size.Add(sizeof(res::ResReferenceLookupTableData));
                        }
                    }
                }
            }
        }
    }
    size *= bufferCount;
    pSize->Add(size);
}

// 0x004AFC78 | nintendogs:callgraph [tier A]
void nw::gfx::Material::DestroyResFragmentShader(os::IAllocator* pAllocator, res::ResFragmentShader shader)
{
    if (!shader.IsValid()) {
        return;
    }
    res::ResFragmentLightingTableData* pTable = shader.ptr()->lightingTable.To<res::ResFragmentLightingTableData>();
    if (pTable != nullptr) {
        for (int i = 0; i < 6; ++i) {
            if (pTable->samplers[i].ToPtr() != nullptr) {
                pAllocator->Free(pTable->samplers[i].To<res::ResLightingLookupTableData>()->sampler.ToPtr());
                pAllocator->Free(pTable->samplers[i].ToPtr());
            }
        }
        pAllocator->Free(pTable);
    }
    pAllocator->Free(shader.ptr());
}

// 0x004AFF70 | nintendogs:bytes [tier A]
nw::gfx::Material::CopyResult<nw::gfx::res::ResLightingLookupTable>
nw::gfx::Material::CopyResLightingLookupTable(os::IAllocator* pAllocator, res::ResLightingLookupTable table)
{
    CopyResult<res::ResLightingLookupTable> copy;
    void* pMemory = pAllocator->Alloc(sizeof(res::ResLightingLookupTableData), 4);
    if (pMemory == nullptr) {
        copy.resource = res::ResLightingLookupTable(nullptr);
        copy.result = RESULT_FAILURE_MASK;
        return copy;
    }
    res::ResLightingLookupTableData* pCopy = new (pMemory) res::ResLightingLookupTableData(*table.ptr());
    void* pSamplerMemory = pAllocator->Alloc(sizeof(res::ResReferenceLookupTableData), 4);
    if (pSamplerMemory == nullptr) {
        pAllocator->Free(pCopy);
        copy.resource = res::ResLightingLookupTable(nullptr);
        copy.result = RESULT_FAILURE_MASK;
        return copy;
    }

    // the sampler refers to the table of the original
    const res::ResReferenceLookupTableData* pSampler = table.ptr()->sampler.To<res::ResReferenceLookupTableData>();
    res::ResReferenceLookupTableData* pSamplerCopy = new (pSamplerMemory) res::ResReferenceLookupTableData(*pSampler);
    pSamplerCopy->target = pSampler->target;
    pSamplerCopy->path = pSampler->path;
    pSamplerCopy->tableName = pSampler->tableName;
    pCopy->input = static_cast<u8>(table.ptr()->input);
    pCopy->scale = static_cast<u8>(table.ptr()->scale);
    pCopy->sampler.Set(pSamplerCopy);
    copy.resource = res::ResLightingLookupTable(pCopy);
    copy.result = RESULT_SUCCESS;
    return copy;
}

// 0x004B00B4 | nintendogs:bytes [tier A]
nw::gfx::Material::CopyResult<nw::gfx::res::ResFragmentLightingTable>
nw::gfx::Material::CopyResFragmentLightingTable(os::IAllocator* pAllocator, res::ResFragmentLightingTable table)
{
    u32 result = RESULT_SUCCESS;
    CopyResult<res::ResFragmentLightingTable> copy;
    void* pMemory = pAllocator->Alloc(sizeof(res::ResFragmentLightingTableData), 4);
    if (pMemory == nullptr) {
        copy.resource = res::ResFragmentLightingTable(nullptr);
        copy.result = result | RESULT_FAILURE_MASK;
        return copy;
    }
    res::ResFragmentLightingTableData* pCopy = new (pMemory) res::ResFragmentLightingTableData();
    for (int i = 0; i < 6; ++i) {
        if (table.ptr()->samplers[i].ToPtr() != nullptr && !(result & RESULT_FAILURE_MASK)) {
            CopyResult<res::ResLightingLookupTable> samplerCopy =
                CopyResLightingLookupTable(pAllocator, res::ResLightingLookupTable(table.ptr()->samplers[i].ToPtr()));
            pCopy->samplers[i].Set(samplerCopy.resource.ptr());
            result |= samplerCopy.result;
        }
    }
    copy.resource = res::ResFragmentLightingTable(pCopy);
    copy.result = result;
    return copy;
}

// 0x004B0388 | nintendogs:bytes [tier A]
nw::gfx::Material* nw::gfx::Material::Create(res::ResMaterial resource, int bufferCount, Model* pOwner, os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(Material), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    Material* pMaterial = new (pMemory) Material(pAllocator, resource, bufferCount, pOwner);
    u32 result = RESULT_SUCCESS;
    result |= pMaterial->CreateBuffers(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        if (pMaterial != nullptr) {
            pMaterial->~Material();
            pMaterial->GetAllocator()->Free(pMaterial);
        }
        return nullptr;
    }
    pMaterial->m_Resources[0] = pMaterial->GetResMaterial(BUFFER_USER_UNIFORMS);
    pMaterial->m_Resources[1] = pMaterial->GetResMaterial(BUFFER_SHADING);
    pMaterial->m_Resources[2] = pMaterial->GetResMaterial(BUFFER_MATERIAL_COLOR);
    pMaterial->m_Resources[3] = pMaterial->GetResMaterial(BUFFER_RASTERIZATION);
    pMaterial->m_Resources[4] = pMaterial->GetResMaterial(BUFFER_TEXTURE_COORDINATORS);
    pMaterial->m_Resources[5] = pMaterial->GetResMaterial(BUFFER_TEXTURE_MAPPERS);
    pMaterial->m_Resources[6] = pMaterial->GetResMaterial(BUFFER_FRAGMENT_LIGHTING);
    pMaterial->m_Resources[7] = pMaterial->GetResMaterial(BUFFER_FRAGMENT_LIGHTING_TABLE);
    pMaterial->m_Resources[8] = pMaterial->GetResMaterial(BUFFER_TEXTURE_COMBINERS);
    pMaterial->m_Resources[9] = pMaterial->GetResMaterial(BUFFER_COMBINER_BUFFER);
    pMaterial->m_Resources[10] = pMaterial->GetResMaterial(BUFFER_FRAGMENT_OPERATION);
    pMaterial->m_Resources[11] = pMaterial->GetResMaterial(BUFFER_SCENE_ENVIRONMENT);
    return pMaterial;
}

// 0x004B04EC | nintendogs (C)
nw::gfx::Material::Material(os::IAllocator* pAllocator, res::ResMaterial resource, int bufferCount, Model* pOwner)
    : SceneObject(pAllocator, res::ResSceneObject(resource.ptr())), m_Buffers(), m_pOwner(pOwner),
      m_ShaderProgramDescription(nullptr), m_BufferCount(bufferCount), m_Resources()
{
    res::ResShader shader = res::ResShader(resource.ptr()->shader.ToPtr()).Dereference();
    m_ShaderProgramDescription =
        res::ResShaderProgramDescription(res::ResBinaryShader(shader.ptr()).GetDescription(resource.ptr()->shaderProgramDescriptionIndex));
}

// 0x004B0658 | nintendogs:bytes
// 0x004B05A8 (deleting dtor)
nw::gfx::Material::~Material()
{
    os::IAllocator* pAllocator = GetAllocator();
    for (res::ResMaterialData** ppBuffer = m_Buffers.begin(); ppBuffer != m_Buffers.end(); ++ppBuffer) {
        DestroyResMaterial(pAllocator, res::ResMaterial(*ppBuffer));
    }
}

// 0x0073C888 | nintendogs:bytes-fuzzy [tier A]
bool nw::gfx::Material::CanUseBuffer(u32 memberType) const
{
    if (m_BufferCount <= 0) {
        return false;
    }
    const u32 bufferOption = m_pOwner->GetBufferOption();
    switch (memberType) {
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MATERIAL_COLOR:
        return (bufferOption & BUFFER_MATERIAL_COLOR) != 0;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER:
        return (bufferOption & BUFFER_TEXTURE_MAPPERS) != 0;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER:
        return (bufferOption & BUFFER_TEXTURE_MAPPERS) != 0;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_BLEND_OPERATION:
        return (bufferOption & BUFFER_FRAGMENT_OPERATION) != 0;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_COORDINATOR:
        return (bufferOption & BUFFER_TEXTURE_COORDINATORS) != 0;
    default:
        return false;
    }
}

// 0x00525404 (name is ours)
nw::gfx::res::ResMaterial nw::gfx::Material::GetResMaterial(BufferFlag flag) const
{
    if ((m_pOwner->GetBufferOption() & flag) != flag) {
        return GetOriginal();
    }
    return GetBuffer();
}

} // namespace gfx
} // namespace nw
