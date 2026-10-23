#pragma once

#include "decomp.h"
#include "nn/gx/detail/gx_Context.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_RenderContext.h"
#include "nw/gfx/gfx_SceneEnvironment.h"
#include "nw/gfx/gfx_ShaderProgram.h"
#include "nw/gfx/internal/gfx_LightingRegisters.h"
#include "nw/gfx/internal/gfx_MaterialState.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"

#include <cstring>

namespace nw {
namespace gfx {
namespace internal {
// The body the material activators share (names are ours): MaterialActivator and
// SimpleMaterialActivator are the same code apart from the resource each part is read from.
namespace activation {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_DEPTHMAP_SCALE = 0x04D;  // with DEPTHMAP_OFFSET
const u32 REG_DEPTHMAP_ENABLE = 0x06D;
const u32 REG_TEXUNIT_CONFIG = 0x080;
const u32 REG_LIGHTING_ENABLE0 = 0x08F;
const u32 REG_TEXENV_UPDATE_BUFFER = 0x0E0;
const u32 REG_STENCIL_TEST = 0x105;
const u32 REG_DEPTH_COLOR_MASK = 0x107;
const u32 REG_LIGHT0_CONFIG = 0x149;
const u32 REG_LIGHT_STRIDE = 0x10;
const u32 REG_LIGHTING_CONFIG0 = 0x1C3;
const u32 REG_LIGHTING_CONFIG1 = 0x1C4;
const u32 REG_LIGHTING_ENABLE1 = 0x1C6; // 0: the lighting is used

// in TEXUNIT_CONFIG: the texture cache is cleared
const u32 TEXUNIT_CONFIG_CLEAR_CACHE = 1 << 16;
// in TEXENV_UPDATE_BUFFER: the fog mode
const u32 FOG_MODE_FOG = 5;
// in LIGHTING_CONFIG0: bits 10 and 31 are always set
const u32 LIGHTING_CONFIG0_ALWAYS = 0x80000400;
// in LIGHTn_CONFIG
const u32 LIGHT_CONFIG_DIRECTIONAL = 1 << 0;
const u32 LIGHT_CONFIG_TWO_SIDED_DIFFUSE = 1 << 1;
const u32 LIGHT_CONFIG_GEOMETRIC_FACTOR0 = 1 << 2;
const u32 LIGHT_CONFIG_GEOMETRIC_FACTOR1 = 1 << 3;

// the words of g_BufferAccessCommand that the fragment operation sets
const int BUFFER_ACCESS_COLOR_READ = 4;
const int BUFFER_ACCESS_COLOR_WRITE = 6;
const int BUFFER_ACCESS_DEPTH_READ = 7;
const int BUFFER_ACCESS_DEPTH_WRITE = 8;
// the fragment operation modes (COLOR_OPERATION bits 0-1)
const u8 FRAGMENT_OPERATION_MODE_DEFAULT = 0;
const u8 FRAGMENT_OPERATION_MODE_GAS = 1;

// GL_DEPTH_COMPONENT16
const u32 DEPTH_FORMAT_16 = 0x81A5;

// the colors of a material a texture combiner stage can use as its constant
enum ConstantSource
{
    CONSTANT_SOURCE_CONSTANT0 = 0,
    CONSTANT_SOURCE_CONSTANT1 = 1,
    CONSTANT_SOURCE_CONSTANT2 = 2,
    CONSTANT_SOURCE_CONSTANT3 = 3,
    CONSTANT_SOURCE_CONSTANT4 = 4,
    CONSTANT_SOURCE_CONSTANT5 = 5,
    CONSTANT_SOURCE_EMISSION = 6,
    CONSTANT_SOURCE_AMBIENT = 7,
    CONSTANT_SOURCE_DIFFUSE = 8,
    CONSTANT_SOURCE_SPECULAR0 = 9,
    CONSTANT_SOURCE_SPECULAR1 = 10,
};

// the words of a resource part to the current command list
inline void PutCommand(const u32* pCommand, int count)
{
    u32* pCurrent = nn::gx::detail::s_pCommandBufferCurrent;
    for (int i = 0; i < count; ++i) {
        pCurrent[i] = pCommand[i];
    }
    nn::gx::detail::s_pCommandBufferCurrent = pCurrent + count;
}

// (inline) the polygon offset (DEPTHMAP_OFFSET from the depth range) and the culling
DECOMP_ALWAYS_INLINE void PutRasterization(const res::ResMaterialData* pRasterization)
{
    g_IsPolygonOffset = (pRasterization->rasterizationFlags & res::ResMaterialData::RASTERIZATION_POLYGON_OFFSET) != 0;
    g_PolygonOffsetUnits = pRasterization->polygonOffsetUnits;

    // 0x00AF9264
    static u32 s_DepthMapCommand[6] = {
        0, Header(REG_DEPTHMAP_ENABLE, 0x1), 0, Header(REG_DEPTHMAP_SCALE, 0x7, 1, true), 0, 0,
    };
    f32 offset;
    f32 units;
    if (g_WScale != 0) {
        s_DepthMapCommand[0] = 0;
        s_DepthMapCommand[2] = g_WScale;
        offset = 0.0f;
        units = -g_PolygonOffsetUnits;
    } else {
        s_DepthMapCommand[0] = 1;
        s_DepthMapCommand[2] = g_DepthMapScale;
        offset = g_DepthRangeNear;
        units = (g_DepthRangeNear - g_DepthRangeFar) * g_PolygonOffsetUnits;
    }
    if (g_IsPolygonOffset) {
        // one unit is the smallest step of the depth buffer
        if (g_DepthFormat == DEPTH_FORMAT_16) {
            offset -= units * (1.0f / 65535.0f);
        } else {
            offset -= units * 128.0f * (1.0f / 16777215.0f);
        }
    }
    s_DepthMapCommand[4] = Float32ToFloat24Clamped(offset);
    u32* pCommand = FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, s_DepthMapCommand);
    pCommand[0] = pRasterization->cullingCommand[0];
    pCommand[1] = pRasterization->cullingCommand[1];
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
}

// (inline) the texture combiners with the constant colors of the material
DECOMP_ALWAYS_INLINE void PutTextureCombiners(res::ResFragmentShaderData* pShader, const res::ResMaterialColorData& color)
{
    nn::gx::detail::s_pCommandBufferCurrent =
        FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, pShader->bufferColorCommand);
    for (int i = 0; i < 6; ++i) {
        res::ResTextureCombinerData& combiner = pShader->combiners[i];
        u32 constant;
        switch (static_cast<u8>(combiner.constantSource)) {
        case CONSTANT_SOURCE_CONSTANT0:
            constant = color.constantU8[0];
            break;
        case CONSTANT_SOURCE_CONSTANT1:
            constant = color.constantU8[1];
            break;
        case CONSTANT_SOURCE_CONSTANT2:
            constant = color.constantU8[2];
            break;
        case CONSTANT_SOURCE_CONSTANT3:
            constant = color.constantU8[3];
            break;
        case CONSTANT_SOURCE_CONSTANT4:
            constant = color.constantU8[4];
            break;
        case CONSTANT_SOURCE_CONSTANT5:
            constant = color.constantU8[5];
            break;
        case CONSTANT_SOURCE_EMISSION:
            constant = color.emissionU8;
            break;
        case CONSTANT_SOURCE_AMBIENT:
            constant = color.ambientU8;
            break;
        case CONSTANT_SOURCE_DIFFUSE:
            constant = color.diffuseU8;
            break;
        case CONSTANT_SOURCE_SPECULAR0:
            constant = color.specular0U8;
            break;
        case CONSTANT_SOURCE_SPECULAR1:
            constant = color.specular1U8;
            break;
        default:
            constant = color.constantU8[0];
            break;
        }
        combiner.command[4] = constant;
        nn::gx::detail::s_pCommandBufferCurrent = FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, combiner.command);
    }
}

// (inline) the fragment operation: depth, stencil, blending, and the access to the buffers it needs
DECOMP_ALWAYS_INLINE void PutFragmentOperation(res::ResMaterialData* pOperation)
{
    u32* pCommand;
    bool isDepthTest = (pOperation->depthFlags & res::ResMaterialData::DEPTH_TEST) != 0;
    g_IsBufferAccessDirty = g_IsBufferAccessDirty || g_IsDepthTest != isDepthTest;
    g_IsDepthTest = isDepthTest;
    bool isDepthWrite = (pOperation->depthFlags & res::ResMaterialData::DEPTH_WRITE) != 0;
    g_IsBufferAccessDirty = g_IsBufferAccessDirty || g_IsDepthWrite != isDepthWrite;
    g_IsDepthWrite = isDepthWrite;

    pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = g_ColorWriteMask << 8 | isDepthWrite << 12;
    pCommand[1] = Header(REG_DEPTH_COLOR_MASK, 0x2);
    pCommand[2] = g_StencilWriteMask << 8;
    pCommand[3] = Header(REG_STENCIL_TEST, 0x2);
    pCommand[4] = pOperation->depthCommand[0];
    pCommand[5] = pOperation->depthCommand[1];
    pCommand[6] = pOperation->depthCommand[2];
    pCommand[7] = pOperation->depthCommand[3];
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 8;

    bool isStencilTest = (pOperation->stencilCommand[0] & 1) != 0;
    g_IsBufferAccessDirty = g_IsBufferAccessDirty || g_IsStencilTest != isStencilTest;
    g_IsStencilTest = isStencilTest;
    PutCommand(pOperation->stencilCommand, 4);

    bool isColorRead = !(pOperation->blendOperation.mode & res::ResMaterialData::FRAGMENT_OPERATION_NO_DESTINATION);
    g_IsBufferAccessDirty = g_IsBufferAccessDirty || g_IsColorRead != isColorRead;
    g_IsColorRead = isColorRead;
    nn::gx::detail::s_pCommandBufferCurrent =
        FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, &pOperation->blendOperation.colorOperation);

    u8 mode = pOperation->blendOperation.colorOperation & 3;
    if (g_FragmentOperationMode != mode) {
        g_FragmentOperationMode = mode;
        g_IsBufferAccessDirty = true;
    }
    if (g_IsBufferAccessDirty) {
        g_IsBufferAccessDirty = false;
        u32* pAccess = g_BufferAccessCommand;
        pAccess[BUFFER_ACCESS_COLOR_READ] = 0;
        pAccess[BUFFER_ACCESS_COLOR_WRITE] = 0;
        if (mode != FRAGMENT_OPERATION_MODE_DEFAULT) {
            pAccess[BUFFER_ACCESS_COLOR_READ] = 0xF;
            pAccess[BUFFER_ACCESS_COLOR_WRITE] = 0xF;
        } else if (g_ColorWriteMask != 0) {
            if (g_IsColorRead || g_ColorWriteMask != 0xF) {
                pAccess[BUFFER_ACCESS_COLOR_READ] = 0xF;
            }
            pAccess[BUFFER_ACCESS_COLOR_WRITE] = 0xF;
        }
        pAccess[BUFFER_ACCESS_DEPTH_READ] = 0;
        pAccess[BUFFER_ACCESS_DEPTH_WRITE] = 0;
        if (mode == FRAGMENT_OPERATION_MODE_GAS) {
            pAccess[BUFFER_ACCESS_DEPTH_READ] = 3;
        } else if (mode == FRAGMENT_OPERATION_MODE_DEFAULT) {
            // bit 1: depth, bit 0: stencil
            if (g_IsDepthTest) {
                if (g_IsDepthWrite) {
                    pAccess[BUFFER_ACCESS_DEPTH_READ] = 2;
                    pAccess[BUFFER_ACCESS_DEPTH_WRITE] = 2;
                } else if (g_ColorWriteMask != 0) {
                    pAccess[BUFFER_ACCESS_DEPTH_READ] = 2;
                }
            }
            if (g_IsStencilTest) {
                if (g_StencilWriteMask != 0) {
                    pAccess[BUFFER_ACCESS_DEPTH_READ] |= 1;
                    pAccess[BUFFER_ACCESS_DEPTH_WRITE] |= 1;
                } else if (g_ColorWriteMask != 0) {
                    pAccess[BUFFER_ACCESS_DEPTH_READ] |= 1;
                }
            }
        }
    }
    std::memcpy(nn::gx::detail::s_pCommandBufferCurrent, g_BufferAccessCommand, sizeof(g_BufferAccessCommand));
    nn::gx::detail::s_pCommandBufferCurrent += sizeof(g_BufferAccessCommand) / sizeof(u32);
}
} // namespace activation

// (inline) the parts of the material whose hash changed (or is 0) to the current command list;
// getResource(material, part) is the resource of a part (Material::ResourceIndex)
template <typename TGetResource>
DECOMP_ALWAYS_INLINE void ActivateMaterial(RenderContext* pRenderContext, const Material* pMaterial, TGetResource getResource)
{
    using namespace activation;

    const SceneEnvironment& environment = pRenderContext->m_SceneEnvironment;
    ShaderProgram* pProgram = pRenderContext->m_ShaderProgram.Get();
    const bool isLightDirty = environment.m_IsFragmentLightDirty || environment.m_IsAmbientLightDirty;
    const bool isHemiSphereLightDirty = environment.m_IsHemiSphereLightDirty;
    RenderContext::MaterialHash hash = pRenderContext->m_MaterialHash;

    // the user uniforms
    res::ResMaterial userUniforms = getResource(pMaterial, Material::RESOURCE_USER_UNIFORMS);
    u32 userUniformsHash = userUniforms.ptr()->userUniformsHash;
    if (hash.m_UserUniformsHash != userUniformsHash || userUniformsHash == 0 || pRenderContext->m_IsShaderProgramChanged) {
        MaterialState::ActivateUserUniforms(pProgram, userUniforms);
        hash.m_UserUniformsHash = userUniformsHash;
    }

    // the switches of the lighting, the hemisphere lighting and the fog
    const res::ResMaterialData* pShading = getResource(pMaterial, Material::RESOURCE_SHADING).ptr();
    const bool isLighting = (pShading->flags & res::ResMaterialData::FLAG_FRAGMENT_LIGHTING) && environment.m_FragmentLightCount != 0;
    if (isLighting) {
        pProgram->m_VertexBoolUniformCommand[0] |= ShaderProgram::BOOL_FRAGMENT_LIGHTING;
    } else {
        pProgram->m_VertexBoolUniformCommand[0] &= ~ShaderProgram::BOOL_FRAGMENT_LIGHTING;
    }
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = !isLighting;
    pCommand[1] = Header(REG_LIGHTING_ENABLE1, 0x1);
    pCommand[2] = isLighting;
    pCommand[3] = Header(REG_LIGHTING_ENABLE0, 0x1);
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;

    u32 shadingHash = pShading->shadingHash;
    if (hash.m_ShadingHash != shadingHash || shadingHash == 0 || isHemiSphereLightDirty) {
        u32 flags = pShading->flags;
        if (pProgram->m_ActiveDescription.ptr()->typeInfo & res::ResShaderProgramDescriptionData::TYPE_FLAG_HEMISPHERE_LIGHTING) {
            if (environment.m_pHemiSphereLight != nullptr && (flags & res::ResMaterialData::FLAG_HEMISPHERE_LIGHTING)) {
                pProgram->m_VertexBoolUniformCommand[0] |= ShaderProgram::BOOL_HEMISPHERE_LIGHTING;
                if (flags & res::ResMaterialData::FLAG_HEMISPHERE_OCCLUSION) {
                    pProgram->m_VertexBoolUniformCommand[0] |= ShaderProgram::BOOL_HEMISPHERE_OCCLUSION;
                } else {
                    pProgram->m_VertexBoolUniformCommand[0] &= ~ShaderProgram::BOOL_HEMISPHERE_OCCLUSION;
                }
            } else {
                pProgram->m_VertexBoolUniformCommand[0] &= ~ShaderProgram::BOOL_HEMISPHERE_LIGHTING;
            }
        }
        u32 fogMode = 0;
        if ((flags & res::ResMaterialData::FLAG_FOG) && environment.m_pFog != nullptr) {
            fogMode = FOG_MODE_FOG;
        }
        pCommand = nn::gx::detail::s_pCommandBufferCurrent;
        pCommand[0] = fogMode;
        pCommand[1] = Header(REG_TEXENV_UPDATE_BUFFER, 0x1);
        nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
        hash.m_ShadingHash = shadingHash;
    }

    // the polygon offset and the culling
    res::ResMaterial rasterizationResource = getResource(pMaterial, Material::RESOURCE_RASTERIZATION);
    const res::ResMaterialData* pRasterization = rasterizationResource.ptr();
    u32 rasterizationHash = pRasterization->rasterizationHash;
    if (hash.m_RasterizationHash != rasterizationHash || rasterizationHash == 0) {
        PutRasterization(pRasterization);
        hash.m_RasterizationHash = rasterizationHash;
    }

    // the texture coordinators
    res::ResMaterial coordinators = getResource(pMaterial, Material::RESOURCE_TEXTURE_COORDINATORS);
    u32 coordinatorsHash = coordinators.ptr()->textureCoordinatorsHash;
    if (hash.m_TextureCoordinatorsHash != coordinatorsHash || coordinatorsHash == 0) {
        MaterialState::ActivateTextureCoordinators(pRenderContext, pProgram, coordinators);
        hash.m_TextureCoordinatorsHash = coordinatorsHash;
    }

    // the texture mappers and the texture units they enable
    const res::ResMaterialData* pMappers = getResource(pMaterial, Material::RESOURCE_TEXTURE_MAPPERS).ptr();
    u32 mappersHash = pMappers->textureMappersHash;
    if (hash.m_TextureMappersHash != mappersHash || mappersHash == 0) {
        u32 units = 0;
        for (int i = 0; i < 3; ++i) {
            const res::ResPixelBasedTextureMapperData* pMapper = pMappers->textureMappers[i].To<res::ResPixelBasedTextureMapperData>();
            if (pMapper != nullptr) {
                units |= 1 << i;
                std::memcpy(nn::gx::detail::s_pCommandBufferCurrent, pMapper->commands, pMapper->commandSize);
                nn::gx::detail::s_pCommandBufferCurrent += pMapper->commandSize / sizeof(u32);
            }
        }
        pCommand = nn::gx::detail::s_pCommandBufferCurrent;
        pCommand[0] = units | TEXUNIT_CONFIG_CLEAR_CACHE;
        pCommand[1] = Header(REG_TEXUNIT_CONFIG, 0x5);
        nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
        hash.m_TextureMappersHash = mappersHash;
    }

    // the fragment lighting and its lookup tables
    bool isSpecular1Raw = false;
    res::ResMaterial lightingTableResource = getResource(pMaterial, Material::RESOURCE_FRAGMENT_LIGHTING_TABLE);
    const res::ResMaterialData* pLightingResource = getResource(pMaterial, Material::RESOURCE_FRAGMENT_LIGHTING).ptr();
    u32 lightingHash = pLightingResource->fragmentLightingHash;
    u32 lightingTableHash = lightingTableResource.ptr()->fragmentLightingTableHash;
    if (pShading->flags & res::ResMaterialData::FLAG_FRAGMENT_LIGHTING) {
        const res::ResFragmentLightingData* pLighting = &pLightingResource->fragmentShader.To<res::ResFragmentShaderData>()->lighting;
        isSpecular1Raw = (pLighting->flags & res::ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) != 0;
        if (hash.m_FragmentLightingHash != lightingHash || lightingHash == 0 || isLightDirty) {
            u32 fresnelSelector = static_cast<u8>(pLighting->fresnelSelector);
            u32 config = static_cast<u8>(pLighting->config);
            u32 bumpSelector = static_cast<u8>(pLighting->bumpSelector);
            u32 bumpMode = static_cast<u8>(pLighting->bumpMode);
            bool isBumpRenormalizeOff = !(pLighting->isBumpRenormalize || bumpMode == 0);
            u32 flags = pLighting->flags;
            u32 config0 = fresnelSelector << 2 | config << 4 | bumpSelector << 22 |
                          ((flags & res::ResFragmentLightingData::FLAG_CLAMP_HIGHLIGHTS) != 0) << 27 | bumpMode << 28 |
                          isBumpRenormalizeOff << 30 | LIGHTING_CONFIG0_ALWAYS;
            // the tables that are not used are off (D0, D1, the spot of all lights, FR, the reflections)
            u32 config1 = ((flags & res::ResFragmentLightingData::FLAG_SAMPLER3) == 0) << 16 |
                          ((flags & res::ResFragmentLightingData::FLAG_SAMPLER4) == 0) << 17 | 1 << 18 |
                          (fresnelSelector == 0) << 19 |
                          ((flags & res::ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) == 0 ? 7 : 0) << 20;
            pCommand = nn::gx::detail::s_pCommandBufferCurrent;
            pCommand[0] = config0;
            pCommand[1] = Header(REG_LIGHTING_CONFIG0);
            pCommand[2] = config1;
            pCommand[3] = Header(REG_LIGHTING_CONFIG1, 0x4);
            nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;

            bool isGeometricFactor0 = (pLighting->flags & res::ResFragmentLightingData::FLAG_GEOMETRIC_FACTOR0) != 0;
            bool isGeometricFactor1 = (pLighting->flags & res::ResFragmentLightingData::FLAG_GEOMETRIC_FACTOR1) != 0;
            for (int i = 0; i < environment.m_FragmentLightCount; ++i) {
                const res::ResFragmentLightData* pLight =
                    static_cast<const res::ResFragmentLightData*>(environment.m_FragmentLights[i]->GetResSceneObject().ptr());
                bool isTwoSided = (pLight->flags & res::ResFragmentLightData::FLAG_TWO_SIDED_DIFFUSE) != 0;
                bool isDirectional = pLight->lightKind == res::ResFragmentLightData::LIGHT_KIND_DIRECTIONAL ||
                                     ((g_DirectionalLightMask >> i) & 1);
                u32 lightConfig = isDirectional ? LIGHT_CONFIG_DIRECTIONAL : 0;
                if (isTwoSided) {
                    lightConfig |= LIGHT_CONFIG_TWO_SIDED_DIFFUSE;
                }
                if (isGeometricFactor0) {
                    lightConfig |= LIGHT_CONFIG_GEOMETRIC_FACTOR0;
                }
                if (isGeometricFactor1) {
                    lightConfig |= LIGHT_CONFIG_GEOMETRIC_FACTOR1;
                }
                pCommand = nn::gx::detail::s_pCommandBufferCurrent;
                pCommand[0] = lightConfig;
                pCommand[1] = Header(REG_LIGHT0_CONFIG + REG_LIGHT_STRIDE * i, 0x1);
                nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
            }
        }
        if (hash.m_FragmentLightingTableHash != lightingTableHash || lightingTableHash == 0) {
            const res::ResFragmentShaderData* pShader = lightingTableResource.ptr()->fragmentShader.To<res::ResFragmentShaderData>();
            MaterialState::ActivateFragmentLightingTable(res::ResFragmentLighting(pLighting),
                                                                   res::ResFragmentLightingTable(pShader->lightingTable.ToPtr()));
        }
    }
    hash.m_FragmentLightingHash = lightingHash;
    hash.m_FragmentLightingTableHash = lightingTableHash;

    // the material colors with the lights (again if the active material treats the second specular
    // color differently)
    const res::ResMaterialData* pColor = getResource(pMaterial, Material::RESOURCE_MATERIAL_COLOR).ptr();
    u32 colorHash = pColor->materialColorHash;
    bool isColorSent;
    if (hash.m_MaterialColorHash != colorHash || colorHash == 0 || isLightDirty) {
        isColorSent = true;
    } else {
        isColorSent = false;
        const Material* pActive = pRenderContext->m_pActiveMaterial;
        if (pActive != nullptr) {
            res::ResMaterial active = getResource(pActive, Material::RESOURCE_FRAGMENT_LIGHTING_TABLE);
            const res::ResFragmentShaderData* pActiveShader = active.ptr()->fragmentShader.To<res::ResFragmentShaderData>();
            isColorSent = isSpecular1Raw != ((pActiveShader->lighting.flags & res::ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) != 0);
        }
    }
    if (isColorSent) {
        MaterialState::ActivateMaterialColor(environment, pProgram, res::ResMaterialColor(&pColor->materialColor), isSpecular1Raw);
        hash.m_MaterialColorHash = colorHash;
    }

    // the texture combiners with the constant colors of the material
    res::ResMaterial combinersResource = getResource(pMaterial, Material::RESOURCE_TEXTURE_COMBINERS);
    u32 combinersHash = combinersResource.ptr()->textureCombinersHash;
    if (hash.m_TextureCombinersHash != combinersHash || combinersHash == 0 || isColorSent) {
        PutTextureCombiners(combinersResource.ptr()->fragmentShader.To<res::ResFragmentShaderData>(), pColor->materialColor);
        hash.m_TextureCombinersHash = combinersHash;
    }

    // the combiner buffer
    const res::ResMaterialData* pBuffer = getResource(pMaterial, Material::RESOURCE_COMBINER_BUFFER).ptr();
    u32 bufferHash = pBuffer->combinerBufferHash;
    if (hash.m_CombinerBufferHash != bufferHash || bufferHash == 0) {
        PutCommand(pBuffer->fragmentShader.To<res::ResFragmentShaderData>()->bufferCommand, 2);
        hash.m_CombinerBufferHash = bufferHash;
    }

    // the fragment operation
    res::ResMaterial operationResource = getResource(pMaterial, Material::RESOURCE_FRAGMENT_OPERATION);
    res::ResMaterialData* pOperation = operationResource.ptr();
    u32 operationHash = pOperation->fragmentOperationHash;
    if (hash.m_FragmentOperationHash != operationHash || operationHash == 0) {
        PutFragmentOperation(pOperation);
        hash.m_FragmentOperationHash = operationHash;
    }

    pRenderContext->m_MaterialHash = hash;
}
} // namespace internal
} // namespace gfx
} // namespace nw
