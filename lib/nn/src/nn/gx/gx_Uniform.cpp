#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gx/detail/gx_Shader.h"

// The uniform setters of the program in use (glUniform*). The uniforms of the shaders go to the
// float, integer and bool registers of the program; the reserved uniforms (dmp_*, see
// s_ReservedUniforms) keep their values in the program and become the values of GPU registers (the
// registers after 3dbrew, numbered as in the register list of the shader tables). All names are ours.

using namespace nn::gx::detail;

namespace {
// the dirty flags of the GL state: the uniforms of the shaders / the registers of the reserved uniforms
const u32 DIRTY_SHADER_UNIFORMS = 0x10000;
const u32 DIRTY_RESERVED_REGISTERS = 0x80000;
// the bits of a uniform location and of its info (see UniformLocation)
const u32 LOCATION_RESERVED = 0x40000;
const u32 INFO_GEOMETRY = 0x400;

// the reserved uniforms (indices into s_ReservedUniforms)
const u32 UNIFORM_SHADOW_Z_BIAS = 1;
const u32 UNIFORM_SHADOW_Z_SCALE = 2;
const u32 UNIFORM_PT_TEX_BIAS = 19;
const u32 UNIFORM_PT_NOISE_U = 21;
const u32 UNIFORM_PT_NOISE_V = 22;
const u32 UNIFORM_PENUMBRA_SCALE = 31;
const u32 UNIFORM_PENUMBRA_BIAS = 32;
const u32 UNIFORM_W_SCALE = 33;
const u32 UNIFORM_CLIPPING_PLANE = 35;
const u32 UNIFORM_ALPHA_REF_VALUE = 38;
const u32 UNIFORM_GAS_LIGHT_XY = 39;
const u32 UNIFORM_GAS_LIGHT_Z = 40;
const u32 UNIFORM_GAS_DELTA_Z = 41;
const u32 UNIFORM_GAS_ACC_MAX = 42;
const u32 UNIFORM_GAS_ATTENUATION = 44;
const u32 UNIFORM_LIGHTING_AMBIENT = 51;
const u32 UNIFORM_MATERIAL_EMISSION = 52;
const u32 UNIFORM_MATERIAL_AMBIENT = 53;
const u32 UNIFORM_MATERIAL_DIFFUSE = 54;
const u32 UNIFORM_MATERIAL_SPECULAR0 = 55;
const u32 UNIFORM_MATERIAL_SPECULAR1 = 56;
const u32 UNIFORM_LIGHT_AMBIENT = 65;
const u32 UNIFORM_LIGHT_DIFFUSE = 73;
const u32 UNIFORM_LIGHT_SPECULAR0 = 81;
const u32 UNIFORM_LIGHT_SPECULAR1 = 89;
const u32 UNIFORM_LIGHT_POSITION = 97;
const u32 UNIFORM_LIGHT_SPOT_DIRECTION = 105;
const u32 UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS = 161;
const u32 UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE = 169;
const u32 UNIFORM_LUT_SCALE = 207;
const u32 UNIFORM_TEX_ENV_SCALE_RGB = 270;
const u32 UNIFORM_TEX_ENV_SCALE_ALPHA = 276;
const u32 UNIFORM_TEX_ENV_CONST_RGBA = 282;
const u32 UNIFORM_TEX_ENV_BUFFER_COLOR = 288;
const u32 UNIFORM_FOG_COLOR = 294;
const u32 LIGHT_COUNT = 8;
const u32 LUT_SCALE_COUNT = 7;
const u32 TEX_ENV_COUNT = 6;

// the values of the reserved uniforms (offsets in the program, see InitializeReservedValues)
const u32 RESERVED_IS_REFLECTION_LUT_DISABLED = 0x9EC;
const u32 RESERVED_IS_SHADOW_PRIMARY = 0x9ED;
const u32 RESERVED_IS_SHADOW_SECONDARY = 0x9EE;
const u32 RESERVED_IS_SHADOW_ALPHA = 0x9EF;
const u32 RESERVED_IS_BUMP_RENORM = 0x9F0;
const u32 RESERVED_MATERIAL_LUTS = 0x9F4;
const u32 RESERVED_LIGHTING_CONFIG = 0xA0C;
const u32 RESERVED_LIGHTING_AMBIENT = 0xA10;
const u32 RESERVED_LIGHTS = 0xA20;
const u32 RESERVED_MATERIAL_AMBIENT = 0xDA0;
const u32 RESERVED_MATERIAL_DIFFUSE = 0xDB0;
const u32 RESERVED_MATERIAL_SPECULAR0 = 0xDC0;
const u32 RESERVED_MATERIAL_SPECULAR1 = 0xDD0;
const u32 RESERVED_MATERIAL_EMISSION = 0xDE0;
const u32 RESERVED_PT_LUTS = 0xDF0;
const u32 RESERVED_PT_SAMPLER_TYPE = 0xE0C;
const u32 RESERVED_PT_TEX_BIAS = 0xE10;
const u32 RESERVED_PT_NOISE_U = 0xE14;
const u32 RESERVED_PT_NOISE_V = 0xE20;
const u32 RESERVED_PENUMBRA_BIAS = 0xE3C;
const u32 RESERVED_PENUMBRA_SCALE = 0xE40;
const u32 RESERVED_SHADOW_Z_BIAS = 0xE44;
const u32 RESERVED_SHADOW_Z_SCALE = 0xE48;
const u32 RESERVED_W_SCALE = 0xE4C;
const u32 RESERVED_CLIPPING_PLANE = 0xE50;
const u32 RESERVED_ALPHA_REF_VALUE = 0xE60;
const u32 RESERVED_FOG_LUT = 0xE64;
const u32 RESERVED_FOG_COLOR = 0xE68;
const u32 RESERVED_GAS_LUTS = 0xE78;
const u32 RESERVED_GAS_LIGHT_XY = 0xE84;
const u32 RESERVED_GAS_LIGHT_Z = 0xE90;
const u32 RESERVED_GAS_DELTA_Z = 0xEA0;
const u32 RESERVED_GAS_ACC_MAX = 0xEA4;
const u32 RESERVED_GAS_ATTENUATION = 0xEA8;
const u32 RESERVED_TEX_ENV_CONST_RGBA = 0xEAC;
const u32 RESERVED_TEX_ENV_BUFFER_COLOR = 0xF0C;
// the values of a light (0x70 bytes each)
const u32 LIGHT_SIZE = 0x70;
const u32 LIGHT_IS_ENABLED = 0x00;
const u32 LIGHT_AMBIENT = 0x04;
const u32 LIGHT_DIFFUSE = 0x14;
const u32 LIGHT_SPECULAR0 = 0x24;
const u32 LIGHT_SPECULAR1 = 0x34;
const u32 LIGHT_POSITION = 0x44;
const u32 LIGHT_SPOT_DIRECTION = 0x54;
const u32 LIGHT_LUT_SP = 0x60;
const u32 LIGHT_DISTANCE_ATTENUATION_BIAS = 0x64;
const u32 LIGHT_DISTANCE_ATTENUATION_SCALE = 0x68;
const u32 LIGHT_LUT_DA = 0x6C;

// the registers of the reserved uniforms (the registers of a light are 11 apart, those of a texture
// combiner 5)
const u32 REGISTER_W_SCALE_DISABLED = 39;
const u32 REGISTER_DEPTH_SCALE = 23;
const u32 REGISTER_DEPTH_OFFSET = 24;
const u32 REGISTER_CLIPPING_PLANE = 34;
const u32 REGISTER_SHADOW = 42;
const u32 REGISTER_PT_TEX_BIAS_LOW = 44;
const u32 REGISTER_PT_NOISE_U = 45;
const u32 REGISTER_PT_NOISE_V = 46;
const u32 REGISTER_PT_NOISE_FREQUENCY = 47;
const u32 REGISTER_PT_TEX_BIAS_HIGH = 48;
const u32 REGISTER_TEX_ENV_CONST_RGBA = 53;
const u32 REGISTER_TEX_ENV_SCALE = 54;
const u32 REGISTER_FOG_COLOR = 81;
const u32 REGISTER_GAS_ATTENUATION = 82;
const u32 REGISTER_GAS_ACC_MAX = 83;
const u32 REGISTER_TEX_ENV_BUFFER_COLOR = 84;
const u32 REGISTER_GAS_LIGHT_XY = 85;
const u32 REGISTER_GAS_LIGHT_Z = 86;
const u32 REGISTER_GAS_LIGHT_Z_COLOR = 87;
const u32 REGISTER_GAS_DELTA_Z = 88;
const u32 REGISTER_ALPHA_TEST = 90;
const u32 REGISTER_PENUMBRA = 91;
const u32 REGISTER_LIGHT_SPECULAR0 = 92;
const u32 REGISTER_LIGHT_SPECULAR1 = 93;
const u32 REGISTER_LIGHT_DIFFUSE = 94;
const u32 REGISTER_LIGHT_AMBIENT = 95;
const u32 REGISTER_LIGHT_POSITION_XY = 96;
const u32 REGISTER_LIGHT_POSITION_Z = 97;
const u32 REGISTER_LIGHT_SPOT_DIRECTION_XY = 98;
const u32 REGISTER_LIGHT_SPOT_DIRECTION_Z = 99;
const u32 REGISTER_LIGHT_CONFIG = 100;
const u32 REGISTER_LIGHT_DISTANCE_ATTENUATION_BIAS = 101;
const u32 REGISTER_LIGHT_DISTANCE_ATTENUATION_SCALE = 102;
const u32 REGISTER_LIGHTING_AMBIENT = 180;
const u32 REGISTER_LUT_SCALE = 187;
const u32 LIGHT_REGISTER_STRIDE = 11;
const u32 TEX_ENV_REGISTER_STRIDE = 5;

inline u32 BitsOf(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    return bits.u;
}

DECOMP_ALWAYS_INLINE bool IsInfiniteOrNan(f32 value)
{
    return ((BitsOf(value) << 1) >> 24) == 0xFF;
}

// 1.5.10 float
DECOMP_ALWAYS_INLINE u32 Float32ToFloat16(f32 value)
{
    const u32 bits = BitsOf(value);
    const u32 sign = bits >> 31;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 112;
    }
    if (exponent < 0) {
        return sign << 15;
    }
    return ((bits << 9) >> 22) | (exponent << 10) | (sign << 15);
}

// 1.7.12 float
DECOMP_ALWAYS_INLINE u32 Float32ToFloat20(f32 value)
{
    const u32 bits = BitsOf(value);
    const u32 sign = bits >> 31;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
    }
    if (exponent < 0) {
        return sign << 19;
    }
    return ((bits << 9) >> 20) | (exponent << 12) | (sign << 19);
}

// unsigned fixed point of 24 bits (0 for values that are not positive or infinite)
DECOMP_ALWAYS_INLINE u32 FloatToFixed24(f32 value, f32 scale)
{
    if (value <= 0.0f || IsInfiniteOrNan(value)) {
        return 0;
    }
    const f32 scaled = value * scale;
    return scaled < 16777216.0f ? static_cast<u32>(scaled) : 0xFFFFFF;
}

// signed fixed point in two's complement: (value + range) * scale is clamped to [0, max] and has its
// sign bit (half) flipped
DECOMP_ALWAYS_INLINE u32 FloatToSignedFixed(f32 value, f32 range, f32 scale, f32 max, f32 half)
{
    if (value == 0.0f || IsInfiniteOrNan(value)) {
        return 0;
    }
    f32 scaled = (value + range) * scale;
    if (scaled < 0.0f) {
        scaled = 0.0f;
    } else if (scaled >= half * 2) {
        scaled = max;
    }
    if (scaled >= half) {
        return static_cast<u32>(scaled - half);
    }
    return static_cast<u32>(scaled + half);
}

// a color component of 8 bits
DECOMP_ALWAYS_INLINE u32 ToColorByte(f32 value)
{
    return static_cast<u32>(value * 255.0f + 0.5f);
}

DECOMP_ALWAYS_INLINE f32 ClampToOne(f32 value)
{
    return value > 1.0f ? 1.0f : value;
}

// a light color: three clamped components in fields of 10 bits, blue first
DECOMP_ALWAYS_INLINE u32 PackLightColor(f32 red, f32 green, f32 blue)
{
    return ToColorByte(ClampToOne(blue)) | (ToColorByte(ClampToOne(green)) << 10) | (ToColorByte(ClampToOne(red)) << 20);
}

// four components of 8 bits, red first
DECOMP_ALWAYS_INLINE u32 PackColor(const f32* pColor)
{
    return ToColorByte(pColor[0]) | (ToColorByte(pColor[1]) << 8) | (ToColorByte(pColor[2]) << 16) | (ToColorByte(pColor[3]) << 24);
}

DECOMP_ALWAYS_INLINE f32* ReservedFloats(Program* pProgram, u32 offset)
{
    return reinterpret_cast<f32*>(reinterpret_cast<u8*>(pProgram) + offset);
}

DECOMP_ALWAYS_INLINE bool* ReservedBool(Program* pProgram, u32 offset)
{
    return reinterpret_cast<bool*>(reinterpret_cast<u8*>(pProgram) + offset);
}

DECOMP_ALWAYS_INLINE u32* ReservedWord(Program* pProgram, u32 offset)
{
    return reinterpret_cast<u32*>(reinterpret_cast<u8*>(pProgram) + offset);
}

DECOMP_ALWAYS_INLINE f32* LightFloats(Program* pProgram, u32 light, u32 offset)
{
    return ReservedFloats(pProgram, RESERVED_LIGHTS + light * LIGHT_SIZE + offset);
}

DECOMP_ALWAYS_INLINE void CopyFloats(f32* pDestination, const f32* pSource, u32 count)
{
    for (u32 i = 0; i < count; i++) {
        pDestination[i] = pSource[i];
    }
}

// the stored color of a material, true if it changed
DECOMP_ALWAYS_INLINE bool UpdateMaterialColor(f32* pColor, const f32* pValues)
{
    bool isChanged = false;
    for (u32 i = 0; i < 4; i++) {
        f32 old = pColor[i];
        pColor[i] = pValues[i];
        if (old != pValues[i]) {
            isChanged = true;
        }
    }
    return isChanged;
}

// the bytes of a register a bit mask touches (the byte mask of its command)
DECOMP_ALWAYS_INLINE u8 ByteMaskOf(u32 bits)
{
    return ((bits & 0xFF) != 0 ? 1 : 0) | ((bits & 0xFF00) != 0 ? 2 : 0) | ((bits & 0xFF0000) != 0 ? 4 : 0) | ((bits & 0xFF000000) != 0 ? 8 : 0);
}

// bits of a register of the program: the bytes of its command and its value (when the commands are
// generated later the last value sent is invalidated, so that the validator sends it again)
DECOMP_ALWAYS_INLINE void SetRegisterBits(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 index, u32 value, u32 bits, u8 byteMask,
                                          u32 dirtyFlags = DIRTY_RESERVED_REGISTERS)
{
    pProgram->m_RegisterMasks[index] |= byteMask;
    if (pState->m_IsCommandGenerationDeferred) {
        pProgram->m_RegisterValues[index] = (pProgram->m_RegisterValues[index] & ~bits) | (value & bits);
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
        pState->m_DirtyFlags |= dirtyFlags;
        pTables->m_SentRegisterValues[index] = ~pProgram->m_RegisterValues[index];
    } else if ((pProgram->m_RegisterValues[index] & bits) != (value & bits)) {
        pProgram->m_RegisterValues[index] = (pProgram->m_RegisterValues[index] & ~bits) | (value & bits);
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
        pState->m_DirtyFlags |= dirtyFlags;
    }
}

DECOMP_ALWAYS_INLINE void SetRegister(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 index, u32 value)
{
    SetRegisterBits(pProgram, pState, pTables, index, value, 0xFFFFFFFF, 0xF);
}

// the ambient color of the lighting: emission + ambient of the lighting * ambient of the material
DECOMP_ALWAYS_INLINE void SetLightingAmbient(Program* pProgram, GlState* pState, ShaderTables* pTables, const f32* pEmission, const f32* pAmbient,
                               const f32* pMaterial)
{
    SetRegister(pProgram, pState, pTables, REGISTER_LIGHTING_AMBIENT,
                PackLightColor(pEmission[0] + pAmbient[0] * pMaterial[0], pEmission[1] + pAmbient[1] * pMaterial[1],
                               pEmission[2] + pAmbient[2] * pMaterial[2]));
}

// a color of a light times the color of the material
DECOMP_ALWAYS_INLINE void SetLightColor(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 index, const f32* pLight, const f32* pMaterial)
{
    SetRegister(pProgram, pState, pTables, index, PackLightColor(pLight[0] * pMaterial[0], pLight[1] * pMaterial[1], pLight[2] * pMaterial[2]));
}

// the specular color 1 of a light (times that of the material while the reflection lookup table is
// disabled)
DECOMP_ALWAYS_INLINE void SetLightSpecular1(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 light, const f32* pLight)
{
    u32 index = REGISTER_LIGHT_SPECULAR1 + light * LIGHT_REGISTER_STRIDE;
    if (*ReservedBool(pProgram, RESERVED_IS_REFLECTION_LUT_DISABLED)) {
        SetLightColor(pProgram, pState, pTables, index, pLight, ReservedFloats(pProgram, RESERVED_MATERIAL_SPECULAR1));
    } else {
        SetRegister(pProgram, pState, pTables, index, PackLightColor(pLight[0], pLight[1], pLight[2]));
    }
}

// the scale of a lookup table: 0.25 ... 8 as 6, 7, 0, 1, 2, 3
DECOMP_ALWAYS_INLINE void SetLutScale(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 table, f32 value)
{
    u32 scale;
    switch (static_cast<s32>(value * 4.0f)) {
    case 1:
        scale = 6;
        break;
    case 2:
        scale = 7;
        break;
    case 4:
        scale = 0;
        break;
    case 8:
        scale = 1;
        break;
    case 16:
        scale = 2;
        break;
    case 32:
        scale = 3;
        break;
    default:
        return;
    }
    u32 shift = table * 4;
    u32 bits = 0xF << shift;
    SetRegisterBits(pProgram, pState, pTables, REGISTER_LUT_SCALE, (scale & 0xF) << shift, bits, ByteMaskOf(bits));
}

// the scale of a texture combiner: 1, 2, 4 as 0, 1, 2
DECOMP_ALWAYS_INLINE bool GetTexEnvScale(f32 value, u32* pScale)
{
    switch (static_cast<s32>(value)) {
    case 1:
        *pScale = 0;
        return true;
    case 2:
        *pScale = 1;
        return true;
    case 4:
        *pScale = 2;
        return true;
    default:
        return false;
    }
}

// the depth of the w buffer: wScale 0 uses the depth range (offset by the polygon offset in steps of
// the depth buffer)
DECOMP_ALWAYS_INLINE void SetWScale(Program* pProgram, GlState* pState, ShaderTables* pTables, f32 value)
{
    SetRegister(pProgram, pState, pTables, REGISTER_W_SCALE_DISABLED, value == 0.0f ? 1 : 0);
    f32 scale;
    f32 offset;
    if (value == 0.0f) {
        offset = pState->m_DepthRangeNear;
        scale = offset - pState->m_DepthRangeFar;
    } else {
        scale = -value;
        offset = 0.0f;
    }
    if (pState->m_IsPolygonOffsetFillEnabled && pState->m_PolygonOffsetUnits != 0.0f) {
        offset += pState->m_PolygonOffsetUnits * (pState->m_DepthFormat != 0 ? 1.0f / 16777215.0f : 1.0f / 65535.0f);
    }
    u32 scaleValue = Float32ToFloat24(scale);
    u32 offsetValue = offset == 0.0f ? 0 : Float32ToFloat24(offset);
    SetRegister(pProgram, pState, pTables, REGISTER_DEPTH_SCALE, scaleValue);
    SetRegister(pProgram, pState, pTables, REGISTER_DEPTH_OFFSET, offsetValue);
    *ReservedFloats(pProgram, RESERVED_W_SCALE) = value;
}

// the penumbra of the shadows: bias + scale and -scale
DECOMP_ALWAYS_INLINE void SetPenumbra(Program* pProgram, GlState* pState, ShaderTables* pTables)
{
    f32 scale = *ReservedFloats(pProgram, RESERVED_PENUMBRA_SCALE);
    u32 negativeScale = Float32ToFloat16(-scale);
    u32 sum = Float32ToFloat16(*ReservedFloats(pProgram, RESERVED_PENUMBRA_BIAS) + scale);
    SetRegister(pProgram, pState, pTables, REGISTER_PENUMBRA, sum | (negativeScale << 16));
}

// the noise of procedural textures: phase and amplitude, frequency in the low or high half
DECOMP_ALWAYS_INLINE void SetNoise(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 index, const f32* pValues, u32 frequencyShift,
                     u8 frequencyByteMask)
{
    u32 amplitude = Float32ToFloat16(pValues[1]);
    u32 phase = FloatToSignedFixed(pValues[2], 8.0f, 4096.0f, 65535.0f, 32768.0f);
    SetRegister(pProgram, pState, pTables, index, phase | (amplitude << 16));
    SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_NOISE_FREQUENCY, Float32ToFloat16(pValues[0]) << frequencyShift,
                    0xFFFF << frequencyShift, frequencyByteMask);
}

// the reserved uniforms
DECOMP_ALWAYS_INLINE void SetReservedUniformFloat(u32 uniform, const GLfloat* pValues)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    GlState* pState = s_pGlState;
    Program* pProgram = pTables->m_pCurrentProgram;

    switch (uniform) {
    case UNIFORM_LUT_SCALE + 0:
    case UNIFORM_LUT_SCALE + 1:
    case UNIFORM_LUT_SCALE + 2:
    case UNIFORM_LUT_SCALE + 3:
    case UNIFORM_LUT_SCALE + 4:
    case UNIFORM_LUT_SCALE + 5:
    case UNIFORM_LUT_SCALE + 6:
    {
        SetLutScale(pProgram, pState, pTables, uniform - UNIFORM_LUT_SCALE, pValues[0]);
        break;
    }
    case UNIFORM_TEX_ENV_SCALE_RGB + 0:
    case UNIFORM_TEX_ENV_SCALE_RGB + 1:
    case UNIFORM_TEX_ENV_SCALE_RGB + 2:
    case UNIFORM_TEX_ENV_SCALE_RGB + 3:
    case UNIFORM_TEX_ENV_SCALE_RGB + 4:
    case UNIFORM_TEX_ENV_SCALE_RGB + 5:
    {
        u32 scale;
        if (GetTexEnvScale(pValues[0], &scale)) {
            u32 index = REGISTER_TEX_ENV_SCALE + (uniform - UNIFORM_TEX_ENV_SCALE_RGB) * TEX_ENV_REGISTER_STRIDE;
            SetRegisterBits(pProgram, pState, pTables, index, scale, 0x3, 0x1);
        }
        break;
    }
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 0:
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 1:
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 2:
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 3:
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 4:
    case UNIFORM_TEX_ENV_SCALE_ALPHA + 5:
    {
        u32 scale;
        if (GetTexEnvScale(pValues[0], &scale)) {
            u32 index = REGISTER_TEX_ENV_SCALE + (uniform - UNIFORM_TEX_ENV_SCALE_ALPHA) * TEX_ENV_REGISTER_STRIDE;
            SetRegisterBits(pProgram, pState, pTables, index, scale << 16, 0x30000, 0x4);
        }
        break;
    }
    case UNIFORM_TEX_ENV_CONST_RGBA + 0:
    case UNIFORM_TEX_ENV_CONST_RGBA + 1:
    case UNIFORM_TEX_ENV_CONST_RGBA + 2:
    case UNIFORM_TEX_ENV_CONST_RGBA + 3:
    case UNIFORM_TEX_ENV_CONST_RGBA + 4:
    case UNIFORM_TEX_ENV_CONST_RGBA + 5:
    {
        u32 combiner = uniform - UNIFORM_TEX_ENV_CONST_RGBA;
        CopyFloats(ReservedFloats(pProgram, RESERVED_TEX_ENV_CONST_RGBA + combiner * 16), pValues, 4);
        SetRegister(pProgram, pState, pTables, REGISTER_TEX_ENV_CONST_RGBA + combiner * TEX_ENV_REGISTER_STRIDE, PackColor(pValues));
        break;
    }
    case UNIFORM_LIGHT_AMBIENT + 0:
    case UNIFORM_LIGHT_AMBIENT + 1:
    case UNIFORM_LIGHT_AMBIENT + 2:
    case UNIFORM_LIGHT_AMBIENT + 3:
    case UNIFORM_LIGHT_AMBIENT + 4:
    case UNIFORM_LIGHT_AMBIENT + 5:
    case UNIFORM_LIGHT_AMBIENT + 6:
    case UNIFORM_LIGHT_AMBIENT + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_AMBIENT;
        CopyFloats(LightFloats(pProgram, light, LIGHT_AMBIENT), pValues, 4);
        SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_AMBIENT + light * LIGHT_REGISTER_STRIDE, pValues,
                      ReservedFloats(pProgram, RESERVED_MATERIAL_AMBIENT));
        break;
    }
    case UNIFORM_LIGHT_DIFFUSE + 0:
    case UNIFORM_LIGHT_DIFFUSE + 1:
    case UNIFORM_LIGHT_DIFFUSE + 2:
    case UNIFORM_LIGHT_DIFFUSE + 3:
    case UNIFORM_LIGHT_DIFFUSE + 4:
    case UNIFORM_LIGHT_DIFFUSE + 5:
    case UNIFORM_LIGHT_DIFFUSE + 6:
    case UNIFORM_LIGHT_DIFFUSE + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_DIFFUSE;
        CopyFloats(LightFloats(pProgram, light, LIGHT_DIFFUSE), pValues, 4);
        SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_DIFFUSE + light * LIGHT_REGISTER_STRIDE, pValues,
                      ReservedFloats(pProgram, RESERVED_MATERIAL_DIFFUSE));
        break;
    }
    case UNIFORM_LIGHT_SPECULAR0 + 0:
    case UNIFORM_LIGHT_SPECULAR0 + 1:
    case UNIFORM_LIGHT_SPECULAR0 + 2:
    case UNIFORM_LIGHT_SPECULAR0 + 3:
    case UNIFORM_LIGHT_SPECULAR0 + 4:
    case UNIFORM_LIGHT_SPECULAR0 + 5:
    case UNIFORM_LIGHT_SPECULAR0 + 6:
    case UNIFORM_LIGHT_SPECULAR0 + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_SPECULAR0;
        CopyFloats(LightFloats(pProgram, light, LIGHT_SPECULAR0), pValues, 4);
        SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_SPECULAR0 + light * LIGHT_REGISTER_STRIDE, pValues,
                      ReservedFloats(pProgram, RESERVED_MATERIAL_SPECULAR0));
        break;
    }
    case UNIFORM_LIGHT_SPECULAR1 + 0:
    case UNIFORM_LIGHT_SPECULAR1 + 1:
    case UNIFORM_LIGHT_SPECULAR1 + 2:
    case UNIFORM_LIGHT_SPECULAR1 + 3:
    case UNIFORM_LIGHT_SPECULAR1 + 4:
    case UNIFORM_LIGHT_SPECULAR1 + 5:
    case UNIFORM_LIGHT_SPECULAR1 + 6:
    case UNIFORM_LIGHT_SPECULAR1 + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_SPECULAR1;
        CopyFloats(LightFloats(pProgram, light, LIGHT_SPECULAR1), pValues, 4);
        SetLightSpecular1(pProgram, pState, pTables, light, pValues);
        break;
    }
    case UNIFORM_LIGHT_POSITION + 0:
    case UNIFORM_LIGHT_POSITION + 1:
    case UNIFORM_LIGHT_POSITION + 2:
    case UNIFORM_LIGHT_POSITION + 3:
    case UNIFORM_LIGHT_POSITION + 4:
    case UNIFORM_LIGHT_POSITION + 5:
    case UNIFORM_LIGHT_POSITION + 6:
    case UNIFORM_LIGHT_POSITION + 7:
    {
        // x and y, z and whether the light is directional (w 0)
        u32 light = uniform - UNIFORM_LIGHT_POSITION;
        u32 base = light * LIGHT_REGISTER_STRIDE;
        CopyFloats(LightFloats(pProgram, light, LIGHT_POSITION), pValues, 4);
        u32 x = Float32ToFloat16(pValues[0]);
        u32 y = Float32ToFloat16(pValues[1]);
        SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_POSITION_XY + base, x | (y << 16));
        SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_POSITION_Z + base, Float32ToFloat16(pValues[2]));
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHT_CONFIG + base, pValues[3] == 0.0f ? 1 : 0, 0x1, 0x1);
        break;
    }
    case UNIFORM_LIGHT_SPOT_DIRECTION + 0:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 1:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 2:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 3:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 4:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 5:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 6:
    case UNIFORM_LIGHT_SPOT_DIRECTION + 7:
    {
        // the negated direction as fixed point of 13 bits
        u32 light = uniform - UNIFORM_LIGHT_SPOT_DIRECTION;
        u32 base = light * LIGHT_REGISTER_STRIDE;
        CopyFloats(LightFloats(pProgram, light, LIGHT_SPOT_DIRECTION), pValues, 3);
        u32 x = FloatToSignedFixed(-pValues[0], 2.0f, 2048.0f, 8191.0f, 4096.0f);
        u32 y = FloatToSignedFixed(-pValues[1], 2.0f, 2048.0f, 8191.0f, 4096.0f);
        SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_SPOT_DIRECTION_XY + base, x | (y << 16));
        SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_SPOT_DIRECTION_Z + base,
                    FloatToSignedFixed(-pValues[2], 2.0f, 2048.0f, 8191.0f, 4096.0f));
        break;
    }
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 0:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 1:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 2:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 3:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 4:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 5:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 6:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_DISTANCE_ATTENUATION_BIAS;
        *LightFloats(pProgram, light, LIGHT_DISTANCE_ATTENUATION_BIAS) = pValues[0];
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHT_DISTANCE_ATTENUATION_BIAS + light * LIGHT_REGISTER_STRIDE,
                        Float32ToFloat20(pValues[0]), 0xFFFFF, 0x7);
        break;
    }
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 0:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 1:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 2:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 3:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 4:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 5:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 6:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE + 7:
    {
        u32 light = uniform - UNIFORM_LIGHT_DISTANCE_ATTENUATION_SCALE;
        *LightFloats(pProgram, light, LIGHT_DISTANCE_ATTENUATION_SCALE) = pValues[0];
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHT_DISTANCE_ATTENUATION_SCALE + light * LIGHT_REGISTER_STRIDE,
                        Float32ToFloat20(pValues[0]), 0xFFFFF, 0x7);
        break;
    }
    case UNIFORM_SHADOW_Z_BIAS:
        *ReservedFloats(pProgram, RESERVED_SHADOW_Z_BIAS) = pValues[0];
        SetRegisterBits(pProgram, pState, pTables, REGISTER_SHADOW, FloatToFixed24(pValues[0], 16777216.0f), 0x00FFFFFE, 0x7);
        break;
    case UNIFORM_SHADOW_Z_SCALE:
        *ReservedFloats(pProgram, RESERVED_SHADOW_Z_SCALE) = pValues[0];
        break;
    case UNIFORM_PT_TEX_BIAS: {
        *ReservedFloats(pProgram, RESERVED_PT_TEX_BIAS) = pValues[0];
        u32 bias = Float32ToFloat16(pValues[0]);
        SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_TEX_BIAS_LOW, bias << 20, 0x0FF00000, 0xC);
        SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_TEX_BIAS_HIGH, (bias >> 8) << 19, 0x07F80000, 0xC);
        break;
    }
    case UNIFORM_PT_NOISE_U:
        SetNoise(pProgram, pState, pTables, REGISTER_PT_NOISE_U, pValues, 0, 0x3);
        CopyFloats(ReservedFloats(pProgram, RESERVED_PT_NOISE_U), pValues, 3);
        break;
    case UNIFORM_PT_NOISE_V:
        SetNoise(pProgram, pState, pTables, REGISTER_PT_NOISE_V, pValues, 16, 0xC);
        CopyFloats(ReservedFloats(pProgram, RESERVED_PT_NOISE_V), pValues, 3);
        break;
    case UNIFORM_PENUMBRA_SCALE:
        *ReservedFloats(pProgram, RESERVED_PENUMBRA_SCALE) = pValues[0];
        SetPenumbra(pProgram, pState, pTables);
        break;
    case UNIFORM_PENUMBRA_BIAS:
        *ReservedFloats(pProgram, RESERVED_PENUMBRA_BIAS) = pValues[0];
        SetPenumbra(pProgram, pState, pTables);
        break;
    case UNIFORM_W_SCALE:
        SetWScale(pProgram, pState, pTables, pValues[0]);
        break;
    case UNIFORM_CLIPPING_PLANE:
        for (u32 i = 0; i < 4; i++) {
            SetRegister(pProgram, pState, pTables, REGISTER_CLIPPING_PLANE + i, Float32ToFloat24(pValues[i]));
        }
        CopyFloats(ReservedFloats(pProgram, RESERVED_CLIPPING_PLANE), pValues, 4);
        break;
    case UNIFORM_ALPHA_REF_VALUE:
        *ReservedFloats(pProgram, RESERVED_ALPHA_REF_VALUE) = pValues[0];
        SetRegisterBits(pProgram, pState, pTables, REGISTER_ALPHA_TEST, ToColorByte(pValues[0]) << 8, 0xFF00, 0x2);
        break;
    case UNIFORM_GAS_LIGHT_XY:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_LIGHT_XY,
                        ToColorByte(pValues[0]) | (ToColorByte(pValues[1]) << 8) | (ToColorByte(pValues[2]) << 16), 0xFFFFFF, 0x7);
        CopyFloats(ReservedFloats(pProgram, RESERVED_GAS_LIGHT_XY), pValues, 3);
        break;
    case UNIFORM_GAS_LIGHT_Z:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_LIGHT_Z,
                        ToColorByte(pValues[0]) | (ToColorByte(pValues[1]) << 8) | (ToColorByte(pValues[2]) << 16), 0xFFFFFF, 0x7);
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_LIGHT_Z_COLOR, ToColorByte(ClampToOne(pValues[3])), 0xFF, 0x1);
        CopyFloats(ReservedFloats(pProgram, RESERVED_GAS_LIGHT_Z), pValues, 4);
        break;
    case UNIFORM_GAS_DELTA_Z:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_DELTA_Z, FloatToFixed24(pValues[0], 256.0f), 0xFFFFFF, 0x7);
        *ReservedFloats(pProgram, RESERVED_GAS_DELTA_Z) = pValues[0];
        break;
    case UNIFORM_GAS_ACC_MAX:
        // (-0 as 0)
        SetRegister(pProgram, pState, pTables, REGISTER_GAS_ACC_MAX, Float32ToFloat16(pValues[0] != 0.0f ? pValues[0] : 0.0f));
        *ReservedFloats(pProgram, RESERVED_GAS_ACC_MAX) = pValues[0];
        break;
    case UNIFORM_GAS_ATTENUATION:
        SetRegister(pProgram, pState, pTables, REGISTER_GAS_ATTENUATION, Float32ToFloat16(pValues[0]));
        *ReservedFloats(pProgram, RESERVED_GAS_ATTENUATION) = pValues[0];
        break;
    case UNIFORM_LIGHTING_AMBIENT:
        CopyFloats(ReservedFloats(pProgram, RESERVED_LIGHTING_AMBIENT), pValues, 4);
        SetLightingAmbient(pProgram, pState, pTables, ReservedFloats(pProgram, RESERVED_MATERIAL_EMISSION), pValues,
                           ReservedFloats(pProgram, RESERVED_MATERIAL_AMBIENT));
        break;
    case UNIFORM_MATERIAL_EMISSION:
        CopyFloats(ReservedFloats(pProgram, RESERVED_MATERIAL_EMISSION), pValues, 4);
        SetLightingAmbient(pProgram, pState, pTables, pValues, ReservedFloats(pProgram, RESERVED_LIGHTING_AMBIENT),
                           ReservedFloats(pProgram, RESERVED_MATERIAL_AMBIENT));
        break;
    case UNIFORM_MATERIAL_AMBIENT: {
        f32* pMaterial = ReservedFloats(pProgram, RESERVED_MATERIAL_AMBIENT);
        if (!UpdateMaterialColor(pMaterial, pValues)) {
            break;
        }
        for (u32 light = 0; light < LIGHT_COUNT; light++) {
            SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_AMBIENT + light * LIGHT_REGISTER_STRIDE, LightFloats(pProgram, light, LIGHT_AMBIENT),
                          pMaterial);
        }
        SetLightingAmbient(pProgram, pState, pTables, ReservedFloats(pProgram, RESERVED_MATERIAL_EMISSION),
                           ReservedFloats(pProgram, RESERVED_LIGHTING_AMBIENT), pValues);
        break;
    }
    case UNIFORM_MATERIAL_DIFFUSE: {
        f32* pMaterial = ReservedFloats(pProgram, RESERVED_MATERIAL_DIFFUSE);
        if (!UpdateMaterialColor(pMaterial, pValues)) {
            break;
        }
        for (u32 light = 0; light < LIGHT_COUNT; light++) {
            SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_DIFFUSE + light * LIGHT_REGISTER_STRIDE, LightFloats(pProgram, light, LIGHT_DIFFUSE),
                          pMaterial);
        }
        break;
    }
    case UNIFORM_MATERIAL_SPECULAR0: {
        f32* pMaterial = ReservedFloats(pProgram, RESERVED_MATERIAL_SPECULAR0);
        if (!UpdateMaterialColor(pMaterial, pValues)) {
            break;
        }
        for (u32 light = 0; light < LIGHT_COUNT; light++) {
            SetLightColor(pProgram, pState, pTables, REGISTER_LIGHT_SPECULAR0 + light * LIGHT_REGISTER_STRIDE,
                          LightFloats(pProgram, light, LIGHT_SPECULAR0), pMaterial);
        }
        break;
    }
    case UNIFORM_MATERIAL_SPECULAR1:
        if (!UpdateMaterialColor(ReservedFloats(pProgram, RESERVED_MATERIAL_SPECULAR1), pValues)) {
            break;
        }
        for (u32 light = 0; light < LIGHT_COUNT; light++) {
            SetLightSpecular1(pProgram, pState, pTables, light, LightFloats(pProgram, light, LIGHT_SPECULAR1));
        }
        break;
    case UNIFORM_TEX_ENV_BUFFER_COLOR:
        CopyFloats(ReservedFloats(pProgram, RESERVED_TEX_ENV_BUFFER_COLOR), pValues, 4);
        SetRegister(pProgram, pState, pTables, REGISTER_TEX_ENV_BUFFER_COLOR, PackColor(pValues));
        break;
    case UNIFORM_FOG_COLOR:
        CopyFloats(ReservedFloats(pProgram, RESERVED_FOG_COLOR), pValues, 3);
        SetRegister(pProgram, pState, pTables, REGISTER_FOG_COLOR,
                    ToColorByte(pValues[0]) | (ToColorByte(pValues[1]) << 8) | (ToColorByte(pValues[2]) << 16));
        break;
    default:
        break;
    }
}

// the integer uniforms (indices into s_ReservedUniforms)
const u32 UNIFORM_PERSPECTIVE_SHADOW = 0;
const u32 UNIFORM_SAMPLER_TYPE = 3;
const u32 UNIFORM_TEXCOORD2 = 7;
const u32 UNIFORM_TEXCOORD3 = 8;
const u32 UNIFORM_PT_RGB_MAP = 9;
const u32 UNIFORM_PT_ALPHA_MAP = 10;
const u32 UNIFORM_PT_ALPHA_SEPARATE = 11;
const u32 UNIFORM_PT_CLAMP_U = 12;
const u32 UNIFORM_PT_CLAMP_V = 13;
const u32 UNIFORM_PT_SHIFT_U = 14;
const u32 UNIFORM_PT_SHIFT_V = 15;
const u32 UNIFORM_PT_MIN_FILTER = 16;
const u32 UNIFORM_PT_TEX_WIDTH = 17;
const u32 UNIFORM_PT_TEX_OFFSET = 18;
const u32 UNIFORM_PT_NOISE_ENABLE = 20;
const u32 UNIFORM_PT_LUTS = 23;
const u32 UNIFORM_FRAGMENT_MODE = 30;
const u32 UNIFORM_ENABLE_CLIPPING_PLANE = 34;
const u32 UNIFORM_ENABLE_ALPHA_TEST = 36;
const u32 UNIFORM_ALPHA_TEST_FUNC = 37;
const u32 UNIFORM_GAS_AUTO_ACC = 43;
const u32 UNIFORM_GAS_COLOR_LUT_INPUT = 45;
const u32 UNIFORM_GAS_SHADING_DENSITY_SOURCE = 46;
const u32 UNIFORM_GAS_LUTS = 47;
const u32 UNIFORM_LIGHTING_ENABLED = 50;
const u32 UNIFORM_LIGHT_ENABLED = 57;
const u32 UNIFORM_LIGHT_SHADOWED = 113;
const u32 UNIFORM_LIGHT_GEOM_FACTOR0 = 121;
const u32 UNIFORM_LIGHT_GEOM_FACTOR1 = 129;
const u32 UNIFORM_LIGHT_TWO_SIDE_DIFFUSE = 137;
const u32 UNIFORM_LIGHT_LUT_SP = 145;
const u32 UNIFORM_LIGHT_SPOT_ENABLED = 153;
const u32 UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED = 177;
const u32 UNIFORM_LIGHT_LUT_DA = 185;
const u32 UNIFORM_ABS_LUT_INPUT = 193;
const u32 UNIFORM_LUT_INPUT = 200;
const u32 UNIFORM_MATERIAL_LUTS = 214;
const u32 UNIFORM_SHADOW_SELECTOR = 220;
const u32 UNIFORM_BUMP_SELECTOR = 221;
const u32 UNIFORM_BUMP_MODE = 222;
const u32 UNIFORM_BUMP_RENORM = 223;
const u32 UNIFORM_LIGHTING_CONFIG = 224;
const u32 UNIFORM_INVERT_SHADOW = 225;
const u32 UNIFORM_SHADOW_PRIMARY = 226;
const u32 UNIFORM_SHADOW_SECONDARY = 227;
const u32 UNIFORM_SHADOW_ALPHA = 228;
const u32 UNIFORM_FRESNEL_SELECTOR = 229;
const u32 UNIFORM_CLAMP_HIGHLIGHTS = 230;
const u32 UNIFORM_LUT_ENABLED_D0 = 231;
const u32 UNIFORM_LUT_ENABLED_D1 = 232;
const u32 UNIFORM_LUT_ENABLED_REFL = 233;
const u32 UNIFORM_TEX_ENV_COMBINE_RGB = 234;
const u32 UNIFORM_TEX_ENV_COMBINE_ALPHA = 240;
const u32 UNIFORM_TEX_ENV_SOURCE_RGB = 246;
const u32 UNIFORM_TEX_ENV_SOURCE_ALPHA = 252;
const u32 UNIFORM_TEX_ENV_OPERAND_RGB = 258;
const u32 UNIFORM_TEX_ENV_OPERAND_ALPHA = 264;
const u32 UNIFORM_TEX_ENV_BUFFER_INPUT = 289;
const u32 UNIFORM_FOG_MODE = 293;
const u32 UNIFORM_FOG_Z_FLIP = 295;
const u32 UNIFORM_FOG_LUT = 296;
const u32 PT_LUT_COUNT = 7;
const u32 GAS_LUT_COUNT = 3;
const u32 MATERIAL_LUT_COUNT = 6;
const u32 LUT_INPUT_COUNT = 7;
const u32 BUFFER_INPUT_COUNT = 4;

// the slots of the lookup tables in the GL state (m_UsedLutNames)
const u32 LUT_SLOT_MATERIAL = 0;
const u32 LUT_SLOT_SP = 6;
const u32 LUT_SLOT_DA = 14;
const u32 LUT_SLOT_PT = 22;
const u32 LUT_SLOT_FOG = 29;
const u32 LUT_SLOT_GAS = 30;

// more dirty flags of the GL state
const u32 DIRTY_LIGHTING_LUTS = 0x8;
const u32 DIRTY_FOG = 0x10;
const u32 DIRTY_PROCEDURAL_TEXTURE = 0x20;
const u32 DIRTY_FRAGMENT_MODE = 0x100;
const u32 DIRTY_TEXTURE_UNIT_0 = 0x400;
const u32 DIRTY_GAS = 0x2000000;

// more registers
const u32 REGISTER_CLIPPING_ENABLE = 33;
const u32 REGISTER_TEXTURE_CONFIG = 41;
const u32 REGISTER_LIGHTING_ENABLE = 43;
const u32 REGISTER_PT_CONTROL = 44;
const u32 REGISTER_PT_LUT = 48;
const u32 REGISTER_PT_LUT_OFFSET = 49;
const u32 REGISTER_TEX_ENV_SOURCE = 50;
const u32 REGISTER_TEX_ENV_OPERAND = 51;
const u32 REGISTER_TEX_ENV_COMBINER = 52;
const u32 REGISTER_GAS_FOG_MODE = 80;
const u32 REGISTER_GAS_CONTROL = 87;
const u32 REGISTER_FRAGMENT_MODE = 89;
const u32 REGISTER_LIGHT_COUNT = 181;
const u32 REGISTER_LIGHTING_CONFIG0 = 182;
const u32 REGISTER_LIGHTING_CONFIG1 = 183;
const u32 REGISTER_LIGHTING_DISABLED = 184;
const u32 REGISTER_LUT_INPUT_ABS = 185;
const u32 REGISTER_LUT_INPUT = 186;
const u32 REGISTER_LIGHT_PERMUTATION = 188;

// the OpenGL ES values (the others are extensions, names ours)
const GLenum GL_ADD_VALUE = 0x0104;
const GLenum GL_NEVER_VALUE = 0x0200;
const GLenum GL_SRC_COLOR_VALUE = 0x0300;
const GLenum GL_ONE_MINUS_SRC_COLOR_VALUE = 0x0301;
const GLenum GL_SRC_ALPHA_VALUE = 0x0302;
const GLenum GL_ONE_MINUS_SRC_ALPHA_VALUE = 0x0303;
const GLenum GL_FOG_VALUE = 0x0B60;
const GLenum GL_TEXTURE_2D_VALUE = 0x0DE1;
const GLenum GL_REPLACE_VALUE = 0x1E01;
const GLenum GL_MODULATE_VALUE = 0x2100;
const GLenum GL_NEAREST_VALUE = 0x2600;
const GLenum GL_LINEAR_VALUE = 0x2601;
const GLenum GL_NEAREST_MIPMAP_NEAREST_VALUE = 0x2700;
const GLenum GL_LINEAR_MIPMAP_NEAREST_VALUE = 0x2701;
const GLenum GL_NEAREST_MIPMAP_LINEAR_VALUE = 0x2702;
const GLenum GL_LINEAR_MIPMAP_LINEAR_VALUE = 0x2703;
const GLenum GL_CLAMP_TO_EDGE_VALUE = 0x812F;
const GLenum GL_MIRRORED_REPEAT_VALUE = 0x8370;
const GLenum GL_TEXTURE0_VALUE = 0x84C0;
const GLenum GL_TEXTURE1_VALUE = 0x84C1;
const GLenum GL_TEXTURE2_VALUE = 0x84C2;
const GLenum GL_TEXTURE3_VALUE = 0x84C3;
const GLenum GL_SUBTRACT_VALUE = 0x84E7;
const GLenum GL_TEXTURE_CUBE_MAP_VALUE = 0x8513;
const GLenum GL_ADD_SIGNED_VALUE = 0x8574;
const GLenum GL_INTERPOLATE_VALUE = 0x8575;
const GLenum GL_CONSTANT_VALUE = 0x8576;
const GLenum GL_PRIMARY_COLOR_VALUE = 0x8577;
const GLenum GL_PREVIOUS_VALUE = 0x8578;
const GLenum GL_DOT3_RGB_VALUE = 0x86AE;
const GLenum GL_DOT3_RGBA_VALUE = 0x86AF;
const GLenum FRAGMENT_PRIMARY_COLOR = 0x6210;
const GLenum FRAGMENT_SECONDARY_COLOR = 0x6211;
const GLenum PREVIOUS_BUFFER = 0x8579;
const GLenum MULT_ADD = 0x6401;
const GLenum ADD_MULT = 0x6402;
const GLenum OPERAND_SRC_R = 0x8580;
const GLenum OPERAND_ONE_MINUS_SRC_R = 0x8581;
const GLenum OPERAND_SRC_G = 0x8582;
const GLenum OPERAND_ONE_MINUS_SRC_G = 0x8583;
const GLenum OPERAND_SRC_B = 0x8584;
const GLenum OPERAND_ONE_MINUS_SRC_B = 0x8585;
const GLenum SAMPLER_SHADOW_2D = 0x6E00;
const GLenum SAMPLER_SHADOW_2D_1 = 0x6E01;
const GLenum SAMPLER_SHADOW_CUBE = 0x6E02;
const GLenum SAMPLER_PROCEDURAL = 0x6E03;
const GLenum PT_MAP_FIRST = 0x609A;
const GLenum PT_CLAMP_0 = 0x60C0;
const GLenum PT_CLAMP_2 = 0x60C1;
const GLenum PT_CLAMP_4 = 0x60C2;
const GLenum PT_SHIFT_FIRST = 0x60D0;
const GLenum FRAGMENT_MODE_DEFAULT = 0x6030;
const GLenum FRAGMENT_MODE_SHADOW = 0x6048;
const GLenum FRAGMENT_MODE_GAS = 0x6051;
const GLenum GAS_FOG_MODE = 0x6050;
const GLenum GAS_SHADING_DENSITY_0 = 0x605E;
const GLenum GAS_SHADING_DENSITY_1 = 0x605F;
const GLenum GAS_COLOR_LUT_INPUT_0 = 0x6060;
const GLenum GAS_COLOR_LUT_INPUT_1 = 0x6061;
const GLenum LIGHTING_CONFIG_FIRST = 0x62B0;
const GLenum FRESNEL_NONE = 0x62C0;
const GLenum BUMP_MODE_NONE = 0x62C8;

// the integer registers of a vertex / geometry shader and its bool register
const u32 REGISTER_VERTEX_INTEGER = 10;
const u32 REGISTER_GEOMETRY_INTEGER = 2;
const u32 REGISTER_VERTEX_BOOL = 9;
const u32 REGISTER_GEOMETRY_BOOL = 1;
const u32 INFO_COMPONENTS_SHIFT = 13;
const u32 INFO_NOT_BOOL = 0x6000;

// the lookup tables each lighting configuration uses (dmp_LightEnv.config; its register value is the
// index, 7 as 8)
// 0x008B3230 (name is ours)
const u32 s_LightingConfigs[8] = {0x61, 0x64, 0x23, 0x07, 0x7B, 0x7D, 0x67, 0x7F};

// the location of the last integer uniform (and its entry)
// 0x00AE8308 (name is ours)
UniformLocation s_LastIntLocation;

DECOMP_ALWAYS_INLINE u32 GetLightingConfigValue(u32 index)
{
    return index == 7 ? 8 : index;
}

// a lookup table of the program: the name and, with deferred commands, the name the GL state last used
DECOMP_ALWAYS_INLINE void SetLookupTable(Program* pProgram, GlState* pState, u32 offset, u32 slot, GLint name, u32 dirtyFlags)
{
    if (pState->m_IsCommandGenerationDeferred) {
        *ReservedWord(pProgram, offset) = name;
        pState->m_UsedLutNames[slot] = 0;
        pState->m_DirtyFlags |= dirtyFlags;
    } else if (*ReservedWord(pProgram, offset) != static_cast<u32>(name)) {
        *ReservedWord(pProgram, offset) = name;
        pState->m_DirtyFlags |= dirtyFlags;
    }
}

// a value of 1 for a bit: set or clear
DECOMP_ALWAYS_INLINE void SetRegisterFlag(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 index, bool isSet, u32 bit, u8 byteMask,
                                          u32 dirtyFlags = DIRTY_RESERVED_REGISTERS)
{
    SetRegisterBits(pProgram, pState, pTables, index, isSet ? bit : 0, bit, byteMask, dirtyFlags);
}

// the mapping functions of procedural textures
DECOMP_ALWAYS_INLINE bool GetProcTexMap(GLenum value, u32* pMap)
{
    switch (value - PT_MAP_FIRST) {
    case 0:
        *pMap = 0;
        return true;
    case 1:
        *pMap = 2;
        return true;
    case 2:
        *pMap = 1;
        return true;
    case 3:
        *pMap = 3;
        return true;
    case 4:
        *pMap = 4;
        return true;
    case 5:
        *pMap = 5;
        return true;
    case 7:
        *pMap = 6;
        return true;
    case 8:
        *pMap = 7;
        return true;
    case 9:
        *pMap = 8;
        return true;
    case 10:
        *pMap = 9;
        return true;
    default:
        return false;
    }
}

DECOMP_ALWAYS_INLINE bool GetProcTexClamp(GLenum value, u32* pClamp)
{
    switch (value) {
    case PT_CLAMP_0:
        *pClamp = 0;
        return true;
    case GL_CLAMP_TO_EDGE_VALUE:
        *pClamp = 1;
        return true;
    case PT_CLAMP_2:
        *pClamp = 2;
        return true;
    case GL_MIRRORED_REPEAT_VALUE:
        *pClamp = 3;
        return true;
    case PT_CLAMP_4:
        *pClamp = 4;
        return true;
    default:
        return false;
    }
}

// the functions of a texture combiner (DOT3_RGB only for the color)
DECOMP_ALWAYS_INLINE bool GetCombineFunction(GLenum value, bool isAlpha, u32* pFunction)
{
    switch (value) {
    case GL_REPLACE_VALUE:
        *pFunction = 0;
        return true;
    case GL_MODULATE_VALUE:
        *pFunction = 1;
        return true;
    case GL_ADD_VALUE:
        *pFunction = 2;
        return true;
    case GL_ADD_SIGNED_VALUE:
        *pFunction = 3;
        return true;
    case GL_INTERPOLATE_VALUE:
        *pFunction = 4;
        return true;
    case GL_SUBTRACT_VALUE:
        *pFunction = 5;
        return true;
    case GL_DOT3_RGB_VALUE:
        if (isAlpha) {
            return false;
        }
        *pFunction = 6;
        return true;
    case GL_DOT3_RGBA_VALUE:
        *pFunction = 7;
        return true;
    case MULT_ADD:
        *pFunction = 8;
        return true;
    case ADD_MULT:
        *pFunction = 9;
        return true;
    default:
        return false;
    }
}

// the three sources of a texture combiner, four bits each
DECOMP_ALWAYS_INLINE bool GetCombineSources(const GLint* pValues, u32* pSources)
{
    u32 sources = 0;
    for (u32 i = 0; i < 3; i++) {
        u32 source;
        switch (pValues[i]) {
        case GL_PRIMARY_COLOR_VALUE:
            source = 0;
            break;
        case FRAGMENT_PRIMARY_COLOR:
            source = 1;
            break;
        case FRAGMENT_SECONDARY_COLOR:
            source = 2;
            break;
        case GL_TEXTURE0_VALUE:
            source = 3;
            break;
        case GL_TEXTURE1_VALUE:
            source = 4;
            break;
        case GL_TEXTURE2_VALUE:
            source = 5;
            break;
        case GL_TEXTURE3_VALUE:
            source = 6;
            break;
        case PREVIOUS_BUFFER:
            source = 13;
            break;
        case GL_CONSTANT_VALUE:
            source = 14;
            break;
        case GL_PREVIOUS_VALUE:
            source = 15;
            break;
        default:
            return false;
        }
        sources |= source << (i * 4);
    }
    *pSources = sources;
    return true;
}

// the three color operands of a texture combiner, four bits each
DECOMP_ALWAYS_INLINE bool GetCombineOperandsRgb(const GLint* pValues, u32* pOperands)
{
    u32 operands = 0;
    for (u32 i = 0; i < 3; i++) {
        u32 operand;
        switch (pValues[i]) {
        case GL_SRC_COLOR_VALUE:
            operand = 0;
            break;
        case GL_ONE_MINUS_SRC_COLOR_VALUE:
            operand = 1;
            break;
        case GL_SRC_ALPHA_VALUE:
            operand = 2;
            break;
        case GL_ONE_MINUS_SRC_ALPHA_VALUE:
            operand = 3;
            break;
        case OPERAND_SRC_R:
            operand = 4;
            break;
        case OPERAND_ONE_MINUS_SRC_R:
            operand = 8;
            break;
        case OPERAND_SRC_G:
            operand = 12;
            break;
        case OPERAND_ONE_MINUS_SRC_G:
            operand = 5;
            break;
        case OPERAND_SRC_B:
            operand = 9;
            break;
        case OPERAND_ONE_MINUS_SRC_B:
            operand = 13;
            break;
        default:
            return false;
        }
        operands |= operand << (i * 4);
    }
    *pOperands = operands;
    return true;
}

// the three alpha operands of a texture combiner, four bits each
DECOMP_ALWAYS_INLINE bool GetCombineOperandsAlpha(const GLint* pValues, u32* pOperands)
{
    u32 operands = 0;
    for (u32 i = 0; i < 3; i++) {
        u32 operand;
        switch (pValues[i]) {
        case GL_SRC_ALPHA_VALUE:
            operand = 0;
            break;
        case GL_ONE_MINUS_SRC_ALPHA_VALUE:
            operand = 1;
            break;
        case OPERAND_SRC_R:
            operand = 2;
            break;
        case OPERAND_ONE_MINUS_SRC_R:
            operand = 4;
            break;
        case OPERAND_SRC_G:
            operand = 6;
            break;
        case OPERAND_ONE_MINUS_SRC_G:
            operand = 3;
            break;
        case OPERAND_SRC_B:
            operand = 5;
            break;
        case OPERAND_ONE_MINUS_SRC_B:
            operand = 7;
            break;
        default:
            return false;
        }
        operands |= operand << (i * 4);
    }
    *pOperands = operands;
    return true;
}

// the sampler type of texture unit 0: 2D (also shadow) or cube (also shadow cube) textures
DECOMP_ALWAYS_INLINE void SetSamplerType0(Program* pProgram, GlState* pState, GLint value)
{
    bool is2d;
    bool isCube;
    switch (value) {
    case 0:
        is2d = false;
        isCube = false;
        break;
    case GL_TEXTURE_2D_VALUE:
    case SAMPLER_SHADOW_2D:
    case SAMPLER_SHADOW_2D_1:
        is2d = true;
        isCube = false;
        break;
    case GL_TEXTURE_CUBE_MAP_VALUE:
    case SAMPLER_SHADOW_CUBE:
        is2d = false;
        isCube = true;
        break;
    default:
        return;
    }
    if (pState->m_IsTexture2dEnabled[0] != is2d) {
        pState->m_IsTexture2dEnabled[0] = is2d;
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0;
    }
    if (pState->m_IsTextureCubeEnabled[0] != isCube) {
        pState->m_IsTextureCubeEnabled[0] = isCube;
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0;
    }
    pProgram->m_SamplerTypes[0] = value;
    if (pState->m_SamplerTypes[0] != static_cast<GLenum>(value)) {
        pState->m_SamplerTypes[0] = value;
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0;
    }
}

// the sampler type of texture unit 1 or 2 (2D textures only)
DECOMP_ALWAYS_INLINE void SetSamplerType(Program* pProgram, GlState* pState, u32 unit, GLint value)
{
    bool is2d;
    if (value == 0) {
        is2d = false;
    } else if (value == GL_TEXTURE_2D_VALUE) {
        is2d = true;
    } else {
        return;
    }
    if (pState->m_IsTexture2dEnabled[unit] != is2d) {
        pState->m_IsTexture2dEnabled[unit] = is2d;
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0 << unit;
    }
    pProgram->m_SamplerTypes[unit] = value;
    if (pState->m_SamplerTypes[unit] != static_cast<GLenum>(value)) {
        pState->m_SamplerTypes[unit] = value;
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0 << unit;
    }
}

// the shadow flag of the lighting: any of primary, secondary and alpha
DECOMP_ALWAYS_INLINE void SetShadow(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 offset, u32 bit, GLint value)
{
    *ReservedBool(pProgram, offset) = value != 0;
    SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, value != 0, bit, 0x4);
    bool isShadowed = *ReservedBool(pProgram, RESERVED_IS_SHADOW_PRIMARY) || *ReservedBool(pProgram, RESERVED_IS_SHADOW_SECONDARY) ||
                      *ReservedBool(pProgram, RESERVED_IS_SHADOW_ALPHA);
    SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, isShadowed, 0x1, 0x1);
}

// the lighting on or off (the configuration of the lookup tables only while it is on)
DECOMP_ALWAYS_INLINE void SetLightingEnabled(Program* pProgram, GlState* pState, ShaderTables* pTables, GLint value)
{
    if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_ENABLE] & 1) != 0) {
        if (value == 0) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, 0, 0xF0, 0x1);
        }
    } else if (value != 0) {
        u32 config = *ReservedWord(pProgram, RESERVED_LIGHTING_CONFIG);
        u32 configValue = 1;
        for (u32 i = 0; i < 8; i++) {
            if (s_LightingConfigs[i] == config) {
                configValue = GetLightingConfigValue(i);
                break;
            }
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, configValue << 4, 0xF0, 0x1);
    }
    SetRegister(pProgram, pState, pTables, REGISTER_LIGHTING_DISABLED, value == 0 ? 1 : 0);
    SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_ENABLE, value != 0, 0x1, 0x1, DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
}

// a light on or off: the number of lights and the order of the enabled ones
DECOMP_ALWAYS_INLINE void SetLightEnabled(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 light, GLint value)
{
    bool* pIsEnabled = ReservedBool(pProgram, RESERVED_LIGHTS + light * LIGHT_SIZE + LIGHT_IS_ENABLED);
    if (*pIsEnabled == value) {
        return;
    }
    *pIsEnabled = value != 0;
    u32 count = 0;
    u32 permutation = 0;
    for (u32 i = 0; i < LIGHT_COUNT; i++) {
        if (*ReservedBool(pProgram, RESERVED_LIGHTS + i * LIGHT_SIZE + LIGHT_IS_ENABLED)) {
            permutation |= i << (count * 4);
            count++;
        }
    }
    SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_COUNT, count != 0 ? count - 1 : 0);
    SetRegister(pProgram, pState, pTables, REGISTER_LIGHT_PERMUTATION, permutation);
    if (value != 0) {
        // (spot light or distance attenuation on: their lookup tables)
        u32 config = pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG1];
        if ((config & (1 << (light + 8))) == 0 || (config & (1 << (light + 24))) == 0) {
            pState->m_DirtyFlags |= DIRTY_LIGHTING_LUTS;
        }
    }
}

// the inputs of the texture combiners from the combiner buffer (combiners 1 to 4): color, alpha
DECOMP_ALWAYS_INLINE void SetBufferInput(Program* pProgram, GlState* pState, ShaderTables* pTables, u32 combiner, const GLint* pValues)
{
    u32 value;
    if (pValues[0] == GL_PREVIOUS_VALUE) {
        value = 1 << (combiner + 8);
    } else if (pValues[0] == PREVIOUS_BUFFER) {
        value = 0;
    } else {
        return;
    }
    if (pValues[1] == GL_PREVIOUS_VALUE) {
        value |= 1 << (combiner + 12);
    } else if (pValues[1] != PREVIOUS_BUFFER) {
        return;
    }
    u32 bits = 0x1100 << combiner;
    SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, value, bits, ByteMaskOf(bits));
}

// the fog mode: none, fog or gas
DECOMP_ALWAYS_INLINE void SetFogMode(Program* pProgram, GlState* pState, ShaderTables* pTables, GLint value)
{
    switch (value) {
    case 0:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, 0, 0x7, 0x1);
        break;
    case GL_FOG_VALUE:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, 5, 0x7, 0x1, DIRTY_RESERVED_REGISTERS | DIRTY_FOG);
        break;
    case GAS_FOG_MODE:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, 7, 0x7, 0x1, DIRTY_RESERVED_REGISTERS | DIRTY_FOG | DIRTY_GAS);
        break;
    default:
        break;
    }
}

// the reserved integer uniforms
DECOMP_ALWAYS_INLINE void SetReservedUniformInt(u32 uniform, const GLint* pValues)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    GlState* pState = s_pGlState;
    Program* pProgram = pTables->m_pCurrentProgram;
    const GLint value = pValues[0];
    u32 code;

    switch (uniform) {
    case UNIFORM_PERSPECTIVE_SHADOW:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_SHADOW, value == 0, 0x1, 0x1);
        break;
    case UNIFORM_SAMPLER_TYPE + 0:
        SetSamplerType0(pProgram, pState, value);
        break;
    case UNIFORM_SAMPLER_TYPE + 1:
        SetSamplerType(pProgram, pState, 1, value);
        break;
    case UNIFORM_SAMPLER_TYPE + 2:
        SetSamplerType(pProgram, pState, 2, value);
        break;
    case UNIFORM_SAMPLER_TYPE + 3:
        // texture unit 3: procedural textures
        if (value == 0) {
            code = 0;
        } else if (value == SAMPLER_PROCEDURAL) {
            code = 0x400;
        } else {
            break;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_TEXTURE_CONFIG, code, 0x400, 0x2, DIRTY_RESERVED_REGISTERS | DIRTY_PROCEDURAL_TEXTURE);
        *ReservedWord(pProgram, RESERVED_PT_SAMPLER_TYPE) = value;
        break;
    case UNIFORM_TEXCOORD2:
        if (value == GL_TEXTURE1_VALUE) {
            code = 0x2000;
        } else if (value == GL_TEXTURE2_VALUE) {
            code = 0;
        } else {
            break;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_TEXTURE_CONFIG, code, 0x2000, 0x2);
        break;
    case UNIFORM_TEXCOORD3:
        if (value == GL_TEXTURE0_VALUE) {
            code = 0;
        } else if (value == GL_TEXTURE1_VALUE) {
            code = 0x100;
        } else if (value == GL_TEXTURE2_VALUE) {
            code = 0x200;
        } else {
            break;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_TEXTURE_CONFIG, code, 0x300, 0x2);
        break;
    case UNIFORM_PT_RGB_MAP:
        if (GetProcTexMap(value, &code)) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, code << 6, 0x3C0, 0x3);
        }
        break;
    case UNIFORM_PT_ALPHA_MAP:
        if (GetProcTexMap(value, &code)) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, code << 10, 0x3C00, 0x2);
        }
        break;
    case UNIFORM_PT_ALPHA_SEPARATE:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_PT_CONTROL, value != 0, 0x4000, 0x2, DIRTY_RESERVED_REGISTERS | DIRTY_PROCEDURAL_TEXTURE);
        break;
    case UNIFORM_PT_CLAMP_U:
        if (GetProcTexClamp(value, &code)) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, code, 0x7, 0x1);
        }
        break;
    case UNIFORM_PT_CLAMP_V:
        if (GetProcTexClamp(value, &code)) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, code << 3, 0x38, 0x1);
        }
        break;
    case UNIFORM_PT_SHIFT_U:
        if (static_cast<u32>(value - PT_SHIFT_FIRST) <= 2) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, (value - PT_SHIFT_FIRST) << 16, 0x30000, 0x4);
        }
        break;
    case UNIFORM_PT_SHIFT_V:
        if (static_cast<u32>(value - PT_SHIFT_FIRST) <= 2) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_CONTROL, (value - PT_SHIFT_FIRST) << 18, 0xC0000, 0x4);
        }
        break;
    case UNIFORM_PT_MIN_FILTER:
        switch (value) {
        case GL_NEAREST_VALUE:
            code = 0;
            break;
        case GL_LINEAR_VALUE:
            code = 1;
            break;
        case GL_NEAREST_MIPMAP_NEAREST_VALUE:
            code = 2;
            break;
        case GL_LINEAR_MIPMAP_NEAREST_VALUE:
            code = 3;
            break;
        case GL_NEAREST_MIPMAP_LINEAR_VALUE:
            code = 4;
            break;
        case GL_LINEAR_MIPMAP_LINEAR_VALUE:
            code = 5;
            break;
        default:
            return;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_LUT, code, 0x7, 0x1);
        break;
    case UNIFORM_PT_TEX_WIDTH:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_LUT, (value & 0xFF) << 11, 0x7F800, 0x6);
        break;
    case UNIFORM_PT_TEX_OFFSET:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_PT_LUT_OFFSET, value & 0xFF, 0xFF, 0x1);
        break;
    case UNIFORM_PT_NOISE_ENABLE:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_PT_CONTROL, value != 0, 0x8000, 0x2, DIRTY_RESERVED_REGISTERS | DIRTY_PROCEDURAL_TEXTURE);
        break;
    case UNIFORM_PT_LUTS + 0:
    case UNIFORM_PT_LUTS + 1:
    case UNIFORM_PT_LUTS + 2:
    case UNIFORM_PT_LUTS + 3:
    case UNIFORM_PT_LUTS + 4:
    case UNIFORM_PT_LUTS + 5:
    case UNIFORM_PT_LUTS + 6: {
        u32 table = uniform - UNIFORM_PT_LUTS;
        SetLookupTable(pProgram, pState, RESERVED_PT_LUTS + table * 4, LUT_SLOT_PT + table, value, DIRTY_PROCEDURAL_TEXTURE);
        break;
    }
    case UNIFORM_FRAGMENT_MODE:
        if (value == FRAGMENT_MODE_DEFAULT) {
            code = 0xE40000;
        } else if (value == FRAGMENT_MODE_SHADOW) {
            code = 0xE40003;
        } else if (value == FRAGMENT_MODE_GAS) {
            code = 0xE40001;
        } else {
            break;
        }
        pProgram->m_Mode = value;
        SetRegisterBits(pProgram, pState, pTables, REGISTER_FRAGMENT_MODE, code, 0xFFFF00FF, 0xD);
        pState->m_DirtyFlags |= DIRTY_FRAGMENT_MODE;
        break;
    case UNIFORM_GAS_AUTO_ACC:
        pProgram->m_IsGasAutoAcc = value != 0;
        break;
    case UNIFORM_GAS_COLOR_LUT_INPUT:
        if (value == GAS_COLOR_LUT_INPUT_0) {
            code = 0;
        } else if (value == GAS_COLOR_LUT_INPUT_1) {
            code = 0x100;
        } else {
            break;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_CONTROL, code, 0x100, 0x2);
        break;
    case UNIFORM_GAS_SHADING_DENSITY_SOURCE:
        if (value == GAS_SHADING_DENSITY_0) {
            code = 0;
        } else if (value == GAS_SHADING_DENSITY_1) {
            code = 0x8;
        } else {
            break;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, code, 0x8, 0x1);
        break;
    case UNIFORM_GAS_LUTS + 0:
    case UNIFORM_GAS_LUTS + 1:
    case UNIFORM_GAS_LUTS + 2: {
        u32 table = uniform - UNIFORM_GAS_LUTS;
        SetLookupTable(pProgram, pState, RESERVED_GAS_LUTS + table * 4, LUT_SLOT_GAS + table, value, DIRTY_GAS);
        break;
    }
    case UNIFORM_LIGHTING_ENABLED:
        SetLightingEnabled(pProgram, pState, pTables, value);
        break;
    case UNIFORM_LIGHT_ENABLED + 0:
    case UNIFORM_LIGHT_ENABLED + 1:
    case UNIFORM_LIGHT_ENABLED + 2:
    case UNIFORM_LIGHT_ENABLED + 3:
    case UNIFORM_LIGHT_ENABLED + 4:
    case UNIFORM_LIGHT_ENABLED + 5:
    case UNIFORM_LIGHT_ENABLED + 6:
    case UNIFORM_LIGHT_ENABLED + 7:
        SetLightEnabled(pProgram, pState, pTables, uniform - UNIFORM_LIGHT_ENABLED, value);
        break;
    case UNIFORM_LIGHT_SHADOWED + 0:
    case UNIFORM_LIGHT_SHADOWED + 1:
    case UNIFORM_LIGHT_SHADOWED + 2:
    case UNIFORM_LIGHT_SHADOWED + 3:
    case UNIFORM_LIGHT_SHADOWED + 4:
    case UNIFORM_LIGHT_SHADOWED + 5:
    case UNIFORM_LIGHT_SHADOWED + 6:
    case UNIFORM_LIGHT_SHADOWED + 7: {
        // (the register bits disable)
        u32 bit = 1 << (uniform - UNIFORM_LIGHT_SHADOWED);
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, bit, ByteMaskOf(bit));
        break;
    }
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 0:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 1:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 2:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 3:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 4:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 5:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 6:
    case UNIFORM_LIGHT_GEOM_FACTOR0 + 7:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHT_CONFIG + (uniform - UNIFORM_LIGHT_GEOM_FACTOR0) * LIGHT_REGISTER_STRIDE, value != 0,
                        0x4, 0x1);
        break;
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 0:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 1:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 2:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 3:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 4:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 5:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 6:
    case UNIFORM_LIGHT_GEOM_FACTOR1 + 7:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHT_CONFIG + (uniform - UNIFORM_LIGHT_GEOM_FACTOR1) * LIGHT_REGISTER_STRIDE, value != 0,
                        0x8, 0x1);
        break;
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 0:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 1:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 2:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 3:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 4:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 5:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 6:
    case UNIFORM_LIGHT_TWO_SIDE_DIFFUSE + 7:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHT_CONFIG + (uniform - UNIFORM_LIGHT_TWO_SIDE_DIFFUSE) * LIGHT_REGISTER_STRIDE,
                        value != 0, 0x2, 0x1);
        break;
    case UNIFORM_LIGHT_LUT_SP + 0:
    case UNIFORM_LIGHT_LUT_SP + 1:
    case UNIFORM_LIGHT_LUT_SP + 2:
    case UNIFORM_LIGHT_LUT_SP + 3:
    case UNIFORM_LIGHT_LUT_SP + 4:
    case UNIFORM_LIGHT_LUT_SP + 5:
    case UNIFORM_LIGHT_LUT_SP + 6:
    case UNIFORM_LIGHT_LUT_SP + 7: {
        u32 light = uniform - UNIFORM_LIGHT_LUT_SP;
        SetLookupTable(pProgram, pState, RESERVED_LIGHTS + light * LIGHT_SIZE + LIGHT_LUT_SP, LUT_SLOT_SP + light, value, DIRTY_LIGHTING_LUTS);
        break;
    }
    case UNIFORM_LIGHT_SPOT_ENABLED + 0:
    case UNIFORM_LIGHT_SPOT_ENABLED + 1:
    case UNIFORM_LIGHT_SPOT_ENABLED + 2:
    case UNIFORM_LIGHT_SPOT_ENABLED + 3:
    case UNIFORM_LIGHT_SPOT_ENABLED + 4:
    case UNIFORM_LIGHT_SPOT_ENABLED + 5:
    case UNIFORM_LIGHT_SPOT_ENABLED + 6:
    case UNIFORM_LIGHT_SPOT_ENABLED + 7: {
        u32 bit = 1 << (uniform - UNIFORM_LIGHT_SPOT_ENABLED + 8);
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, bit, ByteMaskOf(bit),
                        DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        break;
    }
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 0:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 1:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 2:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 3:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 4:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 5:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 6:
    case UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 7: {
        u32 bit = 1 << (uniform - UNIFORM_LIGHT_DISTANCE_ATTENUATION_ENABLED + 24);
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, bit, ByteMaskOf(bit),
                        DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        break;
    }
    case UNIFORM_LIGHT_LUT_DA + 0:
    case UNIFORM_LIGHT_LUT_DA + 1:
    case UNIFORM_LIGHT_LUT_DA + 2:
    case UNIFORM_LIGHT_LUT_DA + 3:
    case UNIFORM_LIGHT_LUT_DA + 4:
    case UNIFORM_LIGHT_LUT_DA + 5:
    case UNIFORM_LIGHT_LUT_DA + 6:
    case UNIFORM_LIGHT_LUT_DA + 7: {
        u32 light = uniform - UNIFORM_LIGHT_LUT_DA;
        SetLookupTable(pProgram, pState, RESERVED_LIGHTS + light * LIGHT_SIZE + LIGHT_LUT_DA, LUT_SLOT_DA + light, value, DIRTY_LIGHTING_LUTS);
        break;
    }
    case UNIFORM_ABS_LUT_INPUT + 0:
    case UNIFORM_ABS_LUT_INPUT + 1:
    case UNIFORM_ABS_LUT_INPUT + 2:
    case UNIFORM_ABS_LUT_INPUT + 3:
    case UNIFORM_ABS_LUT_INPUT + 4:
    case UNIFORM_ABS_LUT_INPUT + 5:
    case UNIFORM_ABS_LUT_INPUT + 6: {
        // (the register bits disable the absolute value)
        u32 bit = 1 << ((uniform - UNIFORM_ABS_LUT_INPUT) * 4 + 1);
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LUT_INPUT_ABS, value == 0, bit, ByteMaskOf(bit));
        break;
    }
    case UNIFORM_LUT_INPUT + 0:
    case UNIFORM_LUT_INPUT + 1:
    case UNIFORM_LUT_INPUT + 2:
    case UNIFORM_LUT_INPUT + 3:
    case UNIFORM_LUT_INPUT + 4:
    case UNIFORM_LUT_INPUT + 5:
    case UNIFORM_LUT_INPUT + 6: {
        u32 shift = (uniform - UNIFORM_LUT_INPUT) * 4;
        u32 bits = 0x7 << shift;
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LUT_INPUT, (value & 0x7) << shift, bits, ByteMaskOf(bits));
        break;
    }
    case UNIFORM_MATERIAL_LUTS + 0:
    case UNIFORM_MATERIAL_LUTS + 1:
    case UNIFORM_MATERIAL_LUTS + 2:
    case UNIFORM_MATERIAL_LUTS + 3:
    case UNIFORM_MATERIAL_LUTS + 4:
    case UNIFORM_MATERIAL_LUTS + 5: {
        u32 table = uniform - UNIFORM_MATERIAL_LUTS;
        SetLookupTable(pProgram, pState, RESERVED_MATERIAL_LUTS + table * 4, LUT_SLOT_MATERIAL + table, value, DIRTY_LIGHTING_LUTS);
        break;
    }
    case UNIFORM_SHADOW_SELECTOR:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, (value - GL_TEXTURE0_VALUE) << 24, 0x3000000, 0x8);
        break;
    case UNIFORM_BUMP_SELECTOR:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, (value - GL_TEXTURE0_VALUE) << 22, 0xC00000, 0x4);
        break;
    case UNIFORM_BUMP_MODE: {
        // (with the renormalization off for a bump map)
        u32 isRenormDisabled = value != BUMP_MODE_NONE && !*ReservedBool(pProgram, RESERVED_IS_BUMP_RENORM) ? 0x40000000 : 0;
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, ((value - BUMP_MODE_NONE) << 28) | isRenormDisabled, 0x70000000, 0x8);
        break;
    }
    case UNIFORM_BUMP_RENORM: {
        u32 bumpMode = (pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG0] << 2) >> 30;
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, value == 0 && bumpMode != 0, 0x40000000, 0x8);
        *ReservedBool(pProgram, RESERVED_IS_BUMP_RENORM) = value != 0;
        break;
    }
    case UNIFORM_LIGHTING_CONFIG: {
        u32 index = value - LIGHTING_CONFIG_FIRST;
        if (index >= 8) {
            break;
        }
        code = GetLightingConfigValue(index);
        *ReservedWord(pProgram, RESERVED_LIGHTING_CONFIG) = s_LightingConfigs[index];
        if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_ENABLE] & 1) != 0) {
            SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, code << 4, 0xF0, 0x1);
        }
        break;
    }
    case UNIFORM_INVERT_SHADOW:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, value != 0, 0x40000, 0x4);
        break;
    case UNIFORM_SHADOW_PRIMARY:
        SetShadow(pProgram, pState, pTables, RESERVED_IS_SHADOW_PRIMARY, 0x10000, value);
        break;
    case UNIFORM_SHADOW_SECONDARY:
        SetShadow(pProgram, pState, pTables, RESERVED_IS_SHADOW_SECONDARY, 0x20000, value);
        break;
    case UNIFORM_SHADOW_ALPHA:
        SetShadow(pProgram, pState, pTables, RESERVED_IS_SHADOW_ALPHA, 0x80000, value);
        break;
    case UNIFORM_FRESNEL_SELECTOR:
        SetRegisterBits(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, value << 2, 0xC, 0x1);
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == FRESNEL_NONE, 0x80000, 0x4,
                        DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        break;
    case UNIFORM_CLAMP_HIGHLIGHTS:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG0, value != 0, 0x8000000, 0x8);
        break;
    case UNIFORM_LUT_ENABLED_D0:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, 0x10000, 0x4, DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        break;
    case UNIFORM_LUT_ENABLED_D1:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, 0x20000, 0x4, DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        break;
    case UNIFORM_LUT_ENABLED_REFL:
        // (the reflection tables of all three colors; the specular colors 1 of the lights again)
        if (*ReservedBool(pProgram, RESERVED_IS_REFLECTION_LUT_DISABLED) == (value == 0)) {
            break;
        }
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_LIGHTING_CONFIG1, value == 0, 0x700000, 0x4,
                        DIRTY_RESERVED_REGISTERS | DIRTY_LIGHTING_LUTS);
        *ReservedBool(pProgram, RESERVED_IS_REFLECTION_LUT_DISABLED) = value == 0;
        for (u32 light = 0; light < LIGHT_COUNT; light++) {
            SetLightSpecular1(pProgram, pState, pTables, light, LightFloats(pProgram, light, LIGHT_SPECULAR1));
        }
        break;
    case UNIFORM_TEX_ENV_COMBINE_RGB + 0:
    case UNIFORM_TEX_ENV_COMBINE_RGB + 1:
    case UNIFORM_TEX_ENV_COMBINE_RGB + 2:
    case UNIFORM_TEX_ENV_COMBINE_RGB + 3:
    case UNIFORM_TEX_ENV_COMBINE_RGB + 4:
    case UNIFORM_TEX_ENV_COMBINE_RGB + 5:
        if (GetCombineFunction(value, false, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_COMBINER + (uniform - UNIFORM_TEX_ENV_COMBINE_RGB) * TEX_ENV_REGISTER_STRIDE, code, 0xF, 0x1);
        }
        break;
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 0:
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 1:
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 2:
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 3:
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 4:
    case UNIFORM_TEX_ENV_COMBINE_ALPHA + 5:
        if (GetCombineFunction(value, true, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_COMBINER + (uniform - UNIFORM_TEX_ENV_COMBINE_ALPHA) * TEX_ENV_REGISTER_STRIDE, code << 16, 0xF0000,
                            0x4);
        }
        break;
    case UNIFORM_TEX_ENV_SOURCE_RGB + 0:
    case UNIFORM_TEX_ENV_SOURCE_RGB + 1:
    case UNIFORM_TEX_ENV_SOURCE_RGB + 2:
    case UNIFORM_TEX_ENV_SOURCE_RGB + 3:
    case UNIFORM_TEX_ENV_SOURCE_RGB + 4:
    case UNIFORM_TEX_ENV_SOURCE_RGB + 5:
        if (GetCombineSources(pValues, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_SOURCE + (uniform - UNIFORM_TEX_ENV_SOURCE_RGB) * TEX_ENV_REGISTER_STRIDE, code, 0xFFF, 0x3);
        }
        break;
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 0:
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 1:
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 2:
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 3:
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 4:
    case UNIFORM_TEX_ENV_SOURCE_ALPHA + 5:
        if (GetCombineSources(pValues, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_SOURCE + (uniform - UNIFORM_TEX_ENV_SOURCE_ALPHA) * TEX_ENV_REGISTER_STRIDE, code << 16, 0xFFF0000,
                            0xC);
        }
        break;
    case UNIFORM_TEX_ENV_OPERAND_RGB + 0:
    case UNIFORM_TEX_ENV_OPERAND_RGB + 1:
    case UNIFORM_TEX_ENV_OPERAND_RGB + 2:
    case UNIFORM_TEX_ENV_OPERAND_RGB + 3:
    case UNIFORM_TEX_ENV_OPERAND_RGB + 4:
    case UNIFORM_TEX_ENV_OPERAND_RGB + 5:
        if (GetCombineOperandsRgb(pValues, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_OPERAND + (uniform - UNIFORM_TEX_ENV_OPERAND_RGB) * TEX_ENV_REGISTER_STRIDE, code, 0xFFF, 0x3);
        }
        break;
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 0:
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 1:
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 2:
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 3:
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 4:
    case UNIFORM_TEX_ENV_OPERAND_ALPHA + 5:
        if (GetCombineOperandsAlpha(pValues, &code)) {
            SetRegisterBits(pProgram, pState, pTables,
                            REGISTER_TEX_ENV_OPERAND + (uniform - UNIFORM_TEX_ENV_OPERAND_ALPHA) * TEX_ENV_REGISTER_STRIDE, code << 12, 0x777000,
                            0x6);
        }
        break;
    case UNIFORM_TEX_ENV_BUFFER_INPUT + 0:
    case UNIFORM_TEX_ENV_BUFFER_INPUT + 1:
    case UNIFORM_TEX_ENV_BUFFER_INPUT + 2:
    case UNIFORM_TEX_ENV_BUFFER_INPUT + 3:
        SetBufferInput(pProgram, pState, pTables, uniform - UNIFORM_TEX_ENV_BUFFER_INPUT, pValues);
        break;
    case UNIFORM_FOG_MODE:
        SetFogMode(pProgram, pState, pTables, value);
        break;
    case UNIFORM_FOG_Z_FLIP:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_GAS_FOG_MODE, value != 0, 0x10000, 0x4);
        break;
    case UNIFORM_FOG_LUT:
        SetLookupTable(pProgram, pState, RESERVED_FOG_LUT, LUT_SLOT_FOG, value, DIRTY_FOG);
        break;
    case UNIFORM_ENABLE_CLIPPING_PLANE:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_CLIPPING_ENABLE, value != 0, 0x1, 0x1);
        break;
    case UNIFORM_ENABLE_ALPHA_TEST:
        SetRegisterFlag(pProgram, pState, pTables, REGISTER_ALPHA_TEST, value != 0, 0x1, 0x1);
        break;
    case UNIFORM_ALPHA_TEST_FUNC:
        // (GL_NEVER ... GL_ALWAYS in the order of the GPU)
        switch (value - GL_NEVER_VALUE) {
        case 0:
            code = 0;
            break;
        case 7:
            code = 1;
            break;
        case 2:
            code = 2;
            break;
        case 5:
            code = 3;
            break;
        case 1:
            code = 4;
            break;
        case 3:
            code = 5;
            break;
        case 4:
            code = 6;
            break;
        case 6:
            code = 7;
            break;
        default:
            return;
        }
        SetRegisterBits(pProgram, pState, pTables, REGISTER_ALPHA_TEST, code << 4, 0x70, 0x1);
        break;
    default:
        break;
    }
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// (the other reserved uniforms are integers; the values of a matrix are its rows, components values
// per row; the components go to the register from the last one down, starting at the swizzle)
// 0x007B9794 (name is ours)
void SetUniformFloat(GLint location, const GLfloat* pValues, GLsizei components, GLsizei count, bool isTransposeNeeded, bool isMatrix)
{
    if (location == -1 || count == 0) {
        return;
    }
    if ((location & LOCATION_RESERVED) != 0) {
        SetReservedUniformFloat((static_cast<u32>(location) << 14) >> 16, pValues);
        return;
    }

    ShaderTables* pTables = s_ShaderManager.m_pTables;
    GlState* pState = s_pGlState;
    Program* pProgram = pTables->m_pCurrentProgram;
    const u32 info = pProgram->m_pLocations[(static_cast<u32>(location) << 14) >> 21].m_Info;
    const u32 element = location & 0x7F;
    const u32 firstRegister = info >> 24;
    const s32 last = 3 - ((info << 19) >> 30);
    const u32 registersPerElement = isMatrix ? ((info << 17) >> 30) + 1 : 1;
    pState->m_DirtyFlags |= DIRTY_SHADER_UNIFORMS;
    u32* pFloatValues;
    u32* pDirty;
    if ((info & INFO_GEOMETRY) != 0) {
        pFloatValues = pProgram->m_pGeometryFloatValues;
        pDirty = pProgram->m_GeometryDirtyFloatRegisters;
    } else {
        pFloatValues = pProgram->m_pVertexFloatValues;
        pDirty = pProgram->m_VertexDirtyFloatRegisters;
    }
    for (s32 i = 0; i < static_cast<s32>(registersPerElement * count); i++) {
        u32 index = firstRegister + registersPerElement * element + i;
        pDirty[index >> 5] |= 1 << (index & 31);
    }

    if (isMatrix) {
        for (s32 matrix = 0; matrix < count; matrix++) {
            u32* pRegisters = pFloatValues + (firstRegister + (element + matrix) * registersPerElement) * 4;
            for (s32 row = 0; row < components; row++) {
                const GLfloat* pRow = pValues + (matrix * components + row) * components;
                if (isTransposeNeeded) {
                    for (s32 column = 0; column < components; column++) {
                        pRegisters[column * 4 + last - row] = BitsOf(pRow[column]);
                    }
                } else {
                    for (s32 column = 0; column < components; column++) {
                        pRegisters[row * 4 + last - column] = BitsOf(pRow[column]);
                    }
                }
            }
        }
    } else {
        const GLfloat* pValue = pValues;
        for (s32 i = 0; i < count; i++) {
            u32* pRegister = pFloatValues + (firstRegister + element + i) * 4;
            for (s32 component = 0; component < components; component++) {
                pRegister[last - component] = BitsOf(pValue[component]);
            }
            pValue += components;
        }
    }
}

// (the integer uniforms of the shaders are ivec3, one integer register each with eight bits per
// component, the bool uniforms bits of the bool register; the components are not used)
// 0x007BE1E4 (name is ours)
DECOMP_NOINLINE void SetUniformInt(GLint location, const GLint* pValues, GLsizei components, GLsizei count)
{
    if (location == -1 || count == 0) {
        return;
    }
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    GlState* pState = s_pGlState;
    Program* pProgram = pTables->m_pCurrentProgram;
    s_LastIntLocation.m_Location = location;
    if ((location & LOCATION_RESERVED) != 0) {
        SetReservedUniformInt((static_cast<u32>(location) << 14) >> 16, pValues);
        return;
    }

    const u32 element = location & 0x7F;
    s_LastIntLocation = pProgram->m_pLocations[(static_cast<u32>(location) << 14) >> 21];
    if ((s_LastIntLocation.m_Info & INFO_NOT_BOOL) != 0) {
        if (((s_LastIntLocation.m_Info << 17) >> 30) != 2) {
            return;
        }
        for (s32 i = 0; i < count; i++) {
            u32 index = ((s_LastIntLocation.m_Info & INFO_GEOMETRY) != 0 ? REGISTER_GEOMETRY_INTEGER : REGISTER_VERTEX_INTEGER) +
                        (s_LastIntLocation.m_Info >> 24) + element + i;
            const GLint* pValue = pValues + i * 3;
            pProgram->m_RegisterValues[index] = (pValue[0] & 0xFF) | ((pValue[1] & 0xFF) << 8) | ((pValue[2] & 0xFF) << 16);
            pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
            if (pState->m_IsCommandGenerationDeferred) {
                pTables->m_SentRegisterValues[index] = ~pProgram->m_RegisterValues[index];
            }
        }
    } else {
        u32 index = (s_LastIntLocation.m_Info & INFO_GEOMETRY) != 0 ? REGISTER_GEOMETRY_BOOL : REGISTER_VERTEX_BOOL;
        for (s32 i = 0; i < count; i++) {
            u32 bit = 1 << (element + (s_LastIntLocation.m_Info >> 24) + i);
            if (pValues[i] != 0) {
                pProgram->m_RegisterValues[index] |= bit;
            } else {
                pProgram->m_RegisterValues[index] &= ~bit;
            }
        }
        if (pState->m_IsCommandGenerationDeferred) {
            pTables->m_SentRegisterValues[index] = ~pProgram->m_RegisterValues[index];
        }
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
    }
    pState->m_DirtyFlags |= DIRTY_SHADER_UNIFORMS;
}
} // namespace detail
} // namespace gx
} // namespace nn

// (falls into SetUniformInt; nw::font sets its uniforms with it)
// 0x007BE1D4 (name after the OpenGL ES API)
extern "C" void glUniform3iv(GLint location, GLsizei count, const GLint* pValues)
{
    nn::gx::detail::SetUniformInt(location, pValues, 3, count);
}
