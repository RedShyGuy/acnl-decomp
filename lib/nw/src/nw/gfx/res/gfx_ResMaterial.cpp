#include <cstring>

#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"
#include "nw/gfx/res/res_Api.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// the texture types of TEXUNIT0_PARAM bits 28-30 (3dbrew "GPU/Internal Registers"; names are ours)
const u32 PARAM_TYPE_MASK = 0x70000000;
const u32 PARAM_TYPE_CUBE = 0x10000000;
const u32 PARAM_TYPE_SHADOW_2D = 0x20000000;
const u32 PARAM_TYPE_PROJECTION = 0x30000000;
const u32 PARAM_TYPE_SHADOW_CUBE = 0x40000000;

// the blend mode of the fragment operation that uses the logic operation
const u8 BLEND_MODE_LOGIC_OP = 3;

// (inline) a value rotated to the left (name is ours)
inline u32 RotateLeft(u32 value, int shift)
{
    return shift != 0 ? (value << shift) | (value >> (32 - shift)) : value;
}

// (inline) whether a blend factor reads the color buffer: DST_COLOR, ONE_MINUS_DST_COLOR,
// DST_ALPHA, ONE_MINUS_DST_ALPHA, SRC_ALPHA_SATURATE (3dbrew "GPU/Internal Registers"; name is ours)
inline bool ReadsDestination(u32 factor)
{
    switch (factor) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 14:
        return true;
    default:
        return false;
    }
}

// (inline) the hash of the lookup table of a lighting sampler (a reference only; name is ours)
inline u32 HashLightingSampler(ResFragmentLightingTable table, int index, int shift)
{
    ResLightingLookupTable sampler = table.GetSampler(index);
    if (sampler.IsValid()) {
        ResLookupTable lut(DynamicCastData<ResLookupTableData>(sampler.GetSampler().ptr(), ResLookupTable::TYPE_REFERENCE));
        if (lut.IsValid()) {
            return RotateLeft(reinterpret_cast<u32>(lut.Dereference()), shift);
        }
    }
    return 0;
}
} // namespace

// 0x0013B800 | nintendogs:bytes [tier A]
void nw::gfx::res::ResMaterial::Cleanup()
{
    ptr()->flags &= ~ResMaterialData::FLAG_SETUP_DONE;
    ptr()->flags &= ~ResMaterialData::FLAG_FRAGMENT_SHADER_SETUP_DONE;
    ResFragmentShader fragmentShader(ptr()->fragmentShader.ToPtr());
    if (fragmentShader.IsValid()) {
        fragmentShader.Cleanup();
    }

    ptr()->flags &= ~ResMaterialData::FLAG_SHADER_SETUP_DONE;
    ResReferenceShaderData* pReference =
        const_cast<ResReferenceShaderData*>(DynamicCastData<ResReferenceShaderData>(ptr()->shader.ToPtr(), ResShader::TYPE_REFERENCE));
    if (pReference != nullptr) {
        pReference->target.m_Offset = 0;
    }

    ptr()->flags &= ~ResMaterialData::FLAG_TEXTURES_SETUP_DONE;
    for (int i = 0; i < 3; ++i) {
        ResTextureMapper mapper(ptr()->textureMappers[i].ToPtr());
        if (mapper.IsValid()) {
            mapper.Cleanup();
        }
    }
    if (ptr()->proceduralTextureMapper.ToPtr() != nullptr) {
        ResTextureMapper(ptr()->proceduralTextureMapper.ToPtr()).Cleanup();
    }
}

// 0x004A5CA0 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResMaterial::SetupShader(os::IAllocator* pAllocator, ResShader shader, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    if (!shader.IsValid()) {
        return result;
    }

    // a reference is resolved first
    ResShader target;
    const ResReferenceShaderData* pReference = DynamicCastData<ResReferenceShaderData>(shader.ptr(), ResShader::TYPE_REFERENCE);
    bool isFound = true;
    if (pReference == nullptr) {
        target = shader;
    } else if (pReference->target.ToPtr() != nullptr) {
        target = ResShader(pReference->target.ToPtr());
    } else {
        ReferenceTarget<ResShader> found = GetReferenceShaderTarget(ResReferenceShader(pReference), graphicsFile);
        if (found.isFound) {
            target = found.target;
        } else {
            result |= RESULT_SHADER_NOT_FOUND;
            isFound = false;
        }
    }
    if (isFound) {
        result |= target.Setup(pAllocator, graphicsFile);
    }

    if (shader.IsValid() && !(result & RESULT_FAILURE_MASK)) {
        ptr()->flags |= ResMaterialData::FLAG_SHADER_SETUP_DONE;
        const ResBinaryShaderData* pBinary = static_cast<const ResBinaryShaderData*>(shader.Dereference().ptr());
        const ResShaderProgramDescriptionData* pDescription =
            static_cast<const ResShaderProgramDescriptionData*>(pBinary->descriptions.GetData(ptr()->shaderProgramDescriptionIndex));
        CacheUserUniformIndex(ut::internal::ResArray<ResShaderSymbol, ut::internal::ResArrayClassTraits>(
            pDescription->symbols.array.To<ut::Offset>(), pDescription->symbols.count));
    }
    return result;
}

// 0x004A5DF4 | nintendogs:bytes-fuzzy [tier A]
u32 nw::gfx::res::ResMaterial::SetupTextures(os::IAllocator* pAllocator, ResMaterial material, ResGraphicsFile graphicsFile)
{
    ResMaterialData* pData = material.ptr();
    u32 result = RESULT_SUCCESS;

    // which texture coordinators are used: by the mapper of their unit, or for the procedural texture
    // or the texture units 1/2 by the configuration
    const u8 config = static_cast<u8>(pData->textureCoordinateConfig);
    const bool hasProcedural = pData->proceduralTextureMapper.ToPtr() != nullptr;
    for (int i = 0; i < 3; ++i) {
        const bool hasMapper = pData->textureMappers[i].ToPtr() != nullptr;
        bool isEnabled = false;
        switch (i) {
        case 0:
            if (hasMapper) {
                isEnabled = true;
            } else if ((config == 0 || config == 1) && hasProcedural) {
                isEnabled = true;
            }
            break;
        case 1:
            if (hasMapper) {
                isEnabled = true;
            } else {
                switch (config) {
                case 2:
                    if (pData->textureMappers[2].ToPtr() != nullptr || hasProcedural) {
                        isEnabled = true;
                    }
                    break;
                case 1:
                case 3:
                    if (pData->textureMappers[2].ToPtr() != nullptr) {
                        isEnabled = true;
                    }
                    break;
                case 4:
                    if (hasProcedural) {
                        isEnabled = true;
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case 2:
            switch (config) {
            case 5:
                if (hasMapper || hasProcedural) {
                    isEnabled = true;
                }
                break;
            case 0:
            case 4:
                if (hasMapper) {
                    isEnabled = true;
                }
                break;
            case 3:
                if (hasProcedural) {
                    isEnabled = true;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        pData->textureCoordinators[i].isEnabled = isEnabled;
    }

    // the mappers: their setup, the texture type of unit 0 from the mapping method, the hash
    u32 hash = pData->textureSamplersHash;
    for (int i = 0; i < 3; ++i) {
        ResTextureMapper mapper(pData->textureMappers[i].ToPtr());
        if (!mapper.IsValid() || !mapper.GetTexture().IsValid()) {
            continue;
        }
        result |= mapper.Setup(pAllocator, graphicsFile);

        ResPixelBasedTextureMapperData* pMapper = static_cast<ResPixelBasedTextureMapperData*>(mapper.ptr());
        u32& param = pMapper->commands[ResPixelBasedTextureMapperData::COMMAND_PARAM];
        param &= ~PARAM_TYPE_MASK;
        if (i == 0) {
            switch (pData->textureCoordinators[0].GetMappingMethod()) {
            case ResTextureCoordinatorData::MAPPING_CUBE_ENV:
                param |= PARAM_TYPE_CUBE;
                break;
            case ResTextureCoordinatorData::MAPPING_PROJECTION:
                param |= PARAM_TYPE_PROJECTION;
                break;
            case ResTextureCoordinatorData::MAPPING_SHADOW:
                param |= PARAM_TYPE_SHADOW_2D;
                break;
            case ResTextureCoordinatorData::MAPPING_SHADOW_CUBE:
                param |= PARAM_TYPE_SHADOW_CUBE;
                break;
            default:
                break;
            }
        }
        if (result & RESULT_FAILURE_MASK) {
            continue;
        }

        if (pData->textureCoordinators[0].GetMappingMethod() == ResTextureCoordinatorData::MAPPING_CUBE_ENV &&
            internal::DereferenceTexture(pMapper->texture.To<ResTextureData>())->typeInfo != ResTexture::TYPE_CUBE) {
            result |= RESULT_NOT_CUBE_TEXTURE;
        }
        hash ^= RotateLeft(reinterpret_cast<u32>(internal::DereferenceTexture(pMapper->texture.To<ResTextureData>())), i * 8);
    }
    pData->textureMappersHash = hash;

    if (pData->proceduralTextureMapper.ToPtr() != nullptr) {
        ResTextureMapper procedural(pData->proceduralTextureMapper.ToPtr());
        result |= procedural.Setup(pAllocator, graphicsFile);
    }

    if (!(result & RESULT_FAILURE_MASK)) {
        ptr()->flags |= ResMaterialData::FLAG_TEXTURES_SETUP_DONE;
    }
    return result;
}

// 0x004A618C (name is ours)
void nw::gfx::res::ResMaterial::ForceSetupTexture(const char* name, ResTexture texture)
{
    u32 hash = ptr()->textureSamplersHash;
    for (int i = 0; i < 3; ++i) {
        ResPixelBasedTextureMapper mapper(ptr()->textureMappers[i].ToPtr());
        if (!mapper.IsValid()) {
            continue;
        }
        const ResReferenceTextureData* pReference =
            DynamicCastData<ResReferenceTextureData>(mapper.ptr()->texture.ToPtr(), ResTexture::TYPE_REFERENCE);
        if (pReference != nullptr && std::strcmp(pReference->path.To<char>(), name) == 0) {
            mapper.ForceSetupTexture(texture);
        }
        hash ^= RotateLeft(reinterpret_cast<u32>(internal::DereferenceTexture(mapper.ptr()->texture.To<ResTextureData>())), i * 8);
    }
    ptr()->textureMappersHash = hash;
}

// 0x004A62B0 | nintendogs:bytes [tier A]
void nw::gfx::res::ResMaterial::CacheUserUniformIndex(ut::internal::ResArray<ResShaderSymbol, ut::internal::ResArrayClassTraits> symbols)
{
    for (int i = 0; i < ptr()->userUniforms.count; ++i) {
        ResUserUniformData* pUniform = static_cast<ResUserUniformData*>(ptr()->userUniforms.GetData(i));
        if (pUniform == nullptr) {
            continue;
        }
        const char* name = pUniform->name.To<char>();
        if (name == nullptr) {
            continue;
        }
        for (int j = 0; j < static_cast<int>(symbols.size()); ++j) {
            if (std::strcmp(name, symbols[j].GetName()) == 0) {
                pUniform->symbolIndex = j;
                break;
            }
        }
    }
}

// 0x004A6380 | nintendogs:bytes [tier A]
void nw::gfx::res::ResMaterial::CalcFragmentLightingTableHash()
{
    ResFragmentLightingTable table = ResFragmentShader(ptr()->fragmentShader.ToPtr()).GetLightingTable();
    u32 hash = ptr()->fragmentLightingTableHashSeed;
    hash ^= HashLightingSampler(table, 3, 0);
    hash ^= HashLightingSampler(table, 4, 4);
    hash ^= HashLightingSampler(table, 0, 8);
    hash ^= HashLightingSampler(table, 1, 12);
    hash ^= HashLightingSampler(table, 2, 16);
    hash ^= HashLightingSampler(table, 5, 20);
    ptr()->fragmentLightingTableHash = hash;
}

// 0x004A662C | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResMaterial::Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    if (!(ptr()->flags & ResMaterialData::FLAG_TEXTURES_SETUP_DONE)) {
        result |= SetupTextures(pAllocator, *this, graphicsFile);
    }
    if (!(ptr()->flags & ResMaterialData::FLAG_SHADER_SETUP_DONE)) {
        result |= SetupShader(pAllocator, ResShader(ptr()->shader.ToPtr()), graphicsFile);
    }
    if (!(ptr()->flags & ResMaterialData::FLAG_FRAGMENT_SHADER_SETUP_DONE)) {
        ResFragmentShader fragmentShader(ptr()->fragmentShader.ToPtr());
        const u32 fragmentResult = fragmentShader.Setup(pAllocator, graphicsFile, ptr());
        result |= fragmentResult;
        if (!(fragmentResult & RESULT_FAILURE_MASK)) {
            ptr()->flags |= ResMaterialData::FLAG_FRAGMENT_SHADER_SETUP_DONE;
            CalcFragmentLightingTableHash();
        }
    }

    // whether the fragment operation reads the color buffer: a logic operation other than CLEAR,
    // COPY, SET, COPY_INVERTED; a blend with a source factor that reads it or a destination factor
    // other than ZERO
    bool isWithoutDestination;
    u32& mode = ptr()->fragmentOperationMode;
    if (static_cast<u8>(mode) == BLEND_MODE_LOGIC_OP) {
        const u32 operation = ptr()->logicOperation & 0xF;
        isWithoutDestination = operation == 0 || operation == 3 || operation == 4 || operation == 5;
    } else {
        const u32 blend = ptr()->blendFunction;
        isWithoutDestination = !ReadsDestination((blend >> 16) & 0xF) && !ReadsDestination((blend >> 24) & 0xF) &&
                               ((blend >> 20) & 0xF) == 0 && (blend >> 28) == 0;
    }
    if (isWithoutDestination) {
        mode |= ResMaterialData::FRAGMENT_OPERATION_NO_DESTINATION;
    } else {
        mode &= ~ResMaterialData::FRAGMENT_OPERATION_NO_DESTINATION;
    }

    if (!(result & RESULT_FAILURE_MASK)) {
        ptr()->flags |= ResMaterialData::FLAG_SETUP_DONE;
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
