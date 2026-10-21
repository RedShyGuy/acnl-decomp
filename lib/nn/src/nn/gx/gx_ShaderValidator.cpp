#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gx/detail/gx_Shader.h"
#include <cmath>
#include <cstring>

// The shader validator of the GL state (__shv_*): the registers of the shader, lighting, fog,
// texture combiner and fragment operation state (kept in the shader tables and as the last values of
// some registers) become commands. The registers after 3dbrew; all names are ours except those of
// the __shv_ functions.

using namespace nn::gx::detail;

namespace nn {
namespace gx {
namespace detail {
// 0x0097E7A8 (name is ours)
u32 s_BlendFunctionRegister;
// 0x0097E7AC (name is ours)
u32 s_DepthColorMaskRegister;
// 0x0097E7B0 (name is ours)
u32 s_StencilTestRegister;
// 0x0097E7B4 (name is ours)
u32 s_StencilOperationRegister;
// 0x0097E7B8 (name is ours)
u32 s_LogicOperationRegister;
// 0x0097E7BC (name is ours)
u32 s_BlendColorRegister;
// 0x0097E7C0 (name is ours)
u32 s_EarlyDepthFunctionRegister;
// 0x0097E7C4 (name is ours)
u32 s_EarlyDepthDataRegister;
// 0x0097E7C8 (name is ours)
u32 s_GasDeltaZDepthRegister;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the dirty flags of the parts of the state the shader validator handles
const u32 SHADER_DIRTY_FLAG_COUNT = 19;
// 0x008B32B0 (name is ours)
const u32 s_ShaderDirtyFlags[SHADER_DIRTY_FLAG_COUNT] = {0x40000, 0x4,      0x40,     0x10000,   0x80,   0x100,  0x200,
                                                         0x80000, 0x100000, 0x200000, 0x400000,  0x800000, 0x1000000, 0x8000,
                                                         0x4000,  0x8,      0x20,     0x10,      0x2000000};
// the zero values for the shader code and its uniforms at the start (words)
const u32 ZERO_BUFFER_SIZE = 0x4000;
} // namespace

extern "C" {
// 0x0012E3E0 | nintendogs:bytes [tier B]
void __shv_initializeShaderValidator(u32* pDirtyMask)
{
    *pDirtyMask = s_ShaderDirtyFlags[0];
    for (u32 i = 1; i < SHADER_DIRTY_FLAG_COUNT; i++) {
        *pDirtyMask |= s_ShaderDirtyFlags[i];
    }
    s_BlendFunctionRegister = 0x1010000;
    s_DepthColorMaskRegister = 0x1F40;
    s_StencilTestRegister = 0xFF00FF10;
    s_EarlyDepthFunctionRegister = 3;
    s_EarlyDepthDataRegister = 0xFFFFFF;
    s_StencilOperationRegister = 0;
    s_LogicOperationRegister = 3;
    s_BlendColorRegister = 0;
    s_GasDeltaZDepthRegister = 0x3000000;
    __shv_initializeShaderValidatorHWRegister();
}

// the registers at the start: the shader units, the fragment operations, the light and fog lookup
// tables, the shader code (zeros), then the registers the shader tables keep
// 0x00134D3C | nintendogs:bytes [tier B]
void __shv_initializeShaderValidatorHWRegister()
{
    PutCommand(1, 0x10245);
    PutCommand(0, 0x10244);
    PutCommand(0x80000000, 0x80289);
    PutCommand(0, 0xB0229);
    PutCommand(0, 0xF0252);
    PutCommand(0, 0xF0251);
    PutCommand(0, 0xF0254);
    PutCommand(0, 0x10253);
    PutCommand(0, 0xF0242);
    PutCommand(0, 0xF024A);
    PutCommand(0, 0x5025E);
    __cb_fillRegs(0x101, 7, 0);
    PutCommand(0x10140, 0xF011F);
    PutCommand(0xE40100, 0xF0100);
    PutCommand(0x1010000, 0xF0101);
    PutCommand(0x1F40, 0xF0107);
    PutCommand(0xFF00FF10, 0xF0105);
    PutCommand(3, 0x10061);
    PutCommand(0, 0x10062);
    PutCommand(0, 0xF0065);
    PutCommand(0, 0xF0066);
    PutCommand(0, 0xF0067);
    PutCommand(0, 0x10118);
    PutCommand(0, 0xF011B);
    PutCommand(0xFFFFFF, 0x7006A);
    PutCommand(3, 0xF0102);
    PutCommand(0x3000000, 0x80126);
    __cb_fillRegs(0x40, 16, 0);
    __cb_fillRegs(0x50, 7, 0x1F1F1F1F);
    PutCommand(0x100, 0xF0058);
    PutCommand(1, 0xF004C);
    PutCommand(0, 0xF006F);
    PutCommand(0, 0x20060);
    PutCommand(0x20000, 0xC0069);
    PutCommand(15, 0xF0113);
    PutCommand(15, 0xF0112);
    PutCommand(3, 0xF0114);
    PutCommand(3, 0xF0115);
    // the light lookup tables
    for (u32 i = 0; i < 7; i++) {
        PutCommand(i << 8, 0xF01C5);
        for (s32 j = 0; j < 256; j += 8) {
            __cb_fillRegs(0x1C8, 8, 0);
        }
    }
    PutCommand(0x80000000, 0xF0290);
    for (s32 j = 0; j < 96; j += 2) {
        __cb_fillRegs(0x291, 8, 0);
    }
    // the shader code
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    u32* pZeros = pAllocate != nullptr ? static_cast<u32*>(pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, ZERO_BUFFER_SIZE)) : nullptr;
    memset(pZeros, 0, ZERO_BUFFER_SIZE);
    PutCommand(0, 0xF02CB);
    __cb_multiWriteReg(0x2CC, 0x200, pZeros);
    PutCommand(0x200, 0xF029B);
    __cb_multiWriteReg(0x29C, 0xE00, pZeros);
    PutCommand(0, 0xF02BF);
    if (s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pZeros);
    }
    for (u32 i = 0; i < 4; i++) {
        PutCommand(0, (0x2B1 + i) | 0xF0000);
    }
    __cb_fillRegs(0x28B, 2, 0xFFFFFFFF);
    __cb_fillRegs(0x205, 36, 0);
    PutCommand(1, 0xF01D4);
    PutCommand(0xFEFCF8F0, 0xF00AE);
    // the registers of the shader tables
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    for (u32 i = 0; i < SHADER_REGISTER_COUNT; i++) {
        if (pTables->m_RegisterMasks[i] == 0) {
            continue;
        }
        u32* pCurrent = s_pCommandBufferCurrent;
        if (pCurrent < s_pCommandBufferEnd) {
            *pCurrent++ = pTables->m_RegisterValues[i];
            s_pCommandBufferCurrent = pCurrent;
            *pCurrent++ = s_ShaderRegisters[i] | (pTables->m_RegisterMasks[i] << 16);
            s_pCommandBufferCurrent = pCurrent;
        }
    }
}
} // extern "C"

namespace {
// the dirty flags the shader validation reads (names are ours)
const u32 DIRTY_DEPTH = 0x4;                       // DIRTY_10
const u32 DIRTY_LIGHTING_LUTS = 0x8;
const u32 DIRTY_FOG_LUT = 0x10;
const u32 DIRTY_PROCEDURAL_LUTS = 0x20;
const u32 DIRTY_FRAGMENT_MODE = 0x100;             // DIRTY_11
const u32 DIRTY_SCISSOR = 0x200;                   // DIRTY_12
const u32 DIRTY_LUT_DATA = 0x4000;
const u32 DIRTY_ATTRIBUTE_ARRAYS = 0x8042;
const u32 DIRTY_ATTRIBUTE_VALUES = 0x80C2;
const u32 DIRTY_RESERVED_REGISTERS = 0x80000;
const u32 DIRTY_SHADER_CODE = 0x100000;            // DIRTY_0
const u32 DIRTY_VERTEX_FLOAT_CONSTANTS = 0x200000;
const u32 DIRTY_GEOMETRY_FLOAT_CONSTANTS = 0x400000;
const u32 DIRTY_GEOMETRY_INTEGERS = 0x800000;
const u32 DIRTY_VERTEX_INTEGERS = 0x1000000;
const u32 DIRTY_GAS_LUT = 0x2000000;

// the state bits of the parts of the shader validation (see STATE_0 ...)
const GLbitfield STATE_SHADER_CODE = STATE_0;
const GLbitfield STATE_SHADER_REGISTERS = STATE_1;
const GLbitfield STATE_SHADER_INTEGERS = STATE_2;
const GLbitfield STATE_FLOAT_CONSTANTS = STATE_3;
const GLbitfield STATE_FLOAT_UNIFORMS = STATE_4;
const GLbitfield STATE_FRAGMENT_REGISTERS = STATE_5;
const GLbitfield STATE_LUTS = STATE_6;
const GLbitfield STATE_DEPTH = STATE_10;
const GLbitfield STATE_FRAGMENT_MODE = STATE_11;
const GLbitfield STATE_SCISSOR = STATE_12;

// the registers of the program (see s_ShaderRegisters)
const u32 REGISTER_GEOMETRY_BOOL = 1;
const u32 REGISTER_DEPTH_SCALE = 23;
const u32 REGISTER_DEPTH_OFFSET = 24;
const u32 REGISTER_LIGHTING_ENABLE = 43;
const u32 REGISTER_PT_CONTROL = 44;
const u32 REGISTER_GAS_FOG_MODE = 80;
const u32 REGISTER_GAS_ACC_MAX = 83;
const u32 REGISTER_LIGHT0_SPECULAR0 = 92;
const u32 REGISTER_LIGHTING_CONFIG0 = 182;
const u32 REGISTER_LIGHTING_CONFIG1 = 183;

// the values of the reserved uniforms in the program (see gx_Uniform.cpp)
const u32 RESERVED_MATERIAL_LUTS = 0x9F4;
const u32 RESERVED_LIGHTING_CONFIG = 0xA0C;
const u32 RESERVED_LIGHTS = 0xA20;
const u32 RESERVED_PT_LUTS = 0xDF0;
const u32 RESERVED_PT_SAMPLER_TYPE = 0xE0C;
const u32 RESERVED_W_SCALE = 0xE4C;
const u32 RESERVED_FOG_LUT = 0xE64;
const u32 RESERVED_GAS_LUTS = 0xE78;
const u32 LIGHT_SIZE = 0x70;
const u32 LIGHT_LUT_SP = 0x60;
const u32 LIGHT_LUT_DA = 0x6C;
const u32 LIGHT_COUNT = 8;

// the slots of the lookup tables in the GL state (m_UsedLutNames)
const u32 LUT_SLOT_MATERIAL = 0;
const u32 LUT_SLOT_SP = 6;
const u32 LUT_SLOT_DA = 14;
const u32 LUT_SLOT_PT = 22;
const u32 LUT_SLOT_PT_NOISE = 24;
const u32 LUT_SLOT_PT_COLOR = 25;
const u32 LUT_SLOT_FOG = 29;
const u32 LUT_SLOT_GAS = 30;
// the buffers of a lookup table in the formats of the GPU (TextureLut::m_pBuffers)
const u32 LUT_BUFFER_LIGHTING = 0;
const u32 LUT_BUFFER_FOG = 1;
const u32 LUT_BUFFER_GAS = 2;
const u32 LUT_BUFFER_PT_MAP = 3;
const u32 LUT_BUFFER_PT_NOISE = 4;
const u32 LUT_BUFFER_PT_COLOR = 5;

// the fragment modes of the programs (dmp_FragOperation.mode)
const GLenum FRAGMENT_MODE_DEFAULT = 0x6030;
const GLenum FRAGMENT_MODE_SHADOW = 0x6048;
const GLenum FRAGMENT_MODE_GAS = 0x6051;

// the vertex loaders of the GPU
const u32 LOADER_REGISTER_COUNT = 39;
const u32 MAX_LOADER_COMPONENTS = 12;
const u32 MAX_VERTEX_INPUTS = 12;
// the types of the vertex attributes
const GLenum TYPE_BYTE = 0x1400;
const GLenum TYPE_SHORT = 0x1402;
const GLenum TYPE_FLOAT = 0x1406;

DECOMP_ALWAYS_INLINE u32 BitsOf(f32 value)
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

DECOMP_ALWAYS_INLINE u32* ReservedWord(Program* pProgram, u32 offset)
{
    return reinterpret_cast<u32*>(reinterpret_cast<u8*>(pProgram) + offset);
}

DECOMP_ALWAYS_INLINE bool IsLightEnabled(Program* pProgram, u32 light)
{
    return *(reinterpret_cast<u8*>(pProgram) + RESERVED_LIGHTS + light * LIGHT_SIZE) != 0;
}

DECOMP_ALWAYS_INLINE void* AllocateLutBuffer(u32 size)
{
    return s_Allocator.m_pAllocate != nullptr ? s_Allocator.m_pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, size) : nullptr;
}

// an unsigned fixed point value of the lookup tables (0 for values that are not positive)
DECOMP_ALWAYS_INLINE u32 ToLutValue(f32 value, f32 scale, u32 max)
{
    if (value <= 0.0f || IsInfiniteOrNan(value)) {
        return 0;
    }
    f32 scaled = value * scale;
    return scaled < scale ? static_cast<u32>(scaled) : max;
}

// a signed fixed point value in two's complement, see FloatToSignedFixed in gx_Uniform.cpp
DECOMP_ALWAYS_INLINE u32 ToSignedLutValue(f32 value, f32 range, f32 scale, f32 max, f32 half)
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

// a difference of the lighting tables: sign and magnitude of 11 bits
DECOMP_ALWAYS_INLINE u32 ToLightingDifference(f32 value)
{
    if (value == 0.0f || IsInfiniteOrNan(value)) {
        return 0;
    }
    f32 scaled = value * 2048.0f;
    u32 sign;
    if (scaled < 0.0f) {
        scaled = -scaled;
        sign = 0x800;
    } else {
        sign = 0;
    }
    if (scaled >= 2048.0f) {
        scaled = 2047.0f;
    }
    return sign | static_cast<u32>(scaled);
}

// the values of a table (256 floats) and their differences (256 floats)
DECOMP_ALWAYS_INLINE const f32* GetLutValues(TextureLut* pLut)
{
    return reinterpret_cast<const f32*>(pLut->m_Data);
}

// the scissor box, clamped to the framebuffer
DECOMP_ALWAYS_INLINE void PutScissor(GlState* pState)
{
    u32 mode;
    s32 left;
    s32 bottom;
    s32 right;
    s32 top;
    if (pState->m_IsScissorTestEnabled) {
        GLint x = pState->m_ScissorBox[0];
        GLint y = pState->m_ScissorBox[1];
        right = x - 1 + pState->m_ScissorBox[2];
        top = y - 1 + pState->m_ScissorBox[3];
        if (pState->m_FramebufferWidth <= x) {
            left = pState->m_FramebufferWidth - 1;
        } else {
            left = x < 0 ? 0 : x;
        }
        if (pState->m_FramebufferHeight <= y) {
            bottom = pState->m_FramebufferHeight - 1;
        } else {
            bottom = y < 0 ? 0 : y;
        }
        if (pState->m_FramebufferWidth < right) {
            right = pState->m_FramebufferWidth - 1;
        } else if (right < 0) {
            right = 0;
        }
        if (pState->m_FramebufferHeight < top) {
            top = pState->m_FramebufferHeight - 1;
        } else if (top < 0) {
            top = 0;
        }
        mode = 3;
    } else {
        left = 0;
        bottom = 0;
        right = pState->m_FramebufferWidth - 1;
        top = pState->m_FramebufferHeight - 1;
        mode = 0;
    }
    PutCommand(mode, 0xF0065);
    PutCommand(left | (bottom << 16), 0xF0066);
    PutCommand(right | (top << 16), 0xF0067);
}

// whether the GPU runs a geometry shader (after an update of the state always)
DECOMP_ALWAYS_INLINE void PutGeometryShaderState(GlState* pState, Program* pProgram)
{
    s32 hasGeometryShader = pProgram->m_HasGeometryShader ? 1 : 0;
    if (hasGeometryShader != s_ShaderManager.m_LastGeometryShaderState || (pState->m_UpdatedStates & STATE_SHADER_INTEGERS) != 0) {
        __cb_addDummyWrite(0x251, 10);
        __cb_addDummyWrite(0x200, 30);
        PutCommand(hasGeometryShader != 0 ? 2 : 0, 0x10229);
        __cb_addDummyWrite(0x200, 30);
        s_ShaderManager.m_LastGeometryShaderState = hasGeometryShader;
        PutCommand(hasGeometryShader != 0 ? 1 : 0, 0x10244);
    }
}

// the code and operand descriptors (with a geometry shader both units get the binary; else the
// vertex shader code past 512 words goes to the geometry unit, which runs it then)
DECOMP_ALWAYS_INLINE void PutShaderCode(Program* pProgram)
{
    ShaderBinary* pBinary = pProgram->m_pLinkedBinary;
    if (pProgram->m_HasGeometryShader) {
        PutCommand(0, 0xF029B);
        __cb_multiWriteReg(0x29C, pBinary->m_CodeSize, pBinary->m_pCode);
        PutCommand(1, 0xF028F);
        PutCommand(0, 0xF02A5);
        __cb_multiWriteReg(0x2A6, pBinary->m_OperandDescriptorCount, pBinary->m_pOperandDescriptors);
    } else if (pBinary->m_CodeSize > 512) {
        PutCommand(512, 0xF029B);
        __cb_multiWriteReg(0x29C, pBinary->m_CodeSize - 512, pBinary->m_pCode + 512);
        PutCommand(1, 0xF028F);
    }
    PutCommand(0, 0xF02CB);
    __cb_multiWriteReg(0x2CC, pProgram->m_pLinkedBinary->m_CodeSize > 512 ? 512 : pProgram->m_pLinkedBinary->m_CodeSize,
                       pProgram->m_pLinkedBinary->m_pCode);
    PutCommand(1, 0xF02BF);
    PutCommand(0, 0xF02D5);
    __cb_multiWriteReg(0x2D6, pProgram->m_pLinkedBinary->m_OperandDescriptorCount, pProgram->m_pLinkedBinary->m_pOperandDescriptors);
}

// registers of the program are sent again (the last values sent become different)
DECOMP_ALWAYS_INLINE void InvalidateRegisters(Program* pProgram, u32 first, u32 last)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    for (u32 i = first; i <= last; i++) {
        pTables->m_SentRegisterValues[i] = ~pProgram->m_RegisterValues[i];
    }
}

// the registers of a list that the program sets are sent again
DECOMP_ALWAYS_INLINE void InvalidateRegisterList(Program* pProgram, const s32* pList)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    for (u32 i = 0; pList[i] != static_cast<s32>(SHADER_REGISTER_COUNT); i++) {
        u32 index = pList[i];
        if (pProgram->m_RegisterMasks[index] != 0) {
            pTables->m_SentRegisterValues[index] = ~pProgram->m_RegisterValues[index];
            pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
        }
    }
}

// the float constants of the executables (register and three words of float24 each)
DECOMP_ALWAYS_INLINE void PutFloatConstants(Program* pProgram, u32 dirtyFlags)
{
    if (pProgram->m_HasGeometryShader && (dirtyFlags & DIRTY_GEOMETRY_FLOAT_CONSTANTS) != 0) {
        ShaderBinaryEntry* pEntry = &pProgram->m_pLinkedBinary->m_pEntries[pProgram->m_GeometryEntryIndex];
        for (u32 i = 0; i < static_cast<u32>(pEntry->m_FloatConstantCount); i++) {
            __cb_writeRegs(0x290, 4, pEntry->m_pFloatConstants + i * 4);
        }
    }
    if ((dirtyFlags & DIRTY_VERTEX_FLOAT_CONSTANTS) != 0) {
        ShaderBinaryEntry* pEntry = &pProgram->m_pLinkedBinary->m_pEntries[pProgram->m_VertexEntryIndex];
        for (u32 i = 0; i < static_cast<u32>(pEntry->m_FloatConstantCount); i++) {
            __cb_writeRegs(0x2C0, 4, pEntry->m_pFloatConstants + i * 4);
        }
    }
}

// the type of the format of a vertex attribute (byte, unsigned byte, short, float)
DECOMP_ALWAYS_INLINE u32 GetAttributeType(GLenum type)
{
    switch (type) {
    case TYPE_UNSIGNED_BYTE:
        return 1;
    case TYPE_SHORT:
        return 2;
    case TYPE_FLOAT:
        return 3;
    default:
        return 0;
    }
}

// a component of a vertex loader (an attribute or padding of 1-4 words as 12-15)
DECOMP_ALWAYS_INLINE void PutLoaderComponent(u32* pLoader, u32 slot, u32 component)
{
    if (slot < 8) {
        pLoader[1] |= component << (slot * 4);
    } else {
        pLoader[2] |= component << (slot * 4 - 32);
    }
}

DECOMP_ALWAYS_INLINE void PutInputPermutation(Program* pProgram, u32 input, u32 attribute)
{
    u32 reg = pProgram->m_Bindings[attribute].m_Register & 15;
    if (input < 8) {
        pProgram->m_VertexInputPermutation[0] |= reg << (input * 4);
    } else {
        pProgram->m_VertexInputPermutation[1] |= reg << (input * 4 - 32);
    }
}

// the vertex attributes of the program: the arrays (sorted by address) go through the vertex loaders
// of the GPU when they share a base address and fit; the setup of the last validation is kept and
// only moved when the arrays moved together
DECOMP_ALWAYS_INLINE void SetUpAttributes(GlState* pState, Program* pProgram)
{
    AttributeNode nodes[VERTEX_ATTRIBUTE_COUNT];
    u32 constants[VERTEX_ATTRIBUTE_COUNT];
    u32* pRegisters = pProgram->m_AttributeRegisters;
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    bool isSameProgram = pTables->m_pAttributeProgram == pProgram;
    pTables->m_pAttributeProgram = pProgram;
    pState->m_IsVertexBufferUsable = true;

    // the list of the arrays
    u32 maxAddress = 0;
    s32 arrayCount = 0;
    s32 constantCount = 0;
    AttributeNode* pFirst = nodes;
    nodes[0].m_pNext = nullptr;
    for (s32 i = VERTEX_ATTRIBUTE_COUNT - 1; i >= 0; i--) {
        if (pProgram->m_Bindings[i].m_Register == -1) {
            continue;
        }
        VertexAttribute* pAttribute = &pState->m_Attributes[i];
        if (!pAttribute->m_IsEnabled) {
            constants[constantCount++] = i;
            continue;
        }
        if (pAttribute->m_BufferName == 0) {
            pState->m_IsVertexBufferUsable = false;
        } else if (pState->m_ArrayAddresses[1 + i] > maxAddress) {
            maxAddress = pState->m_ArrayAddresses[1 + i];
        }
        AttributeNode* pNode = &nodes[arrayCount];
        pNode->m_Index = i;
        if (arrayCount != 0) {
            s32 address = pState->m_ArrayAddresses[1 + i];
            if (address <= static_cast<s32>(pState->m_ArrayAddresses[1 + pFirst->m_Index])) {
                pNode->m_pNext = pFirst;
                pFirst = pNode;
            } else {
                AttributeNode* pPrevious = pFirst;
                AttributeNode* p = pFirst->m_pNext;
                while (p != nullptr && address > static_cast<s32>(pState->m_ArrayAddresses[1 + p->m_Index])) {
                    pPrevious = p;
                    p = p->m_pNext;
                }
                pPrevious->m_pNext = pNode;
                pNode->m_pNext = p;
            }
        }
        arrayCount++;
    }
    if (pState->m_IsArrayMissing || arrayCount + constantCount > static_cast<s32>(MAX_VERTEX_INPUTS)) {
        pState->m_IsVertexBufferUsable = false;
    }
    if (arrayCount == 0) {
        pState->m_IsVertexBufferUsable = false;
    } else {
        u32 minAddress = pState->m_ArrayAddresses[1 + pFirst->m_Index] & ~0xF;
        pState->m_ArrayBaseAddress = minAddress;
        if (pState->m_IsVertexBufferUsable) {
            if (!pState->m_IsElementArrayUnused) {
                u32 element = pState->m_ArrayAddresses[0];
                if (element < minAddress) {
                    minAddress = element;
                }
                if (element > maxAddress) {
                    maxAddress = element;
                }
            }
            if (maxAddress - minAddress >= 0x10000000) {
                pState->m_IsVertexBufferUsable = false;
            }
        }
    }

    // the setup of the last validation
    bool isCached = true;
    if (pProgram->m_AttributeArrayCount == arrayCount && pProgram->m_ConstantAttributeCount == constantCount &&
        (pState->m_UpdatedStates & STATE_VERTEX) == 0) {
        AttributeNode* pNode = pFirst;
        AttributeNode* pCached = pProgram->m_pFirstAttribute;
        for (s32 j = 0; j < arrayCount; j++) {
            u32 index = pNode->m_Index;
            VertexAttribute* pAttribute = &pState->m_Attributes[index];
            u32 format = ((pAttribute->m_Size << 2) - 4) | GetAttributeType(pAttribute->m_Type);
            u32 cachedFormat = j < 8 ? pRegisters[1] >> (j * 4) : pRegisters[2] >> (j * 4 - 32);
            if (index != pCached->m_Index ||
                pState->m_ArrayAddresses[1 + index] - pState->m_ArrayAddresses[1 + pFirst->m_Index] !=
                    pProgram->m_AttributeAddresses[j] - pProgram->m_AttributeAddresses[0] ||
                pAttribute->m_Stride != pProgram->m_AttributeStrides[j] || (format & 0xFF) != (cachedFormat & 0xF)) {
                isCached = false;
                break;
            }
            pNode = pNode->m_pNext;
            pCached = pCached->m_pNext;
        }
        for (s32 j = 0; j < constantCount; j++) {
            if (constants[j] != pProgram->m_ConstantAttributes[j]) {
                goto rebuild;
            }
        }
        if (isCached && pState->m_IsVertexBufferUsable) {
            // only the base address and the offsets of the loaders
            u32 base = pState->m_ArrayBaseAddress;
            if (!pState->m_IsElementArrayUnused && static_cast<s32>(pState->m_ArrayAddresses[0]) < static_cast<s32>(base)) {
                base = pState->m_ArrayAddresses[0] & ~0xF;
            }
            bool isUnchanged = isSameProgram;
            if (pState->m_ArrayBaseAddress != base || pProgram->m_IsBaseAtElementArray ||
                pState->m_ArrayAddresses[1 + pFirst->m_Index] - base != pRegisters[3]) {
                u32 offset = pState->m_ArrayAddresses[1 + pFirst->m_Index] - base;
                for (s32 k = 1; k < pProgram->m_LoaderCount; k++) {
                    pRegisters[3 + k * 3] = pRegisters[3 + k * 3] - pRegisters[3] + offset;
                }
                pRegisters[3] = offset;
                isUnchanged = false;
                if (pState->m_ArrayBaseAddress == base) {
                    pProgram->m_IsBaseAtElementArray = false;
                }
            }
            pState->m_ArrayBaseAddress = base;
            pRegisters[0] = base >> 3;
            if (isUnchanged) {
                PutCommand(base >> 3, 0xF0200);
            } else if (isSameProgram) {
                __cb_writeRegs(0x200, pProgram->m_LoaderCount * 3 + 1, pRegisters);
            } else {
                __cb_writeRegs(0x200, LOADER_REGISTER_COUNT, pRegisters);
                PutCommand(pProgram->m_VertexInputPermutation[0], 0xF02BB);
                PutCommand(pProgram->m_VertexInputPermutation[1], 0xF02BC);
                if (pProgram->m_HasGeometryShader) {
                    PutCommand(pProgram->m_GeometryInputPermutation[0], 0xF028B);
                    PutCommand(pProgram->m_GeometryInputPermutation[1], 0xF028C);
                }
                AttributeNode* p = pProgram->m_pFirstAttribute;
                pState->m_InputCount = arrayCount + constantCount;
                pState->m_ArrayInputCount = arrayCount;
                s32 input;
                for (input = 0; input < arrayCount; input++) {
                    pState->m_InputAttributes[input] = p->m_Index;
                    p = p->m_pNext;
                }
                for (s32 j = 0; j < constantCount; j++) {
                    pState->m_InputAttributes[input++] = constants[j];
                }
            }
            return;
        }
    }

rebuild:
    {
        AttributeNode* pNode = pFirst;
        for (s32 j = 0; j < arrayCount; j++) {
            pProgram->m_AttributeNodes[j].m_Index = pNode->m_Index;
            pProgram->m_AttributeAddresses[j] = pState->m_ArrayAddresses[1 + pNode->m_Index];
            pProgram->m_AttributeStrides[j] = pState->m_Attributes[pNode->m_Index].m_Stride;
            if (j == arrayCount - 1) {
                pProgram->m_AttributeNodes[j].m_pNext = nullptr;
            } else {
                pProgram->m_AttributeNodes[j].m_pNext = &pProgram->m_AttributeNodes[j + 1];
                pNode = pNode->m_pNext;
            }
        }
    }
    for (s32 j = 0; j < constantCount; j++) {
        pProgram->m_ConstantAttributes[j] = constants[j];
    }
    pProgram->m_pFirstAttribute = pProgram->m_AttributeNodes;
    pProgram->m_AttributeArrayCount = arrayCount;
    pProgram->m_ConstantAttributeCount = constantCount;
    if (pState->m_IsElementArrayUnused) {
        pRegisters[0] = static_cast<s32>(pState->m_ArrayBaseAddress) >> 3;
        pProgram->m_IsBaseAtElementArray = false;
    } else {
        if (pState->m_ElementBufferName == 0) {
            pState->m_IsVertexBufferUsable = false;
        }
        bool isBaseAtElementArray;
        if (static_cast<s32>(pState->m_ArrayAddresses[0]) < static_cast<s32>(pState->m_ArrayBaseAddress)) {
            pState->m_ArrayBaseAddress = pState->m_ArrayAddresses[0] & ~0xF;
            isBaseAtElementArray = true;
        } else {
            isBaseAtElementArray = false;
        }
        pProgram->m_IsBaseAtElementArray = isBaseAtElementArray;
        pRegisters[0] = static_cast<s32>(pState->m_ArrayBaseAddress) >> 3;
    }

    // the vertex loaders: the arrays of one vertex (same stride, within the first vertex) share one,
    // with padding components for the gaps
    if (pState->m_IsVertexBufferUsable) {
        u32* pLoader = &pRegisters[3];
        pProgram->m_LoaderCount = 0;
        for (u32 k = 1; k < LOADER_REGISTER_COUNT; k++) {
            pRegisters[k] = 0;
        }
        u32 alignment = 1;
        u32 size = 0;
        u32 stride = 0;
        u32 end = 0;
        u32 componentCount = 0;
        AttributeNode* pNode = pFirst;
        for (s32 j = 0; j < arrayCount; j++) {
            AttributeNode* pNext = pNode->m_pNext;
            u32 index = pNode->m_Index;
            VertexAttribute* pAttribute = &pState->m_Attributes[index];
            u32 offset = pState->m_ArrayAddresses[1 + index] - pState->m_ArrayBaseAddress;
            u32 type = 0;
            u32 elementSize = 0;
            switch (pAttribute->m_Type) {
            case TYPE_BYTE:
                elementSize = 1;
                break;
            case TYPE_UNSIGNED_BYTE:
                type = 1;
                elementSize = 1;
                break;
            case TYPE_SHORT:
                type = 2;
                elementSize = 2;
                break;
            case TYPE_FLOAT:
                type = 3;
                elementSize = 4;
                break;
            default:
                break;
            }
            u32 format = ((pAttribute->m_Size << 2) - 4) | type;
            if (j < 8) {
                pRegisters[1] |= format << (j * 4);
            } else {
                pRegisters[2] |= format << (j * 4 - 32);
            }
            if (pAttribute->m_Stride == 0) {
                // a loader of its own
                pLoader[0] = offset;
                pLoader[1] |= j;
                pLoader[2] |= 0x10000000 | ((pAttribute->m_Size * elementSize) << 16);
                pLoader += 3;
                pProgram->m_LoaderCount++;
            } else {
                if (elementSize > alignment) {
                    alignment = elementSize;
                }
                u32 bytes = pAttribute->m_Size * elementSize;
                size = ((size + elementSize - 1) & ~(elementSize - 1)) + bytes;
                if (componentCount == 0) {
                    pLoader[0] = offset;
                    stride = pAttribute->m_Stride;
                    end = pAttribute->m_Stride + pState->m_ArrayAddresses[1 + index];
                }
                PutLoaderComponent(pLoader, componentCount, j);
                componentCount++;
                bool isLast;
                u32 nextOffset;
                if (j + 1 == arrayCount) {
                    isLast = true;
                } else {
                    u32 nextAddress = pState->m_ArrayAddresses[1 + pNext->m_Index];
                    nextOffset = nextAddress - pState->m_ArrayBaseAddress;
                    isLast = nextOffset <= offset || pAttribute->m_Stride != pState->m_Attributes[pNext->m_Index].m_Stride ||
                             static_cast<s32>(nextAddress) >= static_cast<s32>(end) || componentCount == MAX_LOADER_COMPONENTS;
                }
                if (isLast) {
                    nextOffset = pLoader[0] + stride;
                }
                // the gap up to the next array (words)
                u32 gap = (nextOffset - offset - bytes) >> 2;
                if (((gap & 3) != 0 ? 1 : 0) + (gap >> 2) > MAX_LOADER_COMPONENTS - componentCount) {
                    pState->m_IsVertexBufferUsable = false;
                    break;
                }
                if (gap != 0) {
                    size = ((size + 3) & ~3) + gap * 4;
                    if ((gap & 3) != 0) {
                        PutLoaderComponent(pLoader, componentCount, 11 + (gap & 3));
                        componentCount++;
                        gap &= ~3;
                    }
                    for (s32 k = 0; k < static_cast<s32>(gap); k += 4) {
                        PutLoaderComponent(pLoader, componentCount, 15);
                        componentCount++;
                    }
                }
                if (isLast) {
                    if (((size + alignment - 1) & ~(alignment - 1)) != stride) {
                        pState->m_IsVertexBufferUsable = false;
                        break;
                    }
                    pLoader[2] |= (stride << 16) | (componentCount << 28);
                    pLoader += 3;
                    pProgram->m_LoaderCount++;
                    alignment = 1;
                    size = 0;
                    componentCount = 0;
                }
            }
            pNode = pNext;
        }
        if (pState->m_IsVertexBufferUsable) {
            // the constant attributes and the number of attributes
            for (s32 j = 0; j < constantCount; j++) {
                pRegisters[2] |= 0x10000 << (arrayCount + j);
            }
            if (arrayCount != 0) {
                pRegisters[2] |= (arrayCount + constantCount - 1) << 28;
            }
            if (!pState->m_IsElementArrayUnused && pProgram->m_LoaderCount == static_cast<s32>(MAX_LOADER_COMPONENTS)) {
                pState->m_IsVertexBufferUsable = false;
            }
        }
    }

    // the inputs of the vertex shader: the arrays, then the constant attributes
    pState->m_ArrayInputCount = arrayCount;
    pState->m_InputCount = arrayCount + constantCount;
    pProgram->m_VertexInputPermutation[0] = 0;
    pProgram->m_VertexInputPermutation[1] = 0;
    pProgram->m_GeometryInputPermutation[0] = 0x76543210;
    pProgram->m_GeometryInputPermutation[1] = 0xFEDCBA98;
    AttributeNode* p = pProgram->m_pFirstAttribute;
    u32 input;
    for (input = 0; input < static_cast<u32>(arrayCount); input++) {
        PutInputPermutation(pProgram, input, p->m_Index);
        pState->m_InputAttributes[input] = p->m_Index;
        p = p->m_pNext;
    }
    for (s32 j = 0; j < constantCount; j++) {
        PutInputPermutation(pProgram, input, constants[j]);
        pState->m_InputAttributes[input] = constants[j];
        input++;
    }
    __cb_writeRegs(0x200, LOADER_REGISTER_COUNT, pRegisters);
    PutCommand(pProgram->m_VertexInputPermutation[0], 0xF02BB);
    PutCommand(pProgram->m_VertexInputPermutation[1], 0xF02BC);
    if (pProgram->m_HasGeometryShader) {
        PutCommand(pProgram->m_GeometryInputPermutation[0], 0xF028B);
        PutCommand(pProgram->m_GeometryInputPermutation[1], 0xF028C);
    }
}

// the values of the constant attributes (the inputs after the arrays)
DECOMP_ALWAYS_INLINE void PutConstantAttributes(GlState* pState)
{
    for (u32 input = pState->m_ArrayInputCount; input < pState->m_InputCount; input++) {
        u32 attribute = pState->m_InputAttributes[input];
        PutCommand(input, 0xF0232);
        PutCommand(pState->m_AttributeValues.m_PackedValues[attribute][0], 0xF0233);
        PutCommand(pState->m_AttributeValues.m_PackedValues[attribute][1], 0xF0234);
        PutCommand(pState->m_AttributeValues.m_PackedValues[attribute][2], 0xF0235);
    }
}

DECOMP_ALWAYS_INLINE bool IsBitSet(const u32* pBits, u32 index)
{
    return (pBits[index >> 5] & (1 << (index & 31))) != 0;
}

// the next bit to test (the rest of a word without bits is skipped)
DECOMP_ALWAYS_INLINE u32 SkipBit(const u32* pBits, u32 index)
{
    return (pBits[index >> 5] >> (index & 31)) == 0 ? (index & ~31) + 32 : index + 1;
}

// the float uniforms of the dirty registers, consecutive registers in one command
DECOMP_ALWAYS_INLINE void PutFloatUniforms(const u32* pValues, const u32* pRegisters, u32 count, const u32* pDirty, u32 indexHeader,
                                           u32 dataRegister)
{
    const u32* pStart = nullptr;
    u32 first = 0;
    u32 run = 0;
    u32 i = 0;
    while (!IsBitSet(pDirty, i)) {
        i = SkipBit(pDirty, i);
    }
    while (i < count) {
        u32 reg = pRegisters[i];
        if (pStart != nullptr && reg - first == run) {
            run++;
        } else {
            if (pStart != nullptr) {
                PutCommand(first | 0x80000000, indexHeader);
                __cb_multiWriteReg(dataRegister, run * 4, pStart);
            }
            first = reg;
            run = 1;
            pStart = pValues + i * 4;
        }
        i++;
        while (!IsBitSet(pDirty, i) && i < count) {
            i = SkipBit(pDirty, i);
        }
    }
    if (pStart != nullptr) {
        PutCommand(first | 0x80000000, indexHeader);
        __cb_multiWriteReg(dataRegister, run * 4, pStart);
    }
}

DECOMP_ALWAYS_INLINE void PutAllFloatUniforms(Program* pProgram)
{
    if (pProgram->m_VertexDirtyFloatRegisters[0] == 0 && pProgram->m_VertexDirtyFloatRegisters[1] == 0 &&
        pProgram->m_VertexDirtyFloatRegisters[2] == 0 && pProgram->m_GeometryDirtyFloatRegisters[0] == 0 &&
        pProgram->m_GeometryDirtyFloatRegisters[1] == 0 && pProgram->m_GeometryDirtyFloatRegisters[2] == 0) {
        return;
    }
    if (pProgram->m_pGeometryFloatValues != nullptr &&
        (pProgram->m_GeometryDirtyFloatRegisters[0] != 0 || pProgram->m_GeometryDirtyFloatRegisters[1] != 0 ||
         pProgram->m_GeometryDirtyFloatRegisters[2] != 0)) {
        PutFloatUniforms(pProgram->m_pGeometryFloatValues, pProgram->m_GeometryFloatRegisters, pProgram->m_GeometryFloatRegisterCount,
                         pProgram->m_GeometryDirtyFloatRegisters, 0xF0290, 0x291);
    }
    if (pProgram->m_pVertexFloatValues != nullptr &&
        (pProgram->m_VertexDirtyFloatRegisters[0] != 0 || pProgram->m_VertexDirtyFloatRegisters[1] != 0 ||
         pProgram->m_VertexDirtyFloatRegisters[2] != 0)) {
        PutFloatUniforms(pProgram->m_pVertexFloatValues, pProgram->m_VertexFloatRegisters, pProgram->m_VertexFloatRegisterCount,
                         pProgram->m_VertexDirtyFloatRegisters, 0xF02C0, 0x2C1);
    }
    pProgram->m_VertexDirtyFloatRegisters[0] = 0;
    pProgram->m_VertexDirtyFloatRegisters[1] = 0;
    pProgram->m_VertexDirtyFloatRegisters[2] = 0;
    pProgram->m_GeometryDirtyFloatRegisters[0] = 0;
    pProgram->m_GeometryDirtyFloatRegisters[1] = 0;
    pProgram->m_GeometryDirtyFloatRegisters[2] = 0;
}

// a register of the program with the validation (see SetRegisterBits in gx_Uniform.cpp)
DECOMP_ALWAYS_INLINE void SetRegister(Program* pProgram, GlState* pState, u32 index, u32 value, u32 dirtyFlags)
{
    pProgram->m_RegisterMasks[index] |= 0xF;
    if (pState->m_IsCommandGenerationDeferred) {
        pProgram->m_RegisterValues[index] = value;
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
        pState->m_DirtyFlags |= dirtyFlags;
        s_ShaderManager.m_pTables->m_SentRegisterValues[index] = ~pProgram->m_RegisterValues[index];
    } else if (pProgram->m_RegisterValues[index] != value) {
        pProgram->m_RegisterValues[index] = value;
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
        pState->m_DirtyFlags |= dirtyFlags;
    }
}

// the depth registers again (the depth range and polygon offset of the GL state with wScale 0)
DECOMP_ALWAYS_INLINE void UpdateDepthRegisters(Program* pProgram, GlState* pState, u32 dirtyFlags)
{
    f32 wScale = *reinterpret_cast<f32*>(ReservedWord(pProgram, RESERVED_W_SCALE));
    f32 scale;
    f32 offset;
    if (wScale == 0.0f) {
        offset = pState->m_DepthRangeNear;
        scale = offset - pState->m_DepthRangeFar;
    } else {
        scale = -wScale;
        offset = 0.0f;
    }
    if (pState->m_IsPolygonOffsetFillEnabled && pState->m_PolygonOffsetUnits != 0.0f) {
        offset += pState->m_PolygonOffsetUnits * (pState->m_DepthFormat != 0 ? 1.0f / 16777215.0f : 1.0f / 65535.0f);
    }
    u32 scaleValue = Float32ToFloat24(scale);
    u32 offsetValue = offset == 0.0f ? 0 : Float32ToFloat24(offset);
    SetRegister(pProgram, pState, REGISTER_DEPTH_SCALE, scaleValue, dirtyFlags);
    SetRegister(pProgram, pState, REGISTER_DEPTH_OFFSET, offsetValue, dirtyFlags);
}

// with automatic accumulation the gas accumulation maximum is not sent
DECOMP_ALWAYS_INLINE void UpdateGasAccumulationMaximum(Program* pProgram)
{
    if ((pProgram->m_RegisterValues[REGISTER_GAS_FOG_MODE] & 7) != 7) {
        return;
    }
    if (pProgram->m_IsGasAutoAcc) {
        s_ShaderManager.m_pTables->m_SentRegisterValues[REGISTER_GAS_ACC_MAX] = pProgram->m_RegisterValues[REGISTER_GAS_ACC_MAX];
        pProgram->m_DirtyRegisters[REGISTER_GAS_ACC_MAX >> 5] &= ~(1 << (REGISTER_GAS_ACC_MAX & 31));
    } else {
        s_ShaderManager.m_pTables->m_SentRegisterValues[REGISTER_GAS_ACC_MAX] = ~pProgram->m_RegisterValues[REGISTER_GAS_ACC_MAX];
        pProgram->m_DirtyRegisters[REGISTER_GAS_ACC_MAX >> 5] |= 1 << (REGISTER_GAS_ACC_MAX & 31);
    }
}

// the dirty registers of a list, one command each
DECOMP_ALWAYS_INLINE void PutRegisterList(Program* pProgram, const s32* pList)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    for (u32 i = 0; pList[i] != static_cast<s32>(SHADER_REGISTER_COUNT); i++) {
        u32 index = pList[i];
        if (!IsBitSet(pProgram->m_DirtyRegisters, index)) {
            continue;
        }
        u8 mask = pProgram->m_RegisterMasks[index];
        if (mask != 0 && pProgram->m_RegisterValues[index] != pTables->m_SentRegisterValues[index]) {
            PutCommand(pProgram->m_RegisterValues[index], s_ShaderRegisters[pList[i]] | (mask << 16));
            pTables->m_SentRegisterValues[index] = pProgram->m_RegisterValues[index];
        }
        pProgram->m_DirtyRegisters[pList[i] >> 5] &= ~(1 << (pList[i] & 31));
    }
}

// a lighting table in the format of the GPU: 12 bits value, 12 bits difference
DECOMP_ALWAYS_INLINE void ConvertLightingLut(TextureLut* pLut)
{
    if ((pLut->m_DirtyBuffers & (1 << LUT_BUFFER_LIGHTING)) == 0) {
        return;
    }
    if (pLut->m_pBuffers[LUT_BUFFER_LIGHTING] == nullptr) {
        pLut->m_pBuffers[LUT_BUFFER_LIGHTING] = static_cast<u32*>(AllocateLutBuffer(256 * sizeof(u32)));
    }
    const f32* pValues = GetLutValues(pLut);
    for (u32 i = 0; i < 256; i++) {
        u32 value = ToLutValue(pValues[i], 4096.0f, 0xFFF);
        pLut->m_pBuffers[LUT_BUFFER_LIGHTING][i] = value;
        pLut->m_pBuffers[LUT_BUFFER_LIGHTING][i] = value | (ToLightingDifference(pValues[256 + i]) << 12);
    }
    pLut->m_DirtyBuffers &= ~(1 << LUT_BUFFER_LIGHTING);
}

// a lighting table of a slot: the changed part if the table is the one sent last, else all of it
// (true if all of it was sent)
DECOMP_ALWAYS_INLINE bool PutLightingLut(GlState* pState, u32 slot, GLuint name, TextureLut* pLut, u32 id)
{
    if (pState->m_UsedLutNames[slot] == name) {
        if (pState->m_LutUpdateCounts[slot] != 0) {
            PutCommand(pState->m_LutUpdateStarts[slot] | (id << 8), 0xF01C5);
            __cb_multiWriteReg(0x1C8, pState->m_LutUpdateCounts[slot], pLut->m_pBuffers[LUT_BUFFER_LIGHTING] + pState->m_LutUpdateStarts[slot]);
            pState->m_LutUpdateCounts[slot] = 0;
        }
        return false;
    }
    if (pState->m_LutUpdateCounts[slot] != 0) {
        pState->m_LutUpdateCounts[slot] = 0;
    }
    pState->m_UsedLutNames[slot] = name;
    ConvertLightingLut(pLut);
    PutCommand(id << 8, 0xF01C5);
    __cb_multiWriteReg(0x1C8, 256, pLut->m_pBuffers[LUT_BUFFER_LIGHTING]);
    return true;
}

// the lighting tables: of the material (as the configuration uses them) and of the enabled lights
DECOMP_ALWAYS_INLINE bool PutLightingLuts(GlState* pState, Program* pProgram, const u32* pDisableBits, const u32* pLutIds)
{
    bool isUploaded = false;
    for (u32 t = 0; t < 6; t++) {
        if (((*ReservedWord(pProgram, RESERVED_LIGHTING_CONFIG) >> t) & 1) == 0 ||
            ((pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG1] >> pDisableBits[t]) & 1) != 0) {
            continue;
        }
        s32 unit = *ReservedWord(pProgram, RESERVED_MATERIAL_LUTS + t * 4);
        if (unit == -1) {
            break;
        }
        GLuint name = pState->m_LutNames[unit];
        if (name == 0) {
            break;
        }
        TextureLut* pLut = __tx_getBoundTextureLut(unit);
        if (pLut == nullptr) {
            break;
        }
        if (PutLightingLut(pState, LUT_SLOT_MATERIAL + t, name, pLut, pLutIds[t])) {
            isUploaded = true;
        }
    }
    for (u32 i = 0; i < LIGHT_COUNT; i++) {
        if (!IsLightEnabled(pProgram, i)) {
            continue;
        }
        if ((*ReservedWord(pProgram, RESERVED_LIGHTING_CONFIG) & 0x40) != 0 &&
            ((pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG1] >> (i + 8)) & 1) == 0) {
            u32 unit = *ReservedWord(pProgram, RESERVED_LIGHTS + i * LIGHT_SIZE + LIGHT_LUT_SP);
            GLuint name = pState->m_LutNames[unit];
            TextureLut* pLut = __tx_getBoundTextureLut(unit);
            if (PutLightingLut(pState, LUT_SLOT_SP + i, name, pLut, 8 + i)) {
                isUploaded = true;
            }
        }
        if (((pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG1] >> (i + 24)) & 1) != 0) {
            continue;
        }
        u32 unit = *ReservedWord(pProgram, RESERVED_LIGHTS + i * LIGHT_SIZE + LIGHT_LUT_DA);
        GLuint name = pState->m_LutNames[unit];
        TextureLut* pLut = __tx_getBoundTextureLut(unit);
        if (PutLightingLut(pState, LUT_SLOT_DA + i, name, pLut, 16 + i)) {
            isUploaded = true;
        }
    }
    return isUploaded;
}

// a map or noise table of procedural textures: 12 bits value, 12 bits signed difference
DECOMP_ALWAYS_INLINE void ConvertProcTexLut(TextureLut* pLut, u32 buffer)
{
    if ((pLut->m_DirtyBuffers & (1 << buffer)) == 0) {
        return;
    }
    if (pLut->m_pBuffers[buffer] == nullptr) {
        pLut->m_pBuffers[buffer] = static_cast<u32*>(AllocateLutBuffer(128 * sizeof(u32)));
    }
    const f32* pValues = GetLutValues(pLut);
    for (u32 i = 0; i < 128; i++) {
        u32 value = ToLutValue(pValues[i], 4096.0f, 0xFFF);
        pLut->m_pBuffers[buffer][i] = value;
        pLut->m_pBuffers[buffer][i] = value | (ToSignedLutValue(pValues[128 + i], 1.0f, 2048.0f, 4095.0f, 2048.0f) << 12);
    }
    pLut->m_DirtyBuffers &= ~(1 << buffer);
}

DECOMP_ALWAYS_INLINE bool PutProcTexLut(GlState* pState, u32 slot, GLuint name, TextureLut* pLut, u32 buffer, u32 id)
{
    if (pState->m_UsedLutNames[slot] == name) {
        if (pState->m_LutUpdateCounts[slot] != 0) {
            PutCommand(pState->m_LutUpdateStarts[slot] | (id << 8), 0xF00AF);
            __cb_multiWriteReg(0xB0, pState->m_LutUpdateCounts[slot], pLut->m_pBuffers[buffer] + pState->m_LutUpdateStarts[slot]);
            pState->m_LutUpdateCounts[slot] = 0;
        }
        return false;
    }
    if (pState->m_LutUpdateCounts[slot] != 0) {
        pState->m_LutUpdateCounts[slot] = 0;
    }
    pState->m_UsedLutNames[slot] = name;
    ConvertProcTexLut(pLut, buffer);
    PutCommand(id << 8, 0xF00AF);
    __cb_multiWriteReg(0xB0, 128, pLut->m_pBuffers[buffer]);
    return true;
}

// a color table of procedural textures in bytes: 256 values, 255 differences
DECOMP_ALWAYS_INLINE void ConvertProcTexColorLut(TextureLut* pLut)
{
    if ((pLut->m_DirtyBuffers & (1 << LUT_BUFFER_PT_COLOR)) == 0) {
        return;
    }
    if (pLut->m_pBuffers[LUT_BUFFER_PT_COLOR] == nullptr) {
        pLut->m_pBuffers[LUT_BUFFER_PT_COLOR] = static_cast<u32*>(AllocateLutBuffer(512));
    }
    const f32* pValues = GetLutValues(pLut);
    u32 i;
    for (i = 0; i < 256; i++) {
        f32 value = pValues[i] > 1.0f ? 1.0f : pValues[i];
        reinterpret_cast<u8*>(pLut->m_pBuffers[LUT_BUFFER_PT_COLOR])[i] = static_cast<u32>(value * 255.0f + 0.5f);
    }
    for (; i < 511; i++) {
        reinterpret_cast<u8*>(pLut->m_pBuffers[LUT_BUFFER_PT_COLOR])[i] = ToSignedLutValue(pValues[i], 1.0f, 128.0f, 255.0f, 128.0f);
    }
    reinterpret_cast<u8*>(pLut->m_pBuffers[LUT_BUFFER_PT_COLOR])[i] = 0;
    pLut->m_DirtyBuffers &= ~(1 << LUT_BUFFER_PT_COLOR);
}

// the color tables of procedural textures (red, green, blue, alpha in the bytes of one table): the
// changed parts if the tables are the ones sent last, else all of them
DECOMP_ALWAYS_INLINE bool PutProcTexColorLuts(GlState* pState, Program* pProgram, u32* pColors)
{
    u32 c;
    for (c = 0; c < 4; c++) {
        u32 unit = *ReservedWord(pProgram, RESERVED_PT_LUTS + 12 + c * 4);
        if (pState->m_LutNames[unit] != pState->m_UsedLutNames[LUT_SLOT_PT_COLOR + c]) {
            break;
        }
    }
    if (c == 4) {
        if (pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR] == 0 && pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + 1] == 0 &&
            pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + 2] == 0 && pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + 3] == 0) {
            return false;
        }
        u32 first = 512;
        u32 last = 0;
        for (c = 0; c < 4; c++) {
            u32 count = pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + c];
            if (count == 0) {
                continue;
            }
            u32 start = pState->m_LutUpdateStarts[LUT_SLOT_PT_COLOR + c];
            pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + c] = 0;
            if (start < first) {
                first = start;
            }
            if (start + count - 1 > last) {
                last = start + count - 1;
            }
        }
        for (c = 0; c < 4; c++) {
            TextureLut* pLut = __tx_getBoundTextureLut(*ReservedWord(pProgram, RESERVED_PT_LUTS + 12 + c * 4));
            for (u32 k = first; k <= last; k++) {
                pColors[k] = (pColors[k] & ~(0xFF << (c * 8))) | (reinterpret_cast<u8*>(pLut->m_pBuffers[LUT_BUFFER_PT_COLOR])[k] << (c * 8));
            }
        }
        if (first < 256) {
            if (last >= 256) {
                PutCommand(first | 0x400, 0xF00AF);
                __cb_multiWriteReg(0xB0, 256 - first, pColors + first);
                PutCommand(0x500, 0xF00AF);
                __cb_multiWriteReg(0xB0, last - 255, pColors + 256);
                return false;
            }
            PutCommand(first | 0x400, 0xF00AF);
        } else {
            PutCommand((first - 256) | 0x500, 0xF00AF);
        }
        __cb_multiWriteReg(0xB0, last - first + 1, pColors + first);
        return false;
    }
    for (c = 0; c < 4; c++) {
        TextureLut* pLut = __tx_getBoundTextureLut(*ReservedWord(pProgram, RESERVED_PT_LUTS + 12 + c * 4));
        if (pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + c] != 0) {
            pState->m_LutUpdateCounts[LUT_SLOT_PT_COLOR + c] = 0;
        }
        ConvertProcTexColorLut(pLut);
        for (u32 k = 0; k < 512; k++) {
            pColors[k] = (pColors[k] & ~(0xFF << (c * 8))) | (reinterpret_cast<u8*>(pLut->m_pBuffers[LUT_BUFFER_PT_COLOR])[k] << (c * 8));
        }
    }
    PutCommand(0x400, 0xF00AF);
    __cb_multiWriteReg(0xB0, 256, pColors);
    PutCommand(0x500, 0xF00AF);
    __cb_multiWriteReg(0xB0, 256, pColors + 256);
    for (c = 0; c < 4; c++) {
        pState->m_UsedLutNames[LUT_SLOT_PT_COLOR + c] = pState->m_LutNames[*ReservedWord(pProgram, RESERVED_PT_LUTS + 12 + c * 4)];
    }
    return true;
}

// the tables of procedural textures: the maps (alpha only separate), the noise, the colors
DECOMP_ALWAYS_INLINE bool PutProcTexLuts(GlState* pState, Program* pProgram, u32* pColors)
{
    bool isUploaded = false;
    const u32 ids[2] = {2, 3};
    for (u32 k = 0; k < 2; k++) {
        if (k == 1 && (pProgram->m_RegisterValues[REGISTER_PT_CONTROL] & 0x4000) == 0) {
            break;
        }
        u32 unit = *ReservedWord(pProgram, RESERVED_PT_LUTS + k * 4);
        GLuint name = pState->m_LutNames[unit];
        TextureLut* pLut = __tx_getBoundTextureLut(unit);
        if (PutProcTexLut(pState, LUT_SLOT_PT + k, name, pLut, LUT_BUFFER_PT_MAP, ids[k])) {
            isUploaded = true;
        }
    }
    if ((pProgram->m_RegisterValues[REGISTER_PT_CONTROL] & 0x8000) != 0) {
        u32 unit = *ReservedWord(pProgram, RESERVED_PT_LUTS + 8);
        GLuint name = pState->m_LutNames[unit];
        TextureLut* pLut = __tx_getBoundTextureLut(unit);
        if (PutProcTexLut(pState, LUT_SLOT_PT_NOISE, name, pLut, LUT_BUFFER_PT_NOISE, 0)) {
            isUploaded = true;
        }
    }
    if (PutProcTexColorLuts(pState, pProgram, pColors)) {
        isUploaded = true;
    }
    return isUploaded;
}

// the fog table: 13 bits signed difference, 11 bits value
DECOMP_ALWAYS_INLINE void ConvertFogLut(TextureLut* pLut)
{
    if ((pLut->m_DirtyBuffers & (1 << LUT_BUFFER_FOG)) == 0) {
        return;
    }
    if (pLut->m_pBuffers[LUT_BUFFER_FOG] == nullptr) {
        pLut->m_pBuffers[LUT_BUFFER_FOG] = static_cast<u32*>(AllocateLutBuffer(128 * sizeof(u32)));
    }
    const f32* pValues = GetLutValues(pLut);
    for (u32 i = 0; i < 128; i++) {
        pLut->m_pBuffers[LUT_BUFFER_FOG][i] = ToSignedLutValue(pValues[128 + i], 2.0f, 2048.0f, 8191.0f, 4096.0f);
        pLut->m_pBuffers[LUT_BUFFER_FOG][i] |= ToLutValue(pValues[i], 2048.0f, 0x7FF) << 13;
    }
    pLut->m_DirtyBuffers &= ~(1 << LUT_BUFFER_FOG);
}

DECOMP_ALWAYS_INLINE bool PutFogLut(GlState* pState, Program* pProgram)
{
    u32 unit = *ReservedWord(pProgram, RESERVED_FOG_LUT);
    GLuint name = pState->m_LutNames[unit];
    TextureLut* pLut = __tx_getBoundTextureLut(unit);
    if (pState->m_UsedLutNames[LUT_SLOT_FOG] == name) {
        if (pState->m_LutUpdateCounts[LUT_SLOT_FOG] != 0) {
            PutCommand(pState->m_LutUpdateStarts[LUT_SLOT_FOG], 0xF00E6);
            __cb_multiWriteReg(0xE8, pState->m_LutUpdateCounts[LUT_SLOT_FOG], pLut->m_pBuffers[LUT_BUFFER_FOG] + pState->m_LutUpdateStarts[LUT_SLOT_FOG]);
            pState->m_LutUpdateCounts[LUT_SLOT_FOG] = 0;
        }
        return false;
    }
    if (pState->m_LutUpdateCounts[LUT_SLOT_FOG] != 0) {
        pState->m_LutUpdateCounts[LUT_SLOT_FOG] = 0;
    }
    pState->m_UsedLutNames[LUT_SLOT_FOG] = name;
    ConvertFogLut(pLut);
    PutCommand(0, 0xF00E6);
    __cb_multiWriteReg(0xE8, 128, pLut->m_pBuffers[LUT_BUFFER_FOG]);
    return true;
}

// a gas table: eight values (8 bits) and eight differences (sign and 7 bits)
DECOMP_ALWAYS_INLINE void ConvertGasLut(TextureLut* pLut)
{
    if ((pLut->m_DirtyBuffers & (1 << LUT_BUFFER_GAS)) == 0) {
        return;
    }
    if (pLut->m_pBuffers[LUT_BUFFER_GAS] == nullptr) {
        pLut->m_pBuffers[LUT_BUFFER_GAS] = static_cast<u32*>(AllocateLutBuffer(16 * sizeof(u32)));
    }
    const f32* pValues = GetLutValues(pLut);
    for (u32 i = 0; i < 8; i++) {
        f32 value = pValues[i] * 255.0f;
        pLut->m_pBuffers[LUT_BUFFER_GAS][i] = value <= 0.0f || IsInfiniteOrNan(value) ? 0 : (value < 256.0f ? static_cast<u32>(value) : 255);
        u32 difference = static_cast<u32>(fabsf(pValues[8 + i] * 127.0f)) & 0x7F;
        pLut->m_pBuffers[LUT_BUFFER_GAS][8 + i] = difference;
        if (pValues[8 + i] < 0.0f) {
            pLut->m_pBuffers[LUT_BUFFER_GAS][8 + i] = difference | 0x80;
        }
    }
    pLut->m_DirtyBuffers &= ~(1 << LUT_BUFFER_GAS);
}

// the gas tables (red, green, blue in the bytes of one table; the differences first)
DECOMP_ALWAYS_INLINE void PutGasLuts(GlState* pState, Program* pProgram, bool isLutUploaded)
{
    u32 c;
    for (c = 0; c < 3; c++) {
        if (pState->m_LutNames[*ReservedWord(pProgram, RESERVED_GAS_LUTS + c * 4)] != pState->m_UsedLutNames[LUT_SLOT_GAS + c]) {
            break;
        }
    }
    if (c == 3) {
        return;
    }
    u32 table[16];
    for (u32 i = 0; i < 16; i++) {
        table[i] = 0;
    }
    for (c = 0; c < 3; c++) {
        TextureLut* pLut = __tx_getBoundTextureLut(*ReservedWord(pProgram, RESERVED_GAS_LUTS + c * 4));
        ConvertGasLut(pLut);
        for (u32 i = 0; i < 8; i++) {
            table[i] |= pLut->m_pBuffers[LUT_BUFFER_GAS][8 + i] << (c * 8);
            table[8 + i] |= pLut->m_pBuffers[LUT_BUFFER_GAS][i] << (c * 8);
        }
    }
    if (!isLutUploaded) {
        __cb_addDummyWrite(0xC0, 45);
    }
    PutCommand(0, 0xF0123);
    __cb_multiWriteReg(0x124, 16, table);
    PutCommand(0, 0x100);
    for (c = 0; c < 3; c++) {
        pState->m_UsedLutNames[LUT_SLOT_GAS + c] = pState->m_LutNames[*ReservedWord(pProgram, RESERVED_GAS_LUTS + c * 4)];
    }
}

// all lookup tables the program uses
DECOMP_ALWAYS_INLINE void PutLuts(GlState* pState, Program* pProgram, u32 dirtyFlags, bool* pIsLutUploaded, const u32* pDisableBits,
                                  const u32* pLutIds)
{
    u32 colors[512];
    if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_ENABLE] & 1) != 0 && (dirtyFlags & (DIRTY_LUT_DATA | DIRTY_LIGHTING_LUTS)) != 0) {
        if (PutLightingLuts(pState, pProgram, pDisableBits, pLutIds)) {
            *pIsLutUploaded = true;
        }
    }
    if (*ReservedWord(pProgram, RESERVED_PT_SAMPLER_TYPE) != 0 && (dirtyFlags & (DIRTY_LUT_DATA | DIRTY_PROCEDURAL_LUTS)) != 0) {
        if (PutProcTexLuts(pState, pProgram, colors)) {
            *pIsLutUploaded = true;
        }
    }
    if ((pProgram->m_RegisterValues[REGISTER_GAS_FOG_MODE] & 7) != 0 && (dirtyFlags & (DIRTY_LUT_DATA | DIRTY_FOG_LUT)) != 0) {
        if (PutFogLut(pState, pProgram)) {
            *pIsLutUploaded = true;
        }
    }
    if ((pProgram->m_RegisterValues[REGISTER_GAS_FOG_MODE] & 7) == 7 && (dirtyFlags & (DIRTY_LUT_DATA | DIRTY_GAS_LUT)) != 0) {
        PutGasLuts(pState, pProgram, *pIsLutUploaded);
    }
}

// the access of the fragment operations to the framebuffer (color read/write, depth and stencil
// read/write) for the mode of the program
DECOMP_ALWAYS_INLINE void PutFragmentMode(GlState* pState, Program* pProgram)
{
    u32 colorMask = pState->m_ColorMask[0] | (pState->m_ColorMask[1] << 1) | (pState->m_ColorMask[2] << 2) | (pState->m_ColorMask[3] << 3);
    pState->m_LastFragmentMode = pProgram->m_Mode;
    PutCommand(1, 0xF0111);
    PutCommand(1, 0xF0110);
    switch (pState->m_LastFragmentMode) {
    case FRAGMENT_MODE_DEFAULT: {
        u32 access;
        if (colorMask == 0) {
            access = 0;
        } else {
            bool isColorRead = pState->m_ColorFormat == 1 || pState->m_IsBlendEnabled || colorMask != 15 || pState->m_IsLogicOpEnabled;
            access = 2 | (isColorRead ? 1 : 0);
        }
        access |= pState->m_IsDepthTestEnabled ? 4 : 0;
        access |= pState->m_IsDepthTestEnabled && pState->m_DepthMask ? 8 : 0;
        access |= pState->m_IsStencilTestEnabled ? 16 : 0;
        access |= pState->m_IsStencilTestEnabled && pState->m_StencilWriteMask != 0 ? 32 : 0;
        PutCommand((access & 1) != 0 ? 15 : 0, 0xF0112);
        PutCommand((access & 2) != 0 ? 15 : 0, 0xF0113);
        PutCommand(((access & 4) != 0 && (access & 10) != 0 ? 2 : 0) | ((access & 16) != 0 && (access & 34) != 0 ? 1 : 0), 0xF0114);
        PutCommand(((access & 8) != 0 ? 2 : 0) | ((access & 32) >> 5), 0xF0115);
        break;
    }
    case FRAGMENT_MODE_SHADOW:
        PutCommand(15, 0xF0113);
        PutCommand(15, 0xF0112);
        PutCommand(0, 0xF0114);
        PutCommand(0, 0xF0115);
        break;
    case FRAGMENT_MODE_GAS:
        PutCommand(15, 0xF0113);
        PutCommand(15, 0xF0112);
        PutCommand(3, 0xF0114);
        PutCommand(0, 0xF0115);
        break;
    default:
        break;
    }
}
} // namespace

extern "C" {
// (only the parts of the given states; the registers one command each)
// 0x0077C890 | nintendogs:bytes [tier B]
void __shv_partialValidateShaderValidator(const u32* pDirtyFlags, GLbitfield states)
{
    // 0x008B3280 (name is ours)
    static const u32 s_LutDisableBits[6] = {16, 17, 19, 20, 21, 22};
    // 0x008B3298 (name is ours)
    static const u32 s_LutIds[6] = {0, 1, 3, 4, 5, 6};
    bool isLutUploaded = false;
    GlState* pState = s_pGlState;
    if ((states & STATE_SCISSOR) != 0 && (*pDirtyFlags & DIRTY_SCISSOR) != 0) {
        PutScissor(pState);
    }
    Program* pProgram = s_ShaderManager.m_pTables->m_pCurrentProgram;
    if (pProgram == nullptr) {
        return;
    }
    if ((states & STATE_SHADER_INTEGERS) != 0) {
        PutGeometryShaderState(pState, pProgram);
    }
    if ((states & STATE_SHADER_CODE) != 0 && (*pDirtyFlags & DIRTY_SHADER_CODE) != 0) {
        PutShaderCode(pProgram);
    }
    if ((states & STATE_SHADER_INTEGERS) != 0) {
        if (pProgram->m_HasGeometryShader && (*pDirtyFlags & DIRTY_GEOMETRY_INTEGERS) != 0) {
            InvalidateRegisters(pProgram, 1, 8);
        }
        if ((*pDirtyFlags & DIRTY_VERTEX_INTEGERS) != 0) {
            InvalidateRegisters(pProgram, 9, 16);
        }
    }
    if ((states & STATE_FLOAT_CONSTANTS) != 0 && (*pDirtyFlags & (DIRTY_VERTEX_FLOAT_CONSTANTS | DIRTY_GEOMETRY_FLOAT_CONSTANTS)) != 0) {
        PutFloatConstants(pProgram, *pDirtyFlags);
    }
    if ((states & STATE_VERTEX) != 0) {
        if ((*pDirtyFlags & DIRTY_ATTRIBUTE_ARRAYS) != 0) {
            SetUpAttributes(pState, pProgram);
        }
        if (pState->m_IsVertexBufferUsable && (*pDirtyFlags & DIRTY_ATTRIBUTE_VALUES) != 0 && pState->m_InputCount > pState->m_ArrayInputCount) {
            PutConstantAttributes(pState);
        }
    }
    u32 updated = pState->m_UpdatedStates;
    if (updated != 0) {
        if ((updated & states & STATE_FLOAT_UNIFORMS) != 0) {
            pProgram->m_VertexDirtyFloatRegisters[2] = 0xFFFFFFFF;
            pProgram->m_VertexDirtyFloatRegisters[1] = 0xFFFFFFFF;
            pProgram->m_VertexDirtyFloatRegisters[0] = 0xFFFFFFFF;
            InvalidateRegisterList(pProgram, s_RegisterList1);
            if (pProgram->m_HasGeometryShader) {
                pProgram->m_GeometryDirtyFloatRegisters[2] = 0xFFFFFFFF;
                pProgram->m_GeometryDirtyFloatRegisters[1] = 0xFFFFFFFF;
                pProgram->m_GeometryDirtyFloatRegisters[0] = 0xFFFFFFFF;
                InvalidateRegisterList(pProgram, s_RegisterList2);
            }
        }
        if ((states & pState->m_UpdatedStates & STATE_FRAGMENT_REGISTERS) != 0) {
            InvalidateRegisterList(pProgram, s_RegisterList3);
        }
        if ((states & pState->m_UpdatedStates & STATE_SHADER_REGISTERS) != 0) {
            InvalidateRegisterList(pProgram, s_RegisterList0);
        }
    }
    if ((states & STATE_FLOAT_UNIFORMS) != 0) {
        PutAllFloatUniforms(pProgram);
    }
    if ((states & STATE_DEPTH) != 0 && (*pDirtyFlags & DIRTY_DEPTH) != 0) {
        UpdateDepthRegisters(pProgram, pState, DIRTY_RESERVED_REGISTERS);
    }
    if ((states & STATE_FRAGMENT_REGISTERS) != 0) {
        UpdateGasAccumulationMaximum(pProgram);
        if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_ENABLE] & 1) != 0) {
            for (u32 i = 0; i < LIGHT_COUNT; i++) {
                if (IsLightEnabled(pProgram, i)) {
                    break;
                }
            }
        }
    }
    if ((states & STATE_FLOAT_UNIFORMS) != 0) {
        PutRegisterList(pProgram, s_RegisterList2);
        PutRegisterList(pProgram, s_RegisterList1);
    }
    if ((states & STATE_SHADER_REGISTERS) != 0) {
        PutRegisterList(pProgram, s_RegisterList0);
    }
    if ((states & STATE_FRAGMENT_REGISTERS) != 0) {
        PutRegisterList(pProgram, s_RegisterList3);
    }
    if ((states & STATE_LUTS) != 0) {
        PutLuts(pState, pProgram, *pDirtyFlags, &isLutUploaded, s_LutDisableBits, s_LutIds);
    }
    if ((states & STATE_FRAGMENT_MODE) != 0 && ((*pDirtyFlags & DIRTY_FRAGMENT_MODE) != 0 || pProgram->m_Mode != pState->m_LastFragmentMode)) {
        PutFragmentMode(pState, pProgram);
    }
}

// (the registers of the program that changed, consecutive ones in one command)
// 0x00780388 | nintendogs:bytes [tier B]
void __shv_validateShaderValidator(const u32* pDirtyFlags)
{
    // 0x008B3250 (name is ours)
    static const u32 s_LutDisableBits[6] = {16, 17, 19, 20, 21, 22};
    // 0x008B3268 (name is ours)
    static const u32 s_LutIds[6] = {0, 1, 3, 4, 5, 6};
    bool areLightsOff = false;
    bool isLutUploaded = false;
    GlState* pState = s_pGlState;
    if ((*pDirtyFlags & DIRTY_SCISSOR) != 0 && (pState->m_IgnoredStates & STATE_SCISSOR) == 0) {
        PutScissor(pState);
    }
    Program* pProgram = s_ShaderManager.m_pTables->m_pCurrentProgram;
    if (pProgram == nullptr) {
        return;
    }
    if ((pState->m_IgnoredStates & STATE_SHADER_INTEGERS) == 0) {
        PutGeometryShaderState(pState, pProgram);
    }
    if (pProgram->m_HasGeometryShader) {
        // the geometry shader runs (bool 15 of the geometry unit)
        pProgram->m_RegisterValues[REGISTER_GEOMETRY_BOOL] &= ~0x8000;
        s_ShaderManager.m_pTables->m_SentRegisterValues[REGISTER_GEOMETRY_BOOL] |= 0x8000;
        pProgram->m_RegisterMasks[REGISTER_GEOMETRY_BOOL] |= 0x2;
        pProgram->m_DirtyRegisters[0] |= 1 << REGISTER_GEOMETRY_BOOL;
        if ((*pDirtyFlags & DIRTY_GEOMETRY_INTEGERS) != 0 && (pState->m_IgnoredStates & STATE_SHADER_INTEGERS) == 0) {
            InvalidateRegisters(pProgram, 1, 8);
        }
    }
    if ((*pDirtyFlags & DIRTY_SHADER_CODE) != 0 && (pState->m_IgnoredStates & STATE_SHADER_CODE) == 0) {
        PutShaderCode(pProgram);
    }
    if ((*pDirtyFlags & DIRTY_VERTEX_INTEGERS) != 0 && (pState->m_IgnoredStates & STATE_SHADER_INTEGERS) == 0) {
        InvalidateRegisters(pProgram, 9, 16);
    }
    if ((*pDirtyFlags & (DIRTY_VERTEX_FLOAT_CONSTANTS | DIRTY_GEOMETRY_FLOAT_CONSTANTS)) != 0 && (pState->m_IgnoredStates & STATE_FLOAT_CONSTANTS) == 0) {
        PutFloatConstants(pProgram, *pDirtyFlags);
    }
    if ((pState->m_IgnoredStates & STATE_VERTEX) == 0) {
        if ((*pDirtyFlags & DIRTY_ATTRIBUTE_ARRAYS) != 0) {
            SetUpAttributes(pState, pProgram);
        }
        if (pState->m_IsVertexBufferUsable && (*pDirtyFlags & DIRTY_ATTRIBUTE_VALUES) != 0 && pState->m_InputCount > pState->m_ArrayInputCount) {
            PutConstantAttributes(pState);
        }
    }
    if (pState->m_UpdatedStates != 0) {
        if ((pState->m_UpdatedStates & STATE_FLOAT_UNIFORMS) != 0) {
            pProgram->m_VertexDirtyFloatRegisters[2] = 0xFFFFFFFF;
            pProgram->m_VertexDirtyFloatRegisters[1] = 0xFFFFFFFF;
            pProgram->m_VertexDirtyFloatRegisters[0] = 0xFFFFFFFF;
            InvalidateRegisterList(pProgram, s_RegisterList1);
            if (pProgram->m_HasGeometryShader) {
                pProgram->m_GeometryDirtyFloatRegisters[2] = 0xFFFFFFFF;
                pProgram->m_GeometryDirtyFloatRegisters[1] = 0xFFFFFFFF;
                pProgram->m_GeometryDirtyFloatRegisters[0] = 0xFFFFFFFF;
                InvalidateRegisterList(pProgram, s_RegisterList2);
            }
        }
        if ((pState->m_UpdatedStates & STATE_FRAGMENT_REGISTERS) != 0) {
            InvalidateRegisterList(pProgram, s_RegisterList3);
        }
        if ((pState->m_UpdatedStates & STATE_SHADER_REGISTERS) != 0) {
            InvalidateRegisterList(pProgram, s_RegisterList0);
        }
    }
    if ((pState->m_IgnoredStates & STATE_FLOAT_UNIFORMS) == 0) {
        PutAllFloatUniforms(pProgram);
    }
    if ((*pDirtyFlags & DIRTY_DEPTH) != 0 && (pState->m_IgnoredStates & STATE_DEPTH) == 0) {
        UpdateDepthRegisters(pProgram, pState, 0);
    }
    UpdateGasAccumulationMaximum(pProgram);

    // the registers of ignored states are not sent
    if (pState->m_IgnoredStates != 0) {
        if ((pState->m_IgnoredStates & STATE_FRAGMENT_REGISTERS) != 0) {
            for (u32 i = 0; i < REGISTER_SET_WORDS; i++) {
                pProgram->m_DirtyRegisters[i] &= ~s_RegisterSet2[i];
            }
        }
        if ((pState->m_IgnoredStates & STATE_SHADER_REGISTERS) != 0) {
            for (u32 i = 0; i < REGISTER_SET_WORDS; i++) {
                pProgram->m_DirtyRegisters[i] &= ~s_RegisterSet0[i];
            }
        }
        if ((pState->m_IgnoredStates & STATE_FLOAT_UNIFORMS) != 0) {
            for (u32 i = 0; i < REGISTER_SET_WORDS; i++) {
                pProgram->m_DirtyRegisters[i] &= ~s_RegisterSet1[i];
            }
        }
    }
    // with the lighting on but no light enabled light 0 is black and the lookup table configuration 0
    if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_ENABLE] & 1) != 0 && (pState->m_IgnoredStates & STATE_FRAGMENT_REGISTERS) == 0) {
        u32 i;
        for (i = 0; i < LIGHT_COUNT; i++) {
            if (IsLightEnabled(pProgram, i)) {
                break;
            }
        }
        if (i == LIGHT_COUNT) {
            ShaderTables* pTables = s_ShaderManager.m_pTables;
            pProgram->m_DirtyRegisters[2] &= ~0xF0000000;
            pTables->m_SentRegisterValues[REGISTER_LIGHT0_SPECULAR0] = 0;
            pTables->m_SentRegisterValues[REGISTER_LIGHT0_SPECULAR0 + 1] = 0;
            pTables->m_SentRegisterValues[REGISTER_LIGHT0_SPECULAR0 + 2] = 0;
            pTables->m_SentRegisterValues[REGISTER_LIGHT0_SPECULAR0 + 3] = 0;
            __cb_fillRegs(0x140, 4, 0);
            areLightsOff = true;
            u32 config = pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG0];
            if ((config & 0xF0) != 0) {
                pProgram->m_DirtyRegisters[REGISTER_LIGHTING_CONFIG0 >> 5] &= ~(1 << (REGISTER_LIGHTING_CONFIG0 & 31));
                s_ShaderManager.m_pTables->m_SentRegisterValues[REGISTER_LIGHTING_CONFIG0] = config & ~0xF0;
                PutCommand(config & ~0xF0, 0xF01C3);
            }
        }
    }

    // the changed registers (all bytes of consecutive registers in one command)
    u32 index = 0;
    for (u32 word = 0; word < REGISTER_SET_WORDS; word++) {
        u32 bits = pProgram->m_DirtyRegisters[word];
        if (bits != 0) {
            u32 bit = 0;
            while ((bits & (1 << bit)) == 0) {
                bit++;
            }
            index += bit;
            break;
        }
        index += 32;
    }
    if (index < SHADER_REGISTER_COUNT) {
        u32* pStart = nullptr;
        u32 first = 0;
        u32 run = 0;
        ShaderTables* pTables = s_ShaderManager.m_pTables;
        do {
            u8 mask = pProgram->m_RegisterMasks[index];
            u32 reg = s_ShaderRegisters[index];
            u32* pValue = &pProgram->m_RegisterValues[index];
            u32* pSent = &pTables->m_SentRegisterValues[index];
            if (mask != 0 && *pValue != *pSent) {
                if (mask != 0xF) {
                    if (pStart != nullptr) {
                        __cb_writeRegs(first, run, pStart);
                        pStart = nullptr;
                    }
                    PutCommand(*pValue, reg | (mask << 16));
                } else if (pStart != nullptr && reg - first == run) {
                    run++;
                } else {
                    if (pStart != nullptr) {
                        __cb_writeRegs(first, run, pStart);
                    }
                    pStart = pValue;
                    first = reg;
                    run = 1;
                }
                *pSent = *pValue;
            } else if (pStart != nullptr) {
                __cb_writeRegs(first, run, pStart);
                pStart = nullptr;
            }
            // the next dirty register
            index++;
            if (index >= SHADER_REGISTER_COUNT) {
                break;
            }
            if ((pProgram->m_DirtyRegisters[index >> 5] >> (index & 31)) != 0) {
                while (!IsBitSet(pProgram->m_DirtyRegisters, index)) {
                    index++;
                }
            } else {
                index = (index & ~31) + 32;
                for (u32 word = index >> 5; word < REGISTER_SET_WORDS; word++) {
                    u32 bits = pProgram->m_DirtyRegisters[word];
                    if (bits != 0) {
                        u32 bit = 0;
                        while ((bits & (1 << bit)) == 0) {
                            bit++;
                        }
                        index += bit;
                        break;
                    }
                    index += 32;
                }
            }
        } while (index < SHADER_REGISTER_COUNT);
        if (pStart != nullptr) {
            __cb_writeRegs(first, run, pStart);
        }
        for (u32 i = 0; i < REGISTER_SET_WORDS; i++) {
            pProgram->m_DirtyRegisters[i] = 0;
        }
    }
    if (areLightsOff) {
        // (sent again when a light is enabled)
        pProgram->m_DirtyRegisters[2] |= 0xF0000000;
        if ((pProgram->m_RegisterValues[REGISTER_LIGHTING_CONFIG0] & 0xF0) != 0) {
            pProgram->m_DirtyRegisters[REGISTER_LIGHTING_CONFIG0 >> 5] |= 1 << (REGISTER_LIGHTING_CONFIG0 & 31);
        }
    }

    if ((pState->m_IgnoredStates & STATE_LUTS) == 0) {
        PutLuts(pState, pProgram, *pDirtyFlags, &isLutUploaded, s_LutDisableBits, s_LutIds);
    }
    if ((pState->m_IgnoredStates & STATE_FRAGMENT_MODE) == 0 &&
        ((*pDirtyFlags & DIRTY_FRAGMENT_MODE) != 0 || pProgram->m_Mode != pState->m_LastFragmentMode)) {
        PutFragmentMode(pState, pProgram);
    }
}
} // extern "C"
