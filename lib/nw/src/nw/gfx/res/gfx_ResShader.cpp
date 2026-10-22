#include <new>

#include "nw/gfx/gfx_ShaderBinaryInfo.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
typedef u32 (*ShaderSetupFunc)(os::IAllocator* pAllocator, ResShaderData* pData);

// the alignment of the memory of a shader
const u8 SHADER_ALIGNMENT = 4;

// 0x004A8268 (name is ours)
// the binary analyzed and its commands built, then those of each program (once: the first slot is
// set by the first setup)
u32 SetupBinaryShader(os::IAllocator* pAllocator, ResShaderData* pData)
{
    ResBinaryShaderData* pShader = const_cast<ResBinaryShaderData*>(DynamicCastData<ResBinaryShaderData>(pData, ResShader::TYPE_BINARY));
    if (pShader->shaderSlots.To<const void*>()[0] != nullptr) {
        return RESULT_SUCCESS;
    }

    pShader->pAllocator = pAllocator;
    for (int i = 0; i < pShader->shaderCount; ++i) {
        pShader->shaderSlots.To<const void*>()[i] = pData;
    }

    void* pMemory = pAllocator->Alloc(sizeof(ShaderBinaryInfo), SHADER_ALIGNMENT);
    ShaderBinaryInfo* pInfo = nullptr;
    if (pMemory != nullptr) {
        pInfo = new (pMemory) ShaderBinaryInfo(pShader->binary.ToPtr());
    }
    pShader->pBinaryInfo = pInfo;
    pInfo->AnalyzeBinary();

    const u32 size = pInfo->GetCommonCommandSize();
    u32* pCommand = static_cast<u32*>(pAllocator->Alloc(size, SHADER_ALIGNMENT));
    pShader->commonCommandSize = pInfo->BuildCommonCommand(pCommand, size);
    pShader->pCommonCommand = pCommand;

    for (int i = 0; i < pShader->descriptions.count; ++i) {
        ResShaderProgramDescriptionData* pDescription = static_cast<ResShaderProgramDescriptionData*>(pShader->descriptions.GetData(i));
        const void* pVertexShader = pShader->shaderSlots.To<const void*>()[pDescription->vertexShaderIndex];
        const void* pGeometryShader = nullptr;
        const s32 geometryShaderIndex = pDescription->geometryShaderIndex;
        if (geometryShaderIndex >= 0 && geometryShaderIndex < pShader->shaderSlotCount) {
            pGeometryShader = pShader->shaderSlots.To<const void*>()[geometryShaderIndex];
        }
        static_cast<ResShaderProgramDescriptionData*>(pShader->descriptions.GetData(i))->pVertexShader = pVertexShader;
        static_cast<ResShaderProgramDescriptionData*>(pShader->descriptions.GetData(i))->pGeometryShader = pGeometryShader;
        ResShaderProgramDescription(pShader->descriptions.GetData(i)).Setup(pAllocator);
    }
    return RESULT_SUCCESS;
}

// 0x004A8C50 (name is ours)
// the shader the reference was resolved to set up (if it is a binary shader)
u32 SetupReferenceShader(os::IAllocator* pAllocator, ResShaderData* pData)
{
    const ResReferenceShaderData* pShader = DynamicCastData<ResReferenceShaderData>(pData, ResShader::TYPE_REFERENCE);
    const ResShaderData* pTarget = DynamicCastData<ResShaderData>(pShader->target.ToPtr(), ResShader::TYPE_BINARY);
    if (pTarget != nullptr) {
        SetupBinaryShader(pAllocator, const_cast<ResShaderData*>(pTarget));
    }
    return RESULT_SUCCESS;
}
} // namespace

// the setup per type of shader: binary, reference (name is ours)
// 0x0097F4A8 (name is ours)
ShaderSetupFunc s_ShaderSetupFuncs[] = {SetupBinaryShader, SetupReferenceShader};

// 0x001378BC | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::res::ResShader::Cleanup()
{
    ResBinaryShaderData* pShader = const_cast<ResBinaryShaderData*>(DynamicCastData<ResBinaryShaderData>(m_pData, TYPE_BINARY));
    if (pShader == nullptr) {
        return;
    }

    if (pShader->pCommonCommand != nullptr) {
        pShader->pAllocator->Free(pShader->pCommonCommand);
        pShader->pCommonCommand = nullptr;
        pShader->commonCommandSize = 0;
    }
    if (pShader->pBinaryInfo != nullptr) {
        pShader->pAllocator->Free(pShader->pBinaryInfo);
        pShader->pBinaryInfo = nullptr;
    }

    const ut::Offset* pBegin = pShader->descriptions.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + pShader->descriptions.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        ResShaderProgramDescriptionData* pDescription = const_cast<ResShaderProgramDescriptionData*>(pIt->To<ResShaderProgramDescriptionData>());
        if (pDescription == nullptr) {
            continue;
        }
        const int symbolCount = pDescription->symbols.count;
        for (int i = 0; i < symbolCount; ++i) {
            ResShaderSymbolData* pSymbol = static_cast<ResShaderSymbolData*>(pDescription->symbols.GetData(i));
            pSymbol->index = -1;
            pSymbol->isGeometry = false;
        }
        pDescription->m_Unknown0x74 = 0;
        if (pDescription->pCommand != nullptr) {
            pDescription->owner.To<ResBinaryShaderData>()->pAllocator->Free(pDescription->pCommand);
            pDescription->pCommand = nullptr;
            pDescription->commandSize = 0;
        }
    }

    for (int i = 0; i < pShader->shaderSlotCount; ++i) {
        if (pShader->shaderSlots.To<const void*>()[i] != nullptr) {
            pShader->shaderSlots.To<const void*>()[i] = nullptr;
        }
    }
    pShader->pAllocator = nullptr;
}

// 0x004AA6C8 | nintendogs:bytes [tier A]
nw::gfx::res::ResShader nw::gfx::res::ResShader::Dereference()
{
    switch (m_pData->typeInfo) {
    case TYPE_REFERENCE:
        return ResShader(static_cast<const ResReferenceShaderData*>(m_pData)->target.ToPtr()).Dereference();
    case TYPE_BINARY:
        return *this;
    default:
        return ResShader(nullptr);
    }
}

// 0x004AA708 | nintendogs:bytes [tier B]
u32 nw::gfx::res::ResShader::Setup(os::IAllocator* pAllocator, ResGraphicsFile /*graphicsFile*/)
{
    u32 result = RESULT_SUCCESS;
    ResShaderData* pData = const_cast<ResShaderData*>(m_pData);
    switch (pData->typeInfo) {
    case TYPE_BINARY:
        result |= s_ShaderSetupFuncs[0](pAllocator, pData);
        break;
    case TYPE_REFERENCE:
        result |= s_ShaderSetupFuncs[1](pAllocator, pData);
        break;
    default:
        break;
    }
    return result;
}

// 0x004A97BC (name is ours)
u32 nw::gfx::res::ResShaderProgramDescription::Setup(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;

    // the registers of the uniforms: in the vertex shader, else in the geometry shader
    const int symbolCount = ptr()->symbols.count;
    for (int i = 0; i < symbolCount; ++i) {
        ResShaderSymbolData* pSymbol = static_cast<ResShaderSymbolData*>(ptr()->symbols.GetData(i));
        if (pSymbol->index != -1) {
            continue;
        }
        ShaderBinaryInfo::UniformLocation location =
            ptr()->owner.To<ResBinaryShaderData>()->pBinaryInfo->SearchUniformIndex(ptr()->vertexShaderIndex, pSymbol->name.To<char>());
        if (location.index >= 0) {
            pSymbol->isGeometry = false;
        } else {
            location = ptr()->owner.To<ResBinaryShaderData>()->pBinaryInfo->SearchUniformIndex(ptr()->geometryShaderIndex,
                                                                                               pSymbol->name.To<char>());
            if (location.index < 0) {
                result |= RESULT_UNIFORM_NOT_FOUND;
                pSymbol->index = -1;
                continue;
            }
            pSymbol->isGeometry = true;
        }
        pSymbol->index = location.index;
    }

    // the commands of the program (once)
    if (ptr()->pCommand != nullptr) {
        return result;
    }
    const ShaderBinaryInfo* pInfo = ptr()->owner.To<ResBinaryShaderData>()->pBinaryInfo;
    const int vertexShaderIndex = ptr()->vertexShaderIndex;
    const int geometryShaderIndex = ptr()->geometryShaderIndex;
    const u32 size = pInfo->GetShaderProgramCommandSize(vertexShaderIndex, geometryShaderIndex);
    u32* pCommand = static_cast<u32*>(pAllocator->Alloc(size, SHADER_ALIGNMENT));
    const u32 commandSize = pInfo->BuildShaderProgramCommand(vertexShaderIndex, geometryShaderIndex, pCommand, size);
    ptr()->pCommand = pCommand;
    ptr()->commandSize = commandSize;
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
