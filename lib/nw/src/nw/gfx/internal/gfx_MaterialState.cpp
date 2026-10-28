#include "nw/gfx/internal/gfx_MaterialState.h"

#include "nn/gx/detail/gx_Context.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_CameraProjectionUpdater.h"
#include "nw/gfx/gfx_GraphicsDevice.h"
#include "nw/gfx/gfx_Light.h"
#include "nw/gfx/gfx_RenderContext.h"
#include "nw/gfx/gfx_SceneEnvironment.h"
#include "nw/gfx/gfx_ShaderProgram.h"
#include "nw/gfx/internal/gfx_LightingRegisters.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResCamera.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {
namespace internal {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_TEXUNIT_CONFIG = 0x080;
const u32 REG_LIGHT0_SPECULAR0 = 0x140;
const u32 REG_LIGHT_STRIDE = 0x10;
const u32 REG_LIGHTING_AMBIENT = 0x1C0;
const u32 REG_LIGHTING_NUM_LIGHTS = 0x1C2;
const u32 REG_LIGHTING_LUTINPUT_ABS = 0x1D0;
const u32 REG_LIGHTING_LIGHT_PERMUTATION = 0x1D9;
const u32 REG_GSH_FLOATUNIFORM_INDEX = 0x290;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// a float uniform index in 32-bit mode
const u32 FLOAT_UNIFORM_F32 = 0x80000000;
// the float uniforms of the gfx shaders (names are ours)
const u32 UNIFORM_TEXTURE_SOURCES = 10;       // c10 per texture unit: the coordinate it uses (or 3/4: environment)
const u32 UNIFORM_TEXTURE_MATRIX0 = 11;       // c11, c14, c17
const u32 UNIFORM_TEXTURE_TRANSLATIONS = 19;  // c19 the translations of projected textures
const u32 UNIFORM_MATERIAL_AMBIENT = 20;
const u32 UNIFORM_MATERIAL_DIFFUSE = 21;

// in TEXUNIT_CONFIG: the texture caches are cleared
const u32 TEXUNIT_CONFIG_CLEAR_CACHE = 0x1000;
// in the bool uniforms of the vertex shader (names are ours): per texture unit, it uses a texture
// coordinate of the vertices (bits 9-11); there are 2 / 3 texture units
const int BOOL_TEXTURE_UV0_SHIFT = 9;
const u32 BOOL_TEXTURE_UV_MASK = 0x7 << BOOL_TEXTURE_UV0_SHIFT;
const u32 BOOL_TEXTURE_UNIT1 = 1 << 13;
const u32 BOOL_TEXTURE_UNIT2 = 1 << 14;

// the lighting lookup tables of the GraphicsDevice
const int LUT_TARGET_D0 = 0;
const int LUT_TARGET_D1 = 1;
const int LUT_TARGET_FR = 3;
const int LUT_TARGET_RB = 4;
const int LUT_TARGET_RG = 5;
const int LUT_TARGET_RR = 6;

// the samplers of a fragment lighting table (ResFragmentLightingTableData::samplers)
const int SAMPLER_RR = 0;
const int SAMPLER_RG = 1;
const int SAMPLER_RB = 2;
const int SAMPLER_D0 = 3;
const int SAMPLER_D1 = 4;
const int SAMPLER_FR = 5;

// a uniform of the values of a type to the shader of the float uniform register (a bool: a bit of
// its bool uniforms)
inline void PutUniformValue(u32* pBools, u32 indexRegister, s32 reg, u8 type, const f32* pValues)
{
    if (type == res::ResUserUniformData::TYPE_BOOL) {
        if (*reinterpret_cast<const u32*>(pValues) != 0) {
            *pBools |= 1 << reg;
        } else {
            *pBools &= ~(1 << reg);
        }
        return;
    }
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    f32* pData = reinterpret_cast<f32*>(pCommand + 2);
    switch (type) {
    case res::ResUserUniformData::TYPE_INT:
        pCommand[0] = FLOAT_UNIFORM_F32 + reg;
        pCommand[1] = Header(indexRegister, 0xF, 4, true);
        NWCopyVec1Reverse(pData, pValues);
        break;
    case res::ResUserUniformData::TYPE_VEC2:
        pCommand[0] = FLOAT_UNIFORM_F32 + reg;
        pCommand[1] = Header(indexRegister, 0xF, 4, true);
        NWCopyVec2Reverse(pData, pValues);
        break;
    case res::ResUserUniformData::TYPE_VEC3:
        pCommand[0] = FLOAT_UNIFORM_F32 + reg;
        pCommand[1] = Header(indexRegister, 0xF, 4, true);
        NWCopyVec3Reverse(pData, pValues);
        break;
    case res::ResUserUniformData::TYPE_VEC4:
        pCommand[0] = FLOAT_UNIFORM_F32 + reg;
        pCommand[1] = Header(indexRegister, 0xF, 4, true);
        NWCopyVec4Reverse(pData, pValues);
        break;
    default:
        return;
    }
    nn::gx::detail::s_pCommandBufferCurrent += 6;
}

// the value of a symbol: to the geometry or the vertex shader
inline void PutSymbolValue(ShaderProgram* pProgram, const res::ResShaderSymbolData* pSymbol, u8 type, const f32* pValues)
{
    if (pSymbol->isGeometry) {
        PutUniformValue(&pProgram->m_GeometryBoolUniformCommand[0], REG_GSH_FLOATUNIFORM_INDEX, pSymbol->index, type, pValues);
    } else {
        PutUniformValue(&pProgram->m_VertexBoolUniformCommand[0], REG_VSH_FLOATUNIFORM_INDEX, pSymbol->index, type, pValues);
    }
}

// a float uniform of up to four values to the vertex shader
inline void PutVertexUniform4(u32 index, const f32* pValues)
{
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = FLOAT_UNIFORM_F32 | index;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 4, true);
    NWCopyVec4Reverse(reinterpret_cast<f32*>(pCommand + 2), pValues);
    nn::gx::detail::s_pCommandBufferCurrent += 6;
}
inline void PutVertexUniform3(u32 index, const f32* pValues)
{
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = FLOAT_UNIFORM_F32 | index;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 4, true);
    NWCopyVec3Reverse(reinterpret_cast<f32*>(pCommand + 2), pValues);
    nn::gx::detail::s_pCommandBufferCurrent += 6;
}

// two 8-bit colors (ut::Color8 words) multiplied as the registers of the lights want them: red in
// bits 20-27, green in 10-17, blue in 0-7
inline u32 ModulateColor(u32 a, u32 b)
{
    const u32 red = ((1 + 2 * (a & 0xFF)) * (1 + 2 * (b & 0xFF))) >> 10;
    const u32 green = ((1 + ((a & 0xFF00) >> 7)) * (1 + ((b & 0xFF00) >> 7))) >> 10;
    const u32 blue = ((1 + ((a & 0xFF0000) >> 15)) * (1 + ((b & 0xFF0000) >> 15))) >> 10;
    return (red << 20) | (green << 10) | blue;
}
// an 8-bit color in the layout of the registers of the lights
inline u32 ToLightColor(u32 color)
{
    return ((color << 24) >> 4) | (((color << 16) >> 24) << 10) | ((color << 8) >> 24);
}

// the first three rows of a texture matrix
inline void CopyTextureMatrix(nn::math::MTX34* pOut, const nn::math::MTX44& matrix)
{
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 4; ++column) {
            pOut->m[row][column] = matrix.m[row][column];
        }
    }
}

// the texture matrix of a coordinator, made again if it is dirty
inline void GetTextureMatrix(nn::math::MTX34* pMatrix, res::ResTextureCoordinatorData* pCoordinator)
{
    if (pCoordinator->isDirty) {
        nn::math::MTX44 textureMatrix;
        MaterialState::SetupTextureMatrix(&textureMatrix,
                                          static_cast<res::ResTextureCoordinator::MappingMatrixMode>(pCoordinator->mappingMatrixMode),
                                          pCoordinator->scale.x, pCoordinator->scale.y, pCoordinator->rotate,
                                          pCoordinator->translate.x, pCoordinator->translate.y);
        CopyTextureMatrix(pMatrix, textureMatrix);
        pCoordinator->textureMatrix = *pMatrix;
        pCoordinator->isDirty = false;
    } else {
        *pMatrix = pCoordinator->textureMatrix;
    }
}

// the configuration of the texture units for the coordinators of a material
inline void PutTextureUnitConfig(const res::ResMaterialData* pMaterial)
{
    const u32 configs[] = {0x0000, 0x2000, 0x2100, 0x2200, 0x0100, 0x0200};
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = configs[static_cast<u8>(pMaterial->textureCoordinateConfig)] | TEXUNIT_CONFIG_CLEAR_CACHE;
    pCommand[1] = Header(REG_TEXUNIT_CONFIG, 0xA);
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
}

// a float uniform of 1 to 4 vectors to the vertex shader (the original has all four cases where only
// two are used; name is ours)
inline void PutVertexUniformRows(u32 index, int rows, const f32* pValues)
{
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    switch (rows) {
    case 1:
        pCommand[0] = FLOAT_UNIFORM_F32 + index;
        pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 4, true);
        NWCopyVec4Reverse(reinterpret_cast<f32*>(pCommand + 2), pValues);
        nn::gx::detail::s_pCommandBufferCurrent += 6;
        break;
    case 2:
        pCommand[0] = FLOAT_UNIFORM_F32 + index;
        pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 8, true);
        NWCopyMtx24Reverse(reinterpret_cast<f32*>(pCommand + 2), pValues);
        nn::gx::detail::s_pCommandBufferCurrent += 10;
        break;
    case 3:
        pCommand[0] = FLOAT_UNIFORM_F32 + index;
        pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
        NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pCommand + 2), pValues, Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11));
        nn::gx::detail::s_pCommandBufferCurrent += 16;
        break;
    case 4:
        pCommand[0] = FLOAT_UNIFORM_F32 + index;
        pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
        NWCopyMtx44WithHeader(reinterpret_cast<f32*>(pCommand + 2), pValues, Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 15));
        nn::gx::detail::s_pCommandBufferCurrent += 20;
        break;
    }
}

// the matrix of a texture unit (the third unit has only two rows)
inline void PutTextureMatrix(int unit, const nn::math::MTX34& matrix)
{
    PutVertexUniformRows(UNIFORM_TEXTURE_MATRIX0 + unit * 3, unit == 2 ? 2 : 3, reinterpret_cast<const f32*>(&matrix));
}

// a lighting lookup table loaded with its input settings (absolute value bit, select and scale at shift)
inline void ActivateLightingSampler(const res::ResLightingLookupTableData* pSampler, u32 absBit, int shift, int target)
{
    const res::ResImageLookupTableData* pTable = res::ResLookupTable(pSampler->sampler.ToPtr()).Dereference();
    if (pTable->isAbsolute) {
        g_LightingLutInputAbs &= ~absBit;
    } else {
        g_LightingLutInputAbs |= absBit;
    }
    g_LightingLutInputSelect = (g_LightingLutInputSelect & ~(0x7 << shift)) | (static_cast<u8>(pSampler->input) << shift);
    g_LightingLutInputScale = (g_LightingLutInputScale & ~(0x7 << shift)) | (static_cast<u8>(pSampler->scale) << shift);
    GraphicsDevice::ActivateLookupTable(res::ResImageLookupTable(pTable), static_cast<GraphicsDevice::LutTarget>(target));
}
} // namespace

// 0x004B0A94 | nintendogs:bytes-fuzzy [tier A]
nn::math::MTX44* nw::gfx::internal::MaterialState::SetupTextureMatrix(nn::math::MTX44* pMatrix, res::ResTextureCoordinator::MappingMatrixMode mode,
                                                                    f32 scaleS, f32 scaleT, f32 rotate, f32 translateS, f32 translateT)
{
    nn::math::MTX44& m = *pMatrix;
    f32 sin;
    f32 cos;
    switch (mode) {
    case res::ResTextureCoordinator::MAPPING_MATRIX_MODE_MAYA:
        nn::math::SinCosFIdx(&sin, &cos, rotate * (256.0f / (2.0f * 3.14159265f)));
        m.m[0][0] = scaleS * cos;
        m.m[1][1] = scaleT * cos;
        m.m[3][2] = 0.0f;
        m.m[3][3] = 1.0f;
        m.m[3][1] = 0.0f;
        m.m[3][0] = 0.0f;
        m.m[2][3] = 0.0f;
        m.m[2][1] = 0.0f;
        m.m[2][0] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[0][2] = 0.0f;
        m.m[2][2] = 1.0f;
        m.m[0][1] = -scaleS * sin;
        m.m[1][0] = scaleT * sin;
        m.m[0][3] = ((0.5f * sin - 0.5f * cos) + 0.5f - translateS) * scaleS;
        m.m[1][3] = ((-0.5f * sin - 0.5f * cos) + 0.5f - translateT) * scaleT;
        break;
    case res::ResTextureCoordinator::MAPPING_MATRIX_MODE_SOFTIMAGE:
        nn::math::SinCosFIdx(&sin, &cos, rotate * (256.0f / (2.0f * 3.14159265f)));
        m.m[0][0] = scaleS * cos;
        m.m[1][1] = scaleT * cos;
        m.m[3][2] = 0.0f;
        m.m[3][3] = 1.0f;
        m.m[3][1] = 0.0f;
        m.m[3][0] = 0.0f;
        m.m[2][3] = 0.0f;
        m.m[2][1] = 0.0f;
        m.m[2][0] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[0][2] = 0.0f;
        m.m[2][2] = 1.0f;
        m.m[0][1] = scaleS * sin;
        m.m[1][0] = -scaleT * sin;
        m.m[0][3] = (-cos * translateS - sin * translateT) * scaleS;
        m.m[1][3] = (sin * translateS - cos * translateT) * scaleT;
        break;
    case res::ResTextureCoordinator::MAPPING_MATRIX_MODE_MAX: {
        nn::math::SinCosFIdx(&sin, &cos, rotate * (256.0f / (2.0f * 3.14159265f)));
        const f32 s = -translateS - 0.5f;
        const f32 t = translateT - 0.5f;
        m.m[3][2] = 0.0f;
        m.m[3][3] = 1.0f;
        m.m[3][1] = 0.0f;
        m.m[3][0] = 0.0f;
        m.m[2][3] = 0.0f;
        m.m[2][1] = 0.0f;
        m.m[2][0] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[0][2] = 0.0f;
        m.m[2][2] = 1.0f;
        m.m[0][1] = -scaleS * sin;
        m.m[0][0] = scaleS * cos;
        m.m[1][0] = scaleT * sin;
        m.m[1][1] = scaleT * cos;
        m.m[0][3] = (scaleS * cos * s - scaleS * sin * t) + 0.5f;
        m.m[1][3] = (scaleT * sin * s + scaleT * cos * t) + 0.5f;
        break;
    }
    default:
        // (ARMCC inlines MTX44SetIdentity here: the static identity copied with MTX44CopyAsm)
        nn::math::MTX44SetIdentity(pMatrix);
        break;
    }
    return pMatrix;
}

// 0x004B0D2C | nintendogs:bytes [tier A]
void nw::gfx::internal::MaterialState::ActivateMaterialColor(const SceneEnvironment& environment, const ShaderProgram* /*pProgram*/,
                                                             res::ResMaterialColor color, bool isSpecular1Raw)
{
    const res::ResMaterialColorData* pColor = color.ptr();
    PutVertexUniform4(UNIFORM_MATERIAL_DIFFUSE, &pColor->diffuse.r);
    PutVertexUniform4(UNIFORM_MATERIAL_AMBIENT, &pColor->ambient.r);

    // the lights in their order
    int lightCount = environment.m_FragmentLightCount;
    if (lightCount > SceneEnvironment::FRAGMENT_LIGHT_MAX) {
        lightCount = SceneEnvironment::FRAGMENT_LIGHT_MAX;
    }
    u32 permutation = 0;
    for (int i = 0; i < lightCount; ++i) {
        permutation |= i << (i * 4);
    }
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = lightCount > 0 ? lightCount - 1 : 0;
    pCommand[1] = Header(REG_LIGHTING_NUM_LIGHTS, 0x1);
    pCommand[2] = permutation;
    pCommand[3] = Header(REG_LIGHTING_LIGHT_PERMUTATION);
    pCommand += 4;

    // the global ambient: the emission and the ambient light
    u32 ambient;
    if (environment.m_pAmbientLight != nullptr) {
        const res::ResAmbientLightData* pLight =
            static_cast<const res::ResAmbientLightData*>(environment.m_pAmbientLight->GetResSceneObject().ptr());
        const u32 lightColor = pLight->colorU32;
        const u32 materialAmbient = pColor->ambientU8;
        const u32 emission = pColor->emissionU8;
        u32 red = (emission & 0xFF) + (((1 + 2 * (lightColor & 0xFF)) * (1 + 2 * (materialAmbient & 0xFF))) >> 10);
        u32 green = ((emission << 16) >> 24) + (((1 + ((lightColor & 0xFF00) >> 7)) * (1 + ((materialAmbient & 0xFF00) >> 7))) >> 10);
        if (red > 0xFF) {
            red = 0xFF;
        }
        if (green > 0xFF) {
            green = 0xFF;
        }
        u32 blue = ((emission << 8) >> 24) + (((1 + ((lightColor & 0xFF0000) >> 15)) * (1 + ((materialAmbient & 0xFF0000) >> 15))) >> 10);
        if (blue > 0xFF) {
            blue = 0xFF;
        }
        ambient = (red << 20) | (green << 10) | blue;
    } else {
        ambient = ToLightColor(pColor->emissionU8);
    }
    pCommand[0] = ambient;
    pCommand[1] = Header(REG_LIGHTING_AMBIENT);
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;

    // the colors of each light with the material
    for (int i = 0; i < lightCount; ++i) {
        const res::ResFragmentLightData* pLight =
            static_cast<const res::ResFragmentLightData*>(environment.m_FragmentLights[i]->GetResSceneObject().ptr());
        u32 lightCommand[ShaderProgram::BOOL_UNIFORM_COMMAND_WORDS];
        lightCommand[0] = ModulateColor(pLight->specular0U8, pColor->specular0U8);
        lightCommand[1] = Header(REG_LIGHT0_SPECULAR0 + REG_LIGHT_STRIDE * i, 0xF, 3, true);
        lightCommand[2] = isSpecular1Raw ? ToLightColor(pLight->specular1U8) : ModulateColor(pColor->specular1U8, pLight->specular1U8);
        lightCommand[3] = ModulateColor(pLight->diffuseU8, pColor->diffuseU8);
        lightCommand[4] = ModulateColor(pLight->ambientU8, pColor->ambientU8);
        lightCommand[5] = 0;
        nn::gx::detail::s_pCommandBufferCurrent = FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, lightCommand);
    }
}

// 0x004B1108 (name is ours)
void nw::gfx::internal::MaterialState::ActivateUserUniforms(ShaderProgram* pProgram, res::ResMaterial material)
{
    bool isSet[64] = {};
    const res::ResShaderProgramDescriptionData* pDescription = pProgram->m_ActiveDescription.ptr();
    const ut::Offset* pSymbols = pDescription->symbols.array.To<ut::Offset>();
    const int symbolCount = pDescription->symbols.count;

    // the uniforms of the material
    const ut::Offset* pBegin = material.ptr()->userUniforms.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + material.ptr()->userUniforms.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        const res::ResUserUniformData* pUniform = pIt->To<res::ResUserUniformData>();
        const int symbolIndex = pUniform->symbolIndex;
        if (symbolIndex < 0 || symbolIndex >= symbolCount) {
            continue;
        }
        const res::ResShaderSymbolData* pSymbol = pSymbols[symbolIndex].To<res::ResShaderSymbolData>();
        if (pSymbol->isEnabled && pSymbol->index >= 0) {
            PutSymbolValue(pProgram, pSymbol, static_cast<u8>(pUniform->type), pUniform->values);
        }
        isSet[symbolIndex] = true;
    }

    // the defaults of the other symbols
    for (int i = 0; i < symbolCount; ++i) {
        if (isSet[i]) {
            continue;
        }
        const res::ResShaderSymbolData* pSymbol = pSymbols[i].To<res::ResShaderSymbolData>();
        if (pSymbol->isEnabled && pSymbol->index >= 0) {
            PutSymbolValue(pProgram, pSymbol, static_cast<u8>(pSymbol->type), pSymbol->values);
        }
    }
}

// 0x004B16B4 (name is ours)
void nw::gfx::internal::MaterialState::ActivateTextureCoordinators(RenderContext* pRenderContext, ShaderProgram* pProgram,
                                                                   res::ResMaterial material)
{
    res::ResMaterialData* pMaterial = material.ptr();
    PutTextureUnitConfig(pMaterial);
    f32 sources[3] = {0.0f, 0.0f, 0.0f};
    f32 translations[4];
    const int count = pMaterial->textureCoordinatorCount;
    u32 bools = pProgram->m_VertexBoolUniformCommand[0] & ~(BOOL_TEXTURE_UV_MASK & ~(1 << (BOOL_TEXTURE_UV0_SHIFT + 2)));
    pProgram->m_VertexBoolUniformCommand[0] = bools;
    bools = count > 1 ? (bools | BOOL_TEXTURE_UNIT1) : (bools & ~BOOL_TEXTURE_UNIT1);
    bools &= ~(1 << (BOOL_TEXTURE_UV0_SHIFT + 2));
    pProgram->m_VertexBoolUniformCommand[0] = bools;
    pProgram->m_VertexBoolUniformCommand[0] = count > 2 ? (bools | BOOL_TEXTURE_UNIT2) : (bools & ~BOOL_TEXTURE_UNIT2);

    bool isTranslationUsed = false;
    SceneEnvironment& environment = pRenderContext->m_SceneEnvironment;
    for (int i = 0; i < count; ++i) {
        res::ResTextureCoordinatorData* pCoordinator = &pMaterial->textureCoordinators[i];
        if (!pCoordinator->isEnabled) {
            continue;
        }
        nn::math::MTX34 matrix;
        GetTextureMatrix(&matrix, pCoordinator);

        const u8 method = static_cast<u8>(pCoordinator->mappingMethod);
        switch (method) {
        case res::ResTextureCoordinatorData::MAPPING_UV:
            sources[i] = static_cast<f32>(pCoordinator->sourceCoordinate);
            pProgram->m_VertexBoolUniformCommand[0] |= 1 << (i + BOOL_TEXTURE_UV0_SHIFT);
            break;
        case res::ResTextureCoordinatorData::MAPPING_CUBE_ENV:
            sources[i] = static_cast<f32>(method) + 2.0f;
            if (environment.m_pCamera != nullptr) {
                matrix = environment.m_pCamera->m_InverseViewMatrix;
            }
            break;
        case res::ResTextureCoordinatorData::MAPPING_SPHERE_ENV:
            sources[i] = static_cast<f32>(method) + 2.0f;
            break;
        case res::ResTextureCoordinatorData::MAPPING_PROJECTION: {
            Camera* pCamera = pCoordinator->referenceCamera < 0 ? environment.m_pCamera : environment.m_Cameras[pCoordinator->referenceCamera];
            if (i == 0) {
                translations[0] = matrix.m[0][3];
                translations[1] = matrix.m[1][3];
            } else {
                translations[2] = matrix.m[0][3];
                translations[3] = matrix.m[1][3];
            }
            matrix.m[0][3] = 0.0f;
            matrix.m[1][3] = 0.0f;
            nn::math::ARMv6::MTX34MultAsm(&matrix, &matrix, &pCamera->m_TextureProjectionMatrix);
            nn::math::ARMv6::MTX34MultAsm(&matrix, &matrix, &pCamera->m_ViewMatrix);
            isTranslationUsed = true;
            break;
        }
        case res::ResTextureCoordinatorData::MAPPING_SHADOW: {
            Camera* pCamera = pCoordinator->referenceCamera < 0 ? environment.m_pCamera : environment.m_Cameras[pCoordinator->referenceCamera];
            translations[0] = matrix.m[0][3];
            translations[1] = matrix.m[1][3];
            matrix.m[0][3] = 0.0f;
            matrix.m[1][3] = 0.0f;
            const f32 near = pCamera->GetNear();
            const f32 far = pCamera->GetFar();
            const res::ResCameraProjectionUpdaterData* pProjection =
                static_cast<const CameraProjectionUpdater*>(pCamera->m_ProjectionUpdater.Get())->GetResource().ptr();
            nn::math::MTX34 textureProjection(pCamera->m_TextureProjectionMatrix);
            switch (pProjection->typeInfo) {
            case res::ResFrustumProjectionUpdaterData::TYPE:
            case res::ResPerspectiveProjectionUpdaterData::TYPE: {
                // the depth over the range of the camera
                const f32 scale = 1.0f / (far - near);
                nn::math::MTX34 depthScale(nn::math::MTX34::Identity());
                depthScale.m[0][0] = scale;
                depthScale.m[0][1] = 0.0f;
                depthScale.m[0][2] = 0.0f;
                depthScale.m[0][3] = 0.0f;
                depthScale.m[1][0] = 0.0f;
                depthScale.m[1][1] = scale;
                depthScale.m[1][2] = 0.0f;
                depthScale.m[1][3] = 0.0f;
                depthScale.m[2][0] = 0.0f;
                depthScale.m[2][1] = 0.0f;
                depthScale.m[2][2] = scale;
                depthScale.m[2][3] = 0.0f;
                nn::math::ARMv6::MTX34MultAsm(&matrix, &matrix, &depthScale);
                break;
            }
            case res::ResOrthoProjectionUpdaterData::TYPE:
                textureProjection.m[2][2] = -1.0f / (far - near);
                textureProjection.m[2][3] = near * textureProjection.m[2][2];
                break;
            default:
                break;
            }
            nn::math::ARMv6::MTX34MultAsm(&matrix, &matrix, &textureProjection);
            nn::math::ARMv6::MTX34MultAsm(&matrix, &matrix, &pCamera->m_ViewMatrix);
            isTranslationUsed = true;
            break;
        }
        default:
            break;
        }
        PutTextureMatrix(i, matrix);
    }

    if (isTranslationUsed) {
        PutVertexUniform4(UNIFORM_TEXTURE_TRANSLATIONS, translations);
    }
    PutVertexUniform3(UNIFORM_TEXTURE_SOURCES, sources);
}

// 0x004B1D14 | nintendogs (C)
void nw::gfx::internal::MaterialState::ActivateFragmentLightingTable(res::ResFragmentLighting lighting, res::ResFragmentLightingTable table)
{
    const res::ResFragmentLightingData* pLighting = lighting.ptr();
    const bool isD0Used = (pLighting->flags & res::ResFragmentLightingData::FLAG_SAMPLER3) != 0;
    const bool isD1Used = (pLighting->flags & res::ResFragmentLightingData::FLAG_SAMPLER4) != 0;
    const bool isReflectionUsed = (pLighting->flags & res::ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) != 0;
    const bool isFresnelUsed = static_cast<u8>(pLighting->fresnelSelector) != 0;
    const res::ResFragmentLightingTableData* pTable = table.ptr();

    const res::ResLightingLookupTableData* pSampler = pTable->samplers[SAMPLER_RR].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isReflectionUsed) {
        ActivateLightingSampler(pSampler, 1 << 25, 24, LUT_TARGET_RR);
    }
    pSampler = pTable->samplers[SAMPLER_RG].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isReflectionUsed) {
        ActivateLightingSampler(pSampler, 1 << 21, 20, LUT_TARGET_RG);
    }
    pSampler = pTable->samplers[SAMPLER_RB].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isReflectionUsed) {
        ActivateLightingSampler(pSampler, 1 << 17, 16, LUT_TARGET_RB);
    }
    pSampler = pTable->samplers[SAMPLER_D0].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isD0Used) {
        ActivateLightingSampler(pSampler, 1 << 1, 0, LUT_TARGET_D0);
    }
    pSampler = pTable->samplers[SAMPLER_D1].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isD1Used) {
        ActivateLightingSampler(pSampler, 1 << 5, 4, LUT_TARGET_D1);
    }
    pSampler = pTable->samplers[SAMPLER_FR].To<res::ResLightingLookupTableData>();
    if (pSampler != nullptr && isFresnelUsed) {
        ActivateLightingSampler(pSampler, 1 << 13, 12, LUT_TARGET_FR);
    }

    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = g_LightingLutInputAbs;
    pCommand[1] = Header(REG_LIGHTING_LUTINPUT_ABS, 0xF, 2, true);
    pCommand[2] = g_LightingLutInputSelect;
    pCommand[3] = g_LightingLutInputScale;
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;
}

// 0x004B2134 | nintendogs:bytes [tier A]
void nw::gfx::internal::MaterialState::ActivateParticleTextureCoordinators(RenderContext* /*pRenderContext*/, const ShaderProgram* pProgram,
                                                                           res::ResMaterial material)
{
    ShaderProgram* pWritableProgram = const_cast<ShaderProgram*>(pProgram);
    res::ResMaterialData* pMaterial = material.ptr();
    PutTextureUnitConfig(pMaterial);
    f32 sources[3] = {0.0f, 0.0f, 0.0f};
    res::ResTextureCoordinatorData* pCoordinator = &pMaterial->textureCoordinators[0];
    pWritableProgram->m_VertexBoolUniformCommand[0] &= ~(1 << BOOL_TEXTURE_UV0_SHIFT);
    if (pCoordinator->isEnabled) {
        nn::math::MTX34 matrix;
        GetTextureMatrix(&matrix, pCoordinator);
        sources[0] = static_cast<f32>(pCoordinator->sourceCoordinate);
        pWritableProgram->m_VertexBoolUniformCommand[0] |= 1 << BOOL_TEXTURE_UV0_SHIFT;
        PutTextureMatrix(0, matrix);
    }
    PutVertexUniform3(UNIFORM_TEXTURE_SOURCES, sources);
}

} // namespace internal
} // namespace gfx
} // namespace nw
