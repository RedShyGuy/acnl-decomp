#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gx/detail/gx_Shader.h"
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
