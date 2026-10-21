#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gxlow/CTR/CTR_Api.h"

// The texture validator of the GL state (__txv_*): the textures of the units become the values of
// the texture registers of the GPU (0x80-0x9E; the fields after 3dbrew), which go into the command
// buffer. Unit 0 has its registers at 0x81-0x8E (with the six addresses of a cube map), units 1 and
// 2 at 0x91-0x96 and 0x99-0x9E. All names are ours except those of the __txv_ functions.

using namespace nn::gx::detail;

namespace {
// the errors (the OpenGL ES values)
const GLenum ERROR_INVALID_VALUE = 0x501;
const GLenum ERROR_INVALID_OPERATION = 0x502;
// the parameters of a texture (the OpenGL ES values)
const GLenum FILTER_NEAREST = 0x2600;
const GLenum FILTER_LINEAR = 0x2601;
const GLenum FILTER_NEAREST_MIPMAP_NEAREST = 0x2700;
const GLenum FILTER_LINEAR_MIPMAP_NEAREST = 0x2701;
const GLenum FILTER_NEAREST_MIPMAP_LINEAR = 0x2702;
const GLenum FILTER_LINEAR_MIPMAP_LINEAR = 0x2703;
const GLenum WRAP_REPEAT = 0x2901;
const GLenum WRAP_CLAMP_TO_BORDER = 0x812D;
const GLenum WRAP_CLAMP_TO_EDGE = 0x812F;
const GLenum WRAP_MIRRORED_REPEAT = 0x8370;
// the sampler type of unit 0 for a projection texture (the uniform dmp_Texture[0].samplerType)
const GLenum SAMPLER_TYPE_PROJECTION = 0x6E00;

// the registers
const u32 REG_FIRST = 0x80;
const u32 REG_CONFIG = 0x80;
const u32 REG_UNIT0_BORDER = 0x81;
const u32 REG_UNIT0_DIMENSION = 0x82;
const u32 REG_UNIT0_PARAMETER = 0x83;
const u32 REG_UNIT0_LOD = 0x84;
const u32 REG_UNIT0_ADDRESS = 0x85; // (0x85-0x8A: the faces of a cube map)
const u32 REG_UNIT0_SHADOW = 0x8B;
const u32 REG_UNIT0_TYPE = 0x8E;
const u32 REG_UNIT1_BORDER = 0x91;
const u32 REG_UNIT2_BORDER = 0x99;
// the registers of units 1 and 2 after their border color
const u32 UNIT_DIMENSION = 1;
const u32 UNIT_PARAMETER = 2;
const u32 UNIT_LOD = 3;
const u32 UNIT_ADDRESS = 4;
const u32 UNIT_TYPE = 5;
const u32 UNIT_REGISTER_COUNT = 6;
const u32 UNIT0_REGISTER_COUNT = 10;

// the fields of the parameter register
const u32 PARAMETER_MAG_LINEAR = 1 << 1;
const u32 PARAMETER_MIN_LINEAR = 1 << 2;
const u32 PARAMETER_ETC1_SHIFT = 4;
const u32 PARAMETER_ETC1_MASK = 3 << 4;
const u32 PARAMETER_UNKNOWN_6 = 3 << 6;
const u32 PARAMETER_WRAP_T_SHIFT = 8;
const u32 PARAMETER_WRAP_T_MASK = 7 << 8;
const u32 PARAMETER_WRAP_S_SHIFT = 12;
const u32 PARAMETER_WRAP_S_MASK = 7 << 12;
const u32 PARAMETER_UNKNOWN_20 = 1 << 20;
const u32 PARAMETER_MIPMAP_LINEAR = 1 << 24;
const u32 PARAMETER_TYPE_SHIFT = 28;
const u32 PARAMETER_TYPE_MASK = 7 << 28;
// the texture types and wrap modes of the GPU
const u32 TYPE_2D = 0;
const u32 TYPE_CUBE = 1;
const u32 TYPE_SHADOW_2D = 2;
const u32 TYPE_PROJECTION = 3;
const u32 TYPE_SHADOW_CUBE = 4;
const u32 WRAP_MODE_CLAMP_TO_EDGE = 0;
const u32 WRAP_MODE_CLAMP_TO_BORDER = 1;
const u32 WRAP_MODE_REPEAT = 2;
const u32 WRAP_MODE_MIRRORED_REPEAT = 3;
// the fields of the LOD register
const u32 LOD_BIAS_MASK = 0x1FFF;
const u32 LOD_MAX_LEVEL_SHIFT = 16;
const u32 LOD_MAX_LEVEL_MASK = 0xF << 16;
const u32 LOD_MIN_LEVEL_SHIFT = 24;
const u32 LOD_MIN_LEVEL_MASK = 0xF << 24;
// the fields of the dimension register
const u32 DIMENSION_HEIGHT_MASK = 0x7FF;
const u32 DIMENSION_WIDTH_MASK = 0x7FF << 16;

inline u32& Register(GlState* pState, u32 reg)
{
    return pState->m_TextureRegisters[reg - REG_FIRST];
}

inline u32 MakeBorderColor(const TextureParameters* pParameters)
{
    return static_cast<u32>(pParameters->m_BorderColor[0] * 255.0f) | (static_cast<u32>(pParameters->m_BorderColor[1] * 255.0f) << 8) |
           (static_cast<u32>(pParameters->m_BorderColor[2] * 255.0f) << 16) | (static_cast<u32>(pParameters->m_BorderColor[3] * 255.0f) << 24);
}

inline void SetDimension(u32* pRegister, const Surface* pSurface)
{
    *pRegister = (*pRegister & ~DIMENSION_WIDTH_MASK) | ((pSurface->m_Width << 16) & 0x7FFFFFF);
    *pRegister = (*pRegister & ~DIMENSION_HEIGHT_MASK) | (pSurface->m_Height & DIMENSION_HEIGHT_MASK);
}

// the bias as a fixed point number of 13 bits (1.4.8)
DECOMP_ALWAYS_INLINE u32 ConvertLodBias(f32 bias)
{
    union {
        f32 f;
        u32 u;
    } pun;
    pun.f = bias;
    if (bias == 0.0f || (pun.u << 1) >> 24 == 0xFF) {
        return 0;
    }
    f32 value = (bias + 16.0f) * 256.0f;
    if (0.0f > value) {
        value = 0.0f;
    } else if (value >= 8192.0f) {
        value = 8191.0f;
    }
    if (value >= 4096.0f) {
        return static_cast<u32>(value - 4096.0f);
    }
    return static_cast<u32>(value + 4096.0f);
}

inline void SetLodBias(u32* pRegister, f32 bias)
{
    *pRegister = (*pRegister & ~LOD_BIAS_MASK) | (ConvertLodBias(bias) & LOD_BIAS_MASK);
}

inline u32 ConvertWrap(GLenum wrap, u32 value, u32 shift, u32 mask)
{
    switch (wrap) {
    case WRAP_REPEAT:
        return (value & ~mask) | (WRAP_MODE_REPEAT << shift);
    case WRAP_CLAMP_TO_BORDER:
        return (value & ~mask) | (WRAP_MODE_CLAMP_TO_BORDER << shift);
    case WRAP_CLAMP_TO_EDGE:
        return (value & ~mask) | (WRAP_MODE_CLAMP_TO_EDGE << shift);
    case WRAP_MIRRORED_REPEAT:
        return (value & ~mask) | (WRAP_MODE_MIRRORED_REPEAT << shift);
    default:
        return value;
    }
}

// the mipmap levels of a filter that uses them
DECOMP_ALWAYS_INLINE void SetLevels(u32* pLod, s32 levelCount, GLint minLod)
{
    *pLod = (*pLod & ~LOD_MAX_LEVEL_MASK) | (((levelCount - 1) << LOD_MAX_LEVEL_SHIFT) & LOD_MAX_LEVEL_MASK);
    if (minLod < 0) {
        minLod = 0;
    }
    *pLod = (*pLod & ~LOD_MIN_LEVEL_MASK) | ((minLod << LOD_MIN_LEVEL_SHIFT) & LOD_MIN_LEVEL_MASK);
}

// the wrap modes and filters of the parameters
DECOMP_ALWAYS_INLINE void SetSampling(u32* pParameter, u32* pLod, const TextureParameters* pParameters, s32 levelCount)
{
    *pParameter = ConvertWrap(pParameters->m_WrapS, *pParameter, PARAMETER_WRAP_S_SHIFT, PARAMETER_WRAP_S_MASK);
    *pParameter = ConvertWrap(pParameters->m_WrapT, *pParameter, PARAMETER_WRAP_T_SHIFT, PARAMETER_WRAP_T_MASK);
    if (pParameters->m_MagFilter == FILTER_NEAREST) {
        *pParameter &= ~PARAMETER_MAG_LINEAR;
    } else if (pParameters->m_MagFilter == FILTER_LINEAR) {
        *pParameter |= PARAMETER_MAG_LINEAR;
    }
    switch (pParameters->m_MinFilter) {
    case FILTER_NEAREST:
        *pLod &= ~(LOD_MIN_LEVEL_MASK | LOD_MAX_LEVEL_MASK);
        *pParameter &= ~(PARAMETER_MIN_LINEAR | PARAMETER_MIPMAP_LINEAR);
        break;
    case FILTER_LINEAR:
        *pLod &= ~(LOD_MIN_LEVEL_MASK | LOD_MAX_LEVEL_MASK);
        *pParameter = (*pParameter | PARAMETER_MIN_LINEAR) & ~PARAMETER_MIPMAP_LINEAR;
        break;
    case FILTER_NEAREST_MIPMAP_NEAREST:
        *pParameter &= ~PARAMETER_MIN_LINEAR;
        SetLevels(pLod, levelCount, pParameters->m_MinLod);
        *pParameter &= ~PARAMETER_MIPMAP_LINEAR;
        break;
    case FILTER_LINEAR_MIPMAP_NEAREST:
        *pParameter |= PARAMETER_MIN_LINEAR;
        SetLevels(pLod, levelCount, pParameters->m_MinLod);
        *pParameter &= ~PARAMETER_MIPMAP_LINEAR;
        break;
    case FILTER_NEAREST_MIPMAP_LINEAR:
        *pParameter &= ~PARAMETER_MIN_LINEAR;
        SetLevels(pLod, levelCount, pParameters->m_MinLod);
        *pParameter |= PARAMETER_MIPMAP_LINEAR;
        break;
    case FILTER_LINEAR_MIPMAP_LINEAR:
        *pParameter |= PARAMETER_MIN_LINEAR;
        SetLevels(pLod, levelCount, pParameters->m_MinLod);
        *pParameter |= PARAMETER_MIPMAP_LINEAR;
        break;
    default:
        break;
    }
}

inline void SetEtc1(u32* pParameter, const Surface* pSurface)
{
    *pParameter = (*pParameter & ~PARAMETER_ETC1_MASK) | ((pSurface->m_Format == FORMAT_ETC1 ? 2 : 0) << PARAMETER_ETC1_SHIFT);
}

inline void SetAddress(u32* pRegister, u32 address)
{
    *pRegister = (*pRegister & 0xC0000000) | (nngxlowGetPhysicalAddr(address) >> 3);
}

// the formats a unit can sample
inline bool IsColorFormat(GLenum format)
{
    switch (format) {
    case FORMAT_ALPHA:
    case FORMAT_RGB:
    case FORMAT_RGBA:
    case FORMAT_LUMINANCE:
    case FORMAT_LUMINANCE_ALPHA:
    case FORMAT_HILO8:
    case FORMAT_ETC1:
    case FORMAT_ETC1A4:
        return true;
    default:
        return false;
    }
}

// units 1 and 2 (no shadow and projection textures)
DECOMP_ALWAYS_INLINE GLenum ValidateUnit(GlState* pState, u32* pRegisters, u32 firstRegister, u32 enableBit, Texture2D* pTexture, u32 borderColor)
{
    const TextureParameters* pParameters = &pTexture->m_Parameters;
    const Surface* pSurface = &pTexture->m_Surface;
    SetLodBias(&pRegisters[UNIT_LOD], pParameters->m_LodBias);
    pRegisters[UNIT_PARAMETER] &= ~PARAMETER_UNKNOWN_6;
    GLenum format = pSurface->m_Format;
    if (format == FORMAT_UNKNOWN_0x6050) {
        pRegisters[UNIT_PARAMETER] &= ~(PARAMETER_MAG_LINEAR | PARAMETER_MIN_LINEAR | PARAMETER_WRAP_S_MASK | PARAMETER_WRAP_T_MASK);
        pRegisters[UNIT_LOD] &= ~(LOD_MAX_LEVEL_MASK | LOD_MIN_LEVEL_MASK | LOD_BIAS_MASK);
        pRegisters[0] = 0;
    } else if (IsColorFormat(format)) {
        SetSampling(&pRegisters[UNIT_PARAMETER], &pRegisters[UNIT_LOD], pParameters, pSurface->m_LevelCount);
        pRegisters[0] = borderColor;
    } else {
        return ERROR_INVALID_OPERATION;
    }
    SetEtc1(&pRegisters[UNIT_PARAMETER], pSurface);
    SetDimension(&pRegisters[UNIT_DIMENSION], pSurface);
    SetAddress(&pRegisters[UNIT_ADDRESS], pSurface->m_Address);
    Register(pState, REG_CONFIG) |= enableBit;
    pRegisters[UNIT_TYPE] = (pRegisters[UNIT_TYPE] & ~0xF) | (pSurface->m_HwFormat & 0xF);
    __cb_writeRegs(firstRegister, UNIT_REGISTER_COUNT, pRegisters);
    return 0;
}

// the registers of the 2D texture of a unit (an error if it cannot be sampled)
// 0x007C4C54 (name is ours)
GLenum ValidateTexture2D(Texture2D* pTexture, s32 unit)
{
    GlState* pState = s_pGlState;
    GLenum error = pTexture->m_Surface.m_Address == 0 ? ERROR_INVALID_OPERATION : 0;
    if (!pState->m_IsTexture2dEnabled[unit] || error != 0) {
        // the unit is off
        switch (unit) {
        case 0:
            Register(pState, REG_CONFIG) &= ~1;
            break;
        case 1:
            Register(pState, REG_CONFIG) &= ~2;
            break;
        case 2:
            Register(pState, REG_CONFIG) &= ~4;
            break;
        default:
            return ERROR_INVALID_VALUE;
        }
        return pState->m_IsTexture2dEnabled[unit] ? error : 0;
    }
    const TextureParameters* pParameters = &pTexture->m_Parameters;
    const Surface* pSurface = &pTexture->m_Surface;
    u32 borderColor = MakeBorderColor(pParameters);
    switch (unit) {
    case 0: {
        u32* pParameter = &Register(pState, REG_UNIT0_PARAMETER);
        u32* pLod = &Register(pState, REG_UNIT0_LOD);
        Register(pState, REG_UNIT0_BORDER) = borderColor;
        *pParameter &= ~PARAMETER_UNKNOWN_20;
        SetDimension(&Register(pState, REG_UNIT0_DIMENSION), pSurface);
        SetLodBias(pLod, pParameters->m_LodBias);
        Register(pState, REG_UNIT0_TYPE) = (Register(pState, REG_UNIT0_TYPE) & ~0xF) | (pSurface->m_HwFormat & 0xF);
        GLenum format = pSurface->m_Format;
        if (format == FORMAT_SHADOW) {
            *pLod &= ~(LOD_MIN_LEVEL_MASK | LOD_MAX_LEVEL_MASK);
            *pParameter = (*pParameter & ~(PARAMETER_TYPE_MASK | PARAMETER_MIPMAP_LINEAR)) | (TYPE_SHADOW_2D << PARAMETER_TYPE_SHIFT) |
                          PARAMETER_MAG_LINEAR | PARAMETER_MIN_LINEAR | PARAMETER_UNKNOWN_20;
            *pLod &= ~LOD_BIAS_MASK;
            *pParameter = (*pParameter & ~(PARAMETER_WRAP_S_MASK | PARAMETER_WRAP_T_MASK | PARAMETER_ETC1_MASK)) |
                          (WRAP_MODE_CLAMP_TO_BORDER << PARAMETER_WRAP_S_SHIFT) | (WRAP_MODE_CLAMP_TO_BORDER << PARAMETER_WRAP_T_SHIFT);
        } else if (format == FORMAT_UNKNOWN_0x6050) {
            *pParameter &= ~(PARAMETER_ETC1_MASK | PARAMETER_TYPE_MASK | PARAMETER_MAG_LINEAR | PARAMETER_MIN_LINEAR | PARAMETER_WRAP_S_MASK |
                             PARAMETER_WRAP_T_MASK);
            *pLod &= ~(LOD_MIN_LEVEL_MASK | LOD_MAX_LEVEL_MASK | LOD_BIAS_MASK);
            Register(pState, REG_UNIT0_BORDER) = 0;
        } else if (IsColorFormat(format)) {
            if (pState->m_SamplerTypes[0] == SAMPLER_TYPE_PROJECTION) {
                *pParameter = (*pParameter & ~PARAMETER_TYPE_MASK) | (TYPE_PROJECTION << PARAMETER_TYPE_SHIFT);
            } else {
                *pParameter &= ~PARAMETER_TYPE_MASK;
            }
            SetSampling(pParameter, pLod, pParameters, pSurface->m_LevelCount);
            SetEtc1(pParameter, pSurface);
        } else {
            return ERROR_INVALID_OPERATION;
        }
        SetAddress(&Register(pState, REG_UNIT0_ADDRESS), pSurface->m_Address);
        Register(pState, REG_CONFIG) |= 1;
        __cb_writeRegs(REG_UNIT0_BORDER, UNIT0_REGISTER_COUNT, &Register(pState, REG_UNIT0_BORDER));
        PutCommand(Register(pState, REG_UNIT0_TYPE), 0xF0000 | REG_UNIT0_TYPE);
        break;
    }
    case 1:
        return ValidateUnit(pState, &Register(pState, REG_UNIT1_BORDER), REG_UNIT1_BORDER, 2, pTexture, borderColor);
    case 2:
        return ValidateUnit(pState, &Register(pState, REG_UNIT2_BORDER), REG_UNIT2_BORDER, 4, pTexture, borderColor);
    default:
        break;
    }
    return 0;
}

// the registers of the cube map of unit 0 (its faces must match the first one and lie in the same
// 32 MB as it)
// 0x007C5BFC (name is ours)
GLenum ValidateTextureCube(TextureCube* pTexture, s32 unit)
{
    GlState* pState = s_pGlState;
    if (unit != 0) {
        return ERROR_INVALID_OPERATION;
    }
    const Surface* pFirst = &pTexture->m_Faces[0];
    s32 face;
    for (face = 1; face < static_cast<s32>(CUBE_FACE_COUNT); face++) {
        const Surface* p = &pTexture->m_Faces[face];
        if (p->m_Address == 0 || pFirst->m_LevelCount != p->m_LevelCount || pFirst->m_Size != p->m_Size || pFirst->m_Width != p->m_Width ||
            pFirst->m_Height != p->m_Height || pFirst->m_Format != p->m_Format ||
            static_cast<s32>(p->m_Size + p->m_Address - 1) >> 25 != static_cast<s32>(pFirst->m_Address) >> 25 ||
            pFirst->m_Address > p->m_Address) {
            break;
        }
    }
    GLenum error = face != static_cast<s32>(CUBE_FACE_COUNT) ? ERROR_INVALID_OPERATION : 0;
    if (!pState->m_IsTextureCubeEnabled[unit] || error != 0) {
        Register(pState, REG_CONFIG) &= ~1;
        return error;
    }
    const TextureParameters* pParameters = &pTexture->m_Parameters;
    u32* pParameter = &Register(pState, REG_UNIT0_PARAMETER);
    u32* pLod = &Register(pState, REG_UNIT0_LOD);
    *pParameter &= ~(PARAMETER_UNKNOWN_20 | PARAMETER_MIPMAP_LINEAR);
    Register(pState, REG_UNIT0_BORDER) = MakeBorderColor(pParameters);
    SetDimension(&Register(pState, REG_UNIT0_DIMENSION), pFirst);
    Register(pState, REG_UNIT0_SHADOW) &= 0xFF000000;
    Register(pState, REG_UNIT0_TYPE) = (Register(pState, REG_UNIT0_TYPE) & ~0xF) | (pFirst->m_HwFormat & 0xF);
    GLenum format = pFirst->m_Format;
    if (format == FORMAT_SHADOW) {
        *pParameter = (*pParameter & ~(PARAMETER_TYPE_MASK | PARAMETER_WRAP_S_MASK | PARAMETER_WRAP_T_MASK | PARAMETER_ETC1_MASK)) |
                      (TYPE_SHADOW_CUBE << PARAMETER_TYPE_SHIFT) | PARAMETER_MAG_LINEAR | PARAMETER_MIN_LINEAR | PARAMETER_UNKNOWN_20;
        *pLod &= ~(LOD_MIN_LEVEL_MASK | LOD_MAX_LEVEL_MASK | LOD_BIAS_MASK);
    } else if (IsColorFormat(format) && format != FORMAT_HILO8) {
        *pParameter = (*pParameter & ~PARAMETER_TYPE_MASK) | (TYPE_CUBE << PARAMETER_TYPE_SHIFT);
        SetSampling(pParameter, pLod, pParameters, pFirst->m_LevelCount);
        *pLod &= ~LOD_BIAS_MASK;
        SetEtc1(pParameter, pFirst);
    } else {
        return ERROR_INVALID_OPERATION;
    }
    SetAddress(&Register(pState, REG_UNIT0_ADDRESS), pFirst->m_Address);
    for (u32 i = 1; i < CUBE_FACE_COUNT; i++) {
        u32* pAddress = &Register(pState, REG_UNIT0_ADDRESS + i);
        *pAddress = (*pAddress & 0xFFC00000) | ((nngxlowGetPhysicalAddr(pTexture->m_Faces[i].m_Address) >> 3) & 0x3FFFFF);
    }
    Register(pState, REG_CONFIG) |= 1;
    __cb_writeRegs(REG_UNIT0_BORDER, UNIT0_REGISTER_COUNT, &Register(pState, REG_UNIT0_BORDER));
    PutCommand(Register(pState, REG_UNIT0_TYPE), 0xF0000 | REG_UNIT0_TYPE);
    return 0;
}
} // namespace

extern "C" {
// 0x0012E4BC | nintendogs:bytes [tier B]
void __txv_initializeTextureValidator(u32* pDirtyMask)
{
    *pDirtyMask = DIRTY_TEXTURE;
    GlState* pState = s_pGlState;
    Register(pState, REG_CONFIG) = (Register(pState, REG_CONFIG) & ~0x2000) | 0x11000;
    __txv_initializeTextureValidatorHWRegister();
}

// the texture registers at the start (0x80: the units off, 0x8C: 0xFF0000, 0x8F stays)
// 0x00135618 | nintendogs:bytes [tier B]
void __txv_initializeTextureValidatorHWRegister()
{
    for (s32 i = 0; i <= 30; i++) {
        switch (i) {
        case 0:
            PutCommand(0x11000, 0xD0000 | REG_CONFIG);
            break;
        case 12:
            PutCommand(0xFF0000, 0xF0000 | (REG_FIRST + 12));
            break;
        case 15:
            break;
        default:
            PutCommand(0, (REG_FIRST + i) | 0xF0000);
            break;
        }
    }
}

// the units whose textures changed (dirty bits 10-12)
// 0x007A0EB0 | nintendogs:bytes [tier B]
void __txv_validateTextureValidator(const u32* pDirtyFlags)
{
    bool isChanged = false;
    GlState* pState = s_pGlState;
    if ((pState->m_IgnoredStates & STATE_TEXTURE) != 0) {
        return;
    }
    for (s32 unit = 0; unit < static_cast<s32>(TEXTURE_UNIT_COUNT); unit++) {
        s32 bit = unit + 10;
        if ((pDirtyFlags[bit >> 5] & (1 << (bit & 31))) == 0) {
            continue;
        }
        TextureTables* pTables = s_TextureManager.m_pTables;
        if (pState->m_IsTextureCubeEnabled[unit]) {
            if (pState->m_TextureCubeNames[unit] != 0) {
                if (pTables->m_pBoundCube[unit] != nullptr) {
                    ValidateTextureCube(static_cast<TextureCube*>(pTables->m_pBoundCube[unit]->m_pData), unit);
                }
            } else {
                ValidateTextureCube(pTables->m_pDefaultCube, unit);
            }
        } else if (pState->m_Texture2dNames[unit] != 0) {
            if (pTables->m_pBound2d[unit] != nullptr) {
                ValidateTexture2D(static_cast<Texture2D*>(pTables->m_pBound2d[unit]->m_pData), unit);
            }
        } else {
            ValidateTexture2D(pTables->m_pDefault2d, unit);
        }
        isChanged = true;
    }
    if (isChanged) {
        u32 config = Register(pState, REG_CONFIG);
        if ((config & 7) != 0) {
            // the cache of the texture units is cleared after a change
            PutCommand(config, 0x10000 | REG_CONFIG);
            PutCommand(0x10000, 0x50000 | REG_CONFIG);
        }
    }
}
} // extern "C"
