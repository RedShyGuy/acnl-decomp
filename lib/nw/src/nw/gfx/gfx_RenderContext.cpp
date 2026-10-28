#include "nw/gfx/gfx_RenderContext.h"

#include <new>

#include "nn/gx/detail/gx_Context.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_GraphicsDevice.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_ParticleMaterialActivator.h"
#include "nw/gfx/gfx_RenderContext_Builder.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/internal/gfx_LightingRegisters.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {

namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_TEXENV_UPDATE_BUFFER = 0x0E0;
const u32 REG_FOG_COLOR = 0x0E1;
const u32 REG_LIGHT0_VECTOR_LOW = 0x144;
const u32 REG_LIGHT0_ATTENUATION_BIAS = 0x14A;
const u32 REG_LIGHT_STRIDE = 0x10;
const u32 REG_LIGHTING_CONFIG1 = 0x1C4;
const u32 REG_GSH_FLOATUNIFORM_INDEX = 0x290;
const u32 REG_GSH_FLOATUNIFORM_DATA = 0x291;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// a float uniform index in 32-bit mode
const u32 FLOAT_UNIFORM_F32 = 0x80000000;
// the float uniforms of the gfx shaders (names are ours)
const u32 UNIFORM_MODEL_MATRIX = 0;         // c0-c2 (the identity unless the skeleton is in model space)
const u32 UNIFORM_NORMAL_MATRIX = 3;        // c3-c5 (Model::m_NormalMatrix)
const u32 UNIFORM_HEMISPHERE_GROUND = 22;
const u32 UNIFORM_HEMISPHERE_SKY = 23;
const u32 UNIFORM_HEMISPHERE_DIRECTION = 24;
const u32 UNIFORM_PROJECTION_MATRIX = 86;   // c86-c89
const u32 UNIFORM_VIEW_MATRIX = 90;         // c90-c92

// in TEXENV_UPDATE_BUFFER: the fog is flipped in z
const u32 FOG_Z_FLIP = 1 << 16;
// the lighting lookup tables of the GraphicsDevice: spot (7 + light) and distance attenuation (15 + light)
const int LUT_TARGET_SPOT0 = 7;
const int LUT_TARGET_DISTANCE_ATTENUATION0 = 15;
// in LIGHTING_LUTINPUT_*: the spot table
const u32 LUT_INPUT_ABS_SPOT_DISABLE = 1 << 9;
const u32 LUT_INPUT_SPOT_SHIFT = 8;
const u32 LUT_INPUT_SPOT_MASK = 0x7 << LUT_INPUT_SPOT_SHIFT;

// a float uniform of four values to the current command list (index, then the values w first)
inline void PutVertexUniform(u32 index, const f32* pValues)
{
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = FLOAT_UNIFORM_F32 | index;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 4, true);
    internal::NWCopyVec4Reverse(reinterpret_cast<f32*>(pCommand + 2), pValues);
    nn::gx::detail::s_pCommandBufferCurrent += 6;
}

// a matrix as three float uniforms
inline void PutVertexMatrix34(u32 index, const nn::math::MTX34& matrix)
{
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[0] = FLOAT_UNIFORM_F32 | index;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
    internal::NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pCommand + 2), reinterpret_cast<const f32*>(&matrix),
                                    Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11));
    nn::gx::detail::s_pCommandBufferCurrent += 16;
}

// a float as a 1.5.10 float (too large: infinity without sign)
inline u16 ToFloat16(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    const u32 sign = (bits.u & 0x80000000) ? 0x8000 : 0;
    const u32 mantissa = ((bits.u << 9) >> 9) >> 13;
    s32 exponent = 0;
    if ((bits.u & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits.u << 1) >> 24) - 112;
        if (exponent < 0) {
            return sign;
        }
        if (exponent > 31) {
            return 0x7C00;
        }
    }
    return mantissa | sign | ((exponent & 31) << 10);
}

// a value of -2.0 to 2.0 as a 1.1.11 fixed-point number (the spot direction)
inline u32 ToFixed13(f32 value)
{
    f32 fixed = (value + 2.0f) * 2048.0f;
    if (fixed < 0.0f) {
        fixed = 0.0f;
    } else if (fixed >= 8192.0f) {
        fixed = 8191.0f;
    }
    return static_cast<s32>(fixed - 4096.0f) & 0x1FFF;
}

// the direction in w = 0 / the position with w = 1 of a light
inline void SetDirectionalLightBit(u32 bit, f32 w)
{
    if (w == 0.0f) {
        internal::g_DirectionalLightMask |= bit;
    } else {
        internal::g_DirectionalLightMask &= ~bit;
    }
}
} // namespace

// 0x00495AFC | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ResetState(int flags, int hashFlags)
{
    if (flags & RESET_MODEL) {
        m_pModel = nullptr;
    }
    if (flags & RESET_MATERIAL) {
        m_pMaterial = nullptr;
    }
    if (flags & RESET_ACTIVE_MATERIAL) {
        m_pActiveMaterial = nullptr;
    }
    if (flags & RESET_SCENE_CAMERA) {
        m_SceneEnvironment.m_ActiveCameraIndex = -1;
        m_SceneEnvironment.m_pCamera = nullptr;
    }
    if (flags & RESET_CAMERA) {
        m_pCamera = nullptr;
    }
    if (flags & RESET_SCENE_ENVIRONMENT) {
        m_SceneEnvironment.Reset();
    }
    if (flags & RESET_SHADER_PROGRAM) {
        ShaderProgram* pProgram = m_ShaderProgram.Get();
        pProgram->m_GeometryMode = ShaderProgram::GEOMETRY_MODE_NONE;
        pProgram->m_Unknown0xC = 0;
        pProgram->m_ActiveDescription = res::ResShaderProgramDescription(nullptr);
        m_IsShaderProgramChanged = true;
    }
    if (flags & RESET_LOOKUP_TABLES) {
        GraphicsDevice::InvalidateAllLookupTables();
    }
    if (flags & RESET_MATERIAL_HASH) {
        m_MaterialHash.ResetMaterialHash(hashFlags);
    }
    if (flags & RESET_VERTEX_ATTRIBUTE) {
        m_IsVertexAttributeDirty = true;
    }
}

// 0x00495BBC | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ResetState()
{
    m_pModel = nullptr;
    m_pMaterial = nullptr;
    m_pActiveMaterial = nullptr;
    m_pCamera = nullptr;
    m_SceneEnvironment.Reset();
    m_IsVertexAttributeDirty = true;
    ShaderProgram* pProgram = m_ShaderProgram.Get();
    pProgram->m_GeometryMode = ShaderProgram::GEOMETRY_MODE_NONE;
    pProgram->m_Unknown0xC = 0;
    pProgram->m_ActiveDescription = res::ResShaderProgramDescription(nullptr);
    GraphicsDevice::InvalidateAllLookupTables();
    m_MaterialHash.ResetMaterialHash(0x1FFF);
}

// 0x00495C10 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateFog()
{
    Fog* pFog = m_SceneEnvironment.m_pFog;
    if (pFog == nullptr) {
        return;
    }
    const res::ResFogData* pResource = static_cast<const res::ResFogData*>(pFog->GetResSceneObject().ptr());
    const void* pSampler = pResource->sampler.ToPtr();
    const u32 color = pResource->color.ToPicaU32() & 0x00FFFFFF;
    const u32 zFlip = (pResource->flags & res::ResFogData::FLAG_Z_FLIP) ? FOG_Z_FLIP : 0;
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[3] = Header(REG_FOG_COLOR);
    pCommand[0] = zFlip;
    pCommand[1] = Header(REG_TEXENV_UPDATE_BUFFER, 0x4);
    pCommand[2] = color;
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;
    GraphicsDevice::ActivateLookupTable(res::ResImageLookupTable(pSampler), GraphicsDevice::LUT_TARGET_FOG);
}

// 0x00495E6C | nintendogs:callseq [tier A]
void nw::gfx::RenderContext::ActivateContext(IMaterialActivator* pActivator)
{
    if (m_pActiveMaterial == m_pMaterial) {
        return;
    }
    if (!(m_RenderModeFlags & RENDER_MODE_NO_SHADER_PROGRAM)) {
        res::ResShaderProgramDescription description = m_pMaterial->GetShaderProgramDescription();
        res::ResShaderProgramDescription active = m_ShaderProgram->m_ActiveDescription;
        if (active.ptr() == description.ptr()) {
            m_IsShaderProgramChanged = false;
        } else {
            // from a geometry shader back to the vertex shader alone: everything is sent again
            if (active.ptr() != nullptr && active.ptr()->geometryShaderIndex >= 0 && description.ptr()->geometryShaderIndex < 0) {
                m_IsVertexAttributeDirty = true;
                m_MaterialHash.ResetMaterialHash(0x1015);
                m_SceneEnvironment.m_IsFogDirty = true;
                m_SceneEnvironment.m_IsAmbientLightDirty = true;
                m_SceneEnvironment.m_IsFragmentLightDirty = true;
                m_SceneEnvironment.m_IsHemiSphereLightDirty = true;
                m_pModel = nullptr;
            }
            m_ShaderProgram->ActivateDescription(description);
            m_IsShaderProgramChanged = true;
        }
    }
    ActivateSceneEnvironment();
    if (!(m_RenderModeFlags & RENDER_MODE_NO_MATERIAL)) {
        if (pActivator == nullptr) {
            pActivator = m_pMaterial->GetOwner()->GetMaterialActivator();
        }
        pActivator->Activate(this, m_pMaterial);
    }
    m_SceneEnvironment.m_IsFogDirty = false;
    m_SceneEnvironment.m_IsAmbientLightDirty = false;
    m_SceneEnvironment.m_IsFragmentLightDirty = false;
    m_SceneEnvironment.m_IsHemiSphereLightDirty = false;
    m_pActiveMaterial = m_pMaterial;
}

// 0x00495F6C | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::RenderPrimitive(res::ResPrimitive primitive)
{
    ShaderProgram* pProgram = m_ShaderProgram.Get();
    nn::gx::detail::s_pCommandBufferCurrent =
        internal::FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, pProgram->m_VertexBoolUniformCommand);
    if (pProgram->m_ActiveDescription.ptr()->geometryShaderIndex >= 0) {
        nn::gx::detail::s_pCommandBufferCurrent =
            internal::FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, pProgram->m_GeometryBoolUniformCommand);
    }
    if (!(primitive.ptr()->flags & res::ResPrimitiveData::FLAG_DRAW_COMMAND_DONE)) {
        primitive.SetupDrawCommand(m_ShaderProgram->m_ActiveDescription.ptr()->pGeometryShader != nullptr);
    }
    const ut::Offset* pBegin = primitive.ptr()->indexStreams.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + primitive.ptr()->indexStreams.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        const res::ResIndexStreamData* pStream = pIt->To<res::ResIndexStreamData>();
        if (pStream->isEnabled) {
            const u32 size = pStream->commandSize;
            nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, pStream->pCommand, size);
            nn::gx::detail::s_pCommandBufferCurrent =
                reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + (size & ~3));
        }
    }
}

// 0x0049605C | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::SetCameraMatrix(Camera* pCamera, bool isForced)
{
    if (m_pCamera == pCamera && !isForced) {
        return;
    }
    m_pCamera = pCamera;
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
    pCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_VIEW_MATRIX;
    internal::NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pCommand + 2), reinterpret_cast<const f32*>(&pCamera->m_ViewMatrix),
                                    Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11));
    pCommand = nn::gx::detail::s_pCommandBufferCurrent += 16;
    pCommand[1] = Header(REG_VSH_FLOATUNIFORM_INDEX);
    pCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_PROJECTION_MATRIX;
    internal::NWCopyMtx44WithHeader(reinterpret_cast<f32*>(pCommand + 2), reinterpret_cast<const f32*>(&pCamera->m_ProjectionMatrix),
                                    Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 15));
    pCommand = nn::gx::detail::s_pCommandBufferCurrent += 20;

    // the same for the geometry shader
    pCommand[1] = Header(REG_GSH_FLOATUNIFORM_INDEX);
    pCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_VIEW_MATRIX;
    internal::NWCopyMtx34WithHeader(reinterpret_cast<f32*>(pCommand + 2), reinterpret_cast<const f32*>(&pCamera->m_ViewMatrix),
                                    Header(REG_GSH_FLOATUNIFORM_DATA, 0xF, 11));
    pCommand = nn::gx::detail::s_pCommandBufferCurrent += 16;
    pCommand[1] = Header(REG_GSH_FLOATUNIFORM_INDEX);
    pCommand[0] = FLOAT_UNIFORM_F32 | UNIFORM_PROJECTION_MATRIX;
    internal::NWCopyMtx44WithHeader(reinterpret_cast<f32*>(pCommand + 2), reinterpret_cast<const f32*>(&pCamera->m_ProjectionMatrix),
                                    Header(REG_GSH_FLOATUNIFORM_DATA, 0xF, 15));
    nn::gx::detail::s_pCommandBufferCurrent += 20;
}

// 0x00496140 | nintendogs:callgraph [tier A]
void nw::gfx::RenderContext::ActivateFragmentLight(int lightIdx, const FragmentLight* pLight)
{
    const res::ResFragmentLightData* pResource = static_cast<const res::ResFragmentLightData*>(pLight->GetResSceneObject().ptr());
    const u32 bit = 1 << lightIdx;
    const nn::math::MTX34* pViewMatrix = &m_SceneEnvironment.m_pCamera->m_ViewMatrix;
    const nn::math::MTX34& world = pLight->m_WorldMatrix;

    switch (pResource->lightKind) {
    case res::ResFragmentLightData::LIGHT_KIND_DIRECTIONAL: {
        // the position of a directional light is the direction towards it
        nn::math::VEC4 direction(pLight->m_Direction.x, pLight->m_Direction.y, pLight->m_Direction.z, 0.0f);
        nn::math::VEC4 viewDirection;
        TransformToViewCoordinate(&viewDirection, pViewMatrix, &direction);
        nn::math::VEC4 position(-viewDirection.x, -viewDirection.y, -viewDirection.z, -viewDirection.w);
        GraphicsDevice::ActivateFragmentLightPosition(lightIdx, position);
        SetDirectionalLightBit(bit, viewDirection.w);
        internal::g_SpotDisableMask |= bit;
        internal::g_DistanceAttenuationDisableMask |= bit;
        return;
    }
    case res::ResFragmentLightData::LIGHT_KIND_POINT:
        internal::g_SpotDisableMask |= bit;
        break;
    case res::ResFragmentLightData::LIGHT_KIND_SPOT: {
        const res::ResLightingLookupTableData* pSpot = pResource->angleSampler.To<res::ResLightingLookupTableData>();
        if (pSpot != nullptr) {
            internal::g_SpotDisableMask &= ~bit;
            GraphicsDevice::ActivateLookupTable(res::ResImageLookupTable(res::ResLookupTable(pSpot->sampler.ToPtr()).Dereference()),
                                                static_cast<GraphicsDevice::LutTarget>(static_cast<u8>(LUT_TARGET_SPOT0 + lightIdx)));
            if (res::ResLookupTable(pSpot->sampler.ToPtr()).Dereference()->isAbsolute) {
                internal::g_LightingLutInputAbs &= ~LUT_INPUT_ABS_SPOT_DISABLE;
            } else {
                internal::g_LightingLutInputAbs |= LUT_INPUT_ABS_SPOT_DISABLE;
            }
            internal::g_LightingLutInputSelect =
                (internal::g_LightingLutInputSelect & ~LUT_INPUT_SPOT_MASK) | (static_cast<u8>(pSpot->input) << LUT_INPUT_SPOT_SHIFT);
            internal::g_LightingLutInputScale =
                (internal::g_LightingLutInputScale & ~LUT_INPUT_SPOT_MASK) | (static_cast<u8>(pSpot->scale) << LUT_INPUT_SPOT_SHIFT);
        }
        break;
    }
    default:
        return;
    }

    // the distance attenuation of a point or spot light
    if ((pResource->flags & res::ResFragmentLightData::FLAG_DISTANCE_ATTENUATION) && pResource->distanceSampler.ToPtr() != nullptr) {
        internal::g_DistanceAttenuationDisableMask &= ~bit;
        GraphicsDevice::ActivateLookupTable(
            res::ResImageLookupTable(res::ResLookupTable(pResource->distanceSampler.ToPtr()).Dereference()),
            static_cast<GraphicsDevice::LutTarget>(static_cast<u8>(LUT_TARGET_DISTANCE_ATTENUATION0 + lightIdx)));
        u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
        pCommand[1] = Header(REG_LIGHT0_ATTENUATION_BIAS + REG_LIGHT_STRIDE * lightIdx, 0x7, 1, true);
        pCommand[0] = pResource->distanceAttenuationBias;
        pCommand[2] = pResource->distanceAttenuationScale;
        pCommand[3] = 0;
        nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;
    } else {
        internal::g_DistanceAttenuationDisableMask |= bit;
    }

    nn::math::VEC4 position(world.m[0][3], world.m[1][3], world.m[2][3], 1.0f);
    if (pResource->lightKind == res::ResFragmentLightData::LIGHT_KIND_POINT) {
        nn::math::VEC4 viewPosition;
        TransformToViewCoordinate(&viewPosition, pViewMatrix, &position);
        GraphicsDevice::ActivateFragmentLightPosition(lightIdx, viewPosition);
        SetDirectionalLightBit(bit, viewPosition.w);
        return;
    }

    // a spot light: its position (16-bit floats) and direction (fixed-point) in view space
    nn::math::VEC4 direction(pLight->m_Direction.x, pLight->m_Direction.y, pLight->m_Direction.z, 0.0f);
    nn::math::VEC4 viewDirection;
    TransformToViewCoordinate(&viewDirection, pViewMatrix, &direction);
    nn::math::VEC4 viewPosition;
    TransformToViewCoordinate(&viewPosition, pViewMatrix, &position);
    u32 command[ShaderProgram::BOOL_UNIFORM_COMMAND_WORDS] = {0, 0, 0, 0, 0, 0};
    command[0] = ToFloat16(viewPosition.x) | (ToFloat16(viewPosition.y) << 16);
    command[1] = Header(REG_LIGHT0_VECTOR_LOW + REG_LIGHT_STRIDE * lightIdx, 0xF, 3, true);
    command[2] = ToFloat16(viewPosition.z);
    command[3] = ToFixed13(-viewDirection.x) | (ToFixed13(-viewDirection.y) << 16);
    command[4] = ToFixed13(-viewDirection.z);
    nn::gx::detail::s_pCommandBufferCurrent = internal::FastWordCopyAsm_6(nn::gx::detail::s_pCommandBufferCurrent, command);
    SetDirectionalLightBit(bit, viewPosition.w);
}

// 0x00496858 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::RenderContext::SetModelMatrixForModel(Model* pModel)
{
    PutVertexMatrix34(UNIFORM_MODEL_MATRIX, nn::math::MTX34::Identity());
    PutVertexMatrix34(UNIFORM_NORMAL_MATRIX, pModel->m_NormalMatrix);
    m_pModel = pModel;
}

// 0x00496944 | nintendogs:bytes [tier B]
void nw::gfx::RenderContext::ActivateHemiSphereLight()
{
    HemiSphereLight* pLight = m_SceneEnvironment.m_pHemiSphereLight;
    if (pLight == nullptr) {
        return;
    }
    if (!(m_ShaderProgram->m_ActiveDescription.ptr()->typeInfo & res::ResShaderProgramDescriptionData::TYPE_FLAG_HEMISPHERE_LIGHTING)) {
        return;
    }
    const res::ResHemiSphereLightData* pResource = static_cast<const res::ResHemiSphereLightData*>(pLight->GetResSceneObject().ptr());
    PutVertexUniform(UNIFORM_HEMISPHERE_GROUND, &pResource->groundColor.r);
    PutVertexUniform(UNIFORM_HEMISPHERE_SKY, &pResource->skyColor.r);

    // the direction in view space (in the space of the light if it turns with it)
    f32 x = pResource->direction.x;
    f32 y = pResource->direction.y;
    f32 z = pResource->direction.z;
    if ((pResource->flags & res::ResHemiSphereLightData::FLAG_DIRECTION_IN_WORLD) && pLight->m_pParent != nullptr) {
        const nn::math::MTX34& world = pLight->m_WorldMatrix;
        x = world.m[0][0] * pResource->direction.x + world.m[0][1] * pResource->direction.y + world.m[0][2] * pResource->direction.z;
        y = world.m[1][0] * pResource->direction.x + world.m[1][1] * pResource->direction.y + world.m[1][2] * pResource->direction.z;
        z = world.m[2][0] * pResource->direction.x + world.m[2][1] * pResource->direction.y + world.m[2][2] * pResource->direction.z;
    }
    const nn::math::MTX34& view = m_SceneEnvironment.m_pCamera->m_ViewMatrix;
    f32 direction[4];
    direction[0] = view.m[0][0] * x + view.m[0][1] * y + view.m[0][2] * z;
    direction[1] = view.m[1][0] * x + view.m[1][1] * y + view.m[1][2] * z;
    direction[2] = view.m[2][0] * x + view.m[2][1] * y + view.m[2][2] * z;
    direction[3] = pResource->lerpFactor;
    PutVertexUniform(UNIFORM_HEMISPHERE_DIRECTION, direction);
}

// 0x00496AE4 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateVertexAttribute(res::ResMesh mesh)
{
    if (m_IsVertexAttributeDirty) {
        internal::ClearVertexAttribute();
        m_IsVertexAttributeDirty = false;
    }
    const u32 size = mesh.ptr()->commandSize;
    nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, mesh.ptr()->pCommand, size);
    nn::gx::detail::s_pCommandBufferCurrent =
        reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + (size & ~3));
    nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, mesh.ptr()->scaleCommand, sizeof(mesh.ptr()->scaleCommand));
    nn::gx::detail::s_pCommandBufferCurrent += res::ResMeshData::SCALE_COMMAND_WORDS;

    // the bool uniforms of the four-component attributes
    const u32 flags = mesh.ptr()->flags;
    const bool isUsage8Vec4 = (flags & res::ResMeshData::FLAG_USAGE8_HAS_4_COMPONENTS) != 0;
    if (flags & res::ResMeshData::FLAG_USAGE3_HAS_4_COMPONENTS) {
        m_ShaderProgram->m_VertexBoolUniformCommand[0] |= ShaderProgram::BOOL_USAGE3_HAS_4_COMPONENTS;
    } else {
        m_ShaderProgram->m_VertexBoolUniformCommand[0] &= ~ShaderProgram::BOOL_USAGE3_HAS_4_COMPONENTS;
    }
    if (isUsage8Vec4) {
        m_ShaderProgram->m_VertexBoolUniformCommand[0] |= ShaderProgram::BOOL_USAGE8_HAS_4_COMPONENTS;
    } else {
        m_ShaderProgram->m_VertexBoolUniformCommand[0] &= ~ShaderProgram::BOOL_USAGE8_HAS_4_COMPONENTS;
    }
}

// 0x00496B8C | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::ActivateSceneEnvironment()
{
    if (m_RenderModeFlags & RENDER_MODE_NO_SCENE_ENVIRONMENT) {
        return;
    }
    const res::ResMaterialData* pResource = m_pMaterial->GetResource(Material::RESOURCE_SCENE_ENVIRONMENT).ptr();
    m_SceneEnvironment.SetActiveLightSet(pResource->lightSetIndex);
    if (m_SceneEnvironment.m_IsFragmentLightDirty) {
        internal::g_DirectionalLightMask = 0;
        internal::g_ShadowDisableMask = 0xFF;
        internal::g_SpotDisableMask = 0xFF;
        internal::g_DistanceAttenuationDisableMask = 0xFF;
        const int count = m_SceneEnvironment.m_FragmentLightCount;
        for (int i = 0; i < count; ++i) {
            ActivateFragmentLight(i, static_cast<const FragmentLight*>(m_SceneEnvironment.m_FragmentLights[i]));
        }
        u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
        pCommand[1] = Header(REG_LIGHTING_CONFIG1, 0xB);
        pCommand[0] = internal::g_ShadowDisableMask | (internal::g_SpotDisableMask << 8) | (internal::g_DistanceAttenuationDisableMask << 24);
        nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
    }
    if (m_SceneEnvironment.m_IsHemiSphereLightDirty) {
        ActivateHemiSphereLight();
    }
    Fog* pFog = m_SceneEnvironment.m_Fogs[pResource->fogIndex];
    if (m_SceneEnvironment.m_pFog != pFog) {
        m_SceneEnvironment.m_pFog = pFog;
        m_SceneEnvironment.m_IsFogDirty = true;
    }
    if (m_SceneEnvironment.m_IsFogDirty) {
        ActivateFog();
    }
}

// 0x00496CB4 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::TransformToViewCoordinate(nn::math::VEC4* pOut, const nn::math::MTX34* pViewMatrix, const nn::math::VEC4* pIn)
{
    const nn::math::MTX34& m = *pViewMatrix;
    const f32 x = pIn->x;
    const f32 y = pIn->y;
    const f32 z = pIn->z;
    const f32 w = pIn->w;
    pOut->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3] * w;
    pOut->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3] * w;
    pOut->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3] * w;
    pOut->w = w;
}

// 0x00496D30 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::RenderContext::SetModelMatrixForSkeletalModel(SkeletalModel* pModel)
{
    // the bones of a skeleton in model space are moved by the model matrix in the shader
    const res::ResSkeletonData* pResSkeleton =
        reinterpret_cast<const res::ResSkeletonData*>(pModel->GetSkeleton()->GetResSceneObject().ptr());
    if (pResSkeleton->flags & res::ResSkeletonData::FLAG_IS_MODEL_COORDINATE) {
        PutVertexMatrix34(UNIFORM_MODEL_MATRIX, pModel->m_WorldMatrix);
    } else {
        PutVertexMatrix34(UNIFORM_MODEL_MATRIX, nn::math::MTX34::Identity());
    }
    PutVertexMatrix34(UNIFORM_NORMAL_MATRIX, pModel->m_NormalMatrix);
    m_pModel = pModel;
}

// 0x00497454 slot 0x00 | nintendogs:bytes
// 0x00497448 (deleting dtor)
nw::gfx::RenderContext::~RenderContext()
{
    // (only the members are destroyed)
}

} // namespace gfx
} // namespace nw
