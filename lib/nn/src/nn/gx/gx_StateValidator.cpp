#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"
#include "nn/gx/detail/gx_Shader.h"
#include "nn/gxlow/CTR/CTR_Api.h"

// The state validator of the GL state (__vldtr_*) and the small validators: the framebuffer
// validator mask (__fbv_initializeFBValidator), the vertex buffer validator (__vbv_*) and the draw
// check of the shader validator (__shv_preDraw).

using namespace nn::gx::detail;

namespace {
// the types of the vertex attributes (the OpenGL ES values)
const GLenum TYPE_BYTE = 0x1400;
const GLenum TYPE_SHORT = 0x1402;
const GLenum TYPE_FLOAT = 0x1406;
// the index of the element array in the vertex arrays (after the attributes)
const u32 ELEMENT_ARRAY = VERTEX_ATTRIBUTE_COUNT;
// the dirty flags the framebuffer and vertex buffer validators handle
const u32 DIRTY_FRAMEBUFFER_MASK = 0x1;
const u32 DIRTY_VERTEX_BUFFER_MASK = 0x42;
// the program mode for which the gas commands are finished at the draw (name is ours;
// __sys_markGasAcmMax follows it)
const GLenum PROGRAM_MODE_GAS = 0x6051;

inline s32 GetTypeSize(GLenum type)
{
    switch (type) {
    case TYPE_BYTE:
        return 1;
    case TYPE_SHORT:
        return 2;
    case TYPE_FLOAT:
        return 4;
    default:
        return 0;
    }
}

inline GLsizei GetStride(const VertexAttribute* pAttribute)
{
    s32 typeSize = GetTypeSize(pAttribute->m_Type);
    return pAttribute->m_Stride != 0 ? pAttribute->m_Stride : pAttribute->m_Size * typeSize;
}

// an array in a buffer object
inline void SetBufferArray(GlState* pState, u32 index, const BufferData* pData)
{
    if (index == ELEMENT_ARRAY) {
        pState->m_ArrayAddresses[0] = nngxlowGetPhysicalAddr(pData->m_Address);
        pState->m_ArraySources[0] = pData->m_Source;
        return;
    }
    const VertexAttribute* pAttribute = &pState->m_Attributes[index];
    u32 offset = reinterpret_cast<u32>(pAttribute->m_pPointer);
    pState->m_ArrayAddresses[1 + index] = nngxlowGetPhysicalAddr(pData->m_Address + offset);
    pState->m_ArraySources[1 + index] = offset + pData->m_Source;
    pState->m_ArrayStrides[index] = GetStride(pAttribute);
}

// an array in the memory of the program (without a buffer object)
// 0x0013A284 (name is ours)
DECOMP_NOINLINE void SetClientArray(u32 index)
{
    GlState* pState = s_pGlState;
    if (index == ELEMENT_ARRAY) {
        pState->m_ArrayAddresses[0] = 0;
        pState->m_ArraySources[0] = 0;
        return;
    }
    const VertexAttribute* pAttribute = &pState->m_Attributes[index];
    u32 address = reinterpret_cast<u32>(pAttribute->m_pPointer);
    pState->m_ArrayAddresses[1 + index] = address;
    pState->m_ArraySources[1 + index] = address;
    pState->m_ArrayStrides[index] = GetStride(pAttribute);
}
} // namespace

extern "C" {
// 0x00127D04 | nintendogs:bytes [tier B]
int __vldtr_initializeStateValidator(StateValidator* pValidator)
{
    s_pStateValidator = pValidator;
    __txv_initializeTextureValidator(&s_pStateValidator->m_TextureMask);
    __fbv_initializeFBValidator(&s_pStateValidator->m_FramebufferMask);
    __vbv_initializeVBValidator(&s_pStateValidator->m_VertexBufferMask);
    __shv_initializeShaderValidator(&s_pStateValidator->m_ShaderMask);
    return 0;
}

// (the viewport first, then each validator whose dirty flags are set; the flags are cleared)
// 0x007A1038 | nintendogs:bytes [tier B]
void __vldtr_validateAllState()
{
    GlState* pState = s_pGlState;
    if ((pState->m_UpdatedStates & STATE_VIEWPORT) != 0) {
        ValidateViewport();
    }
    if ((pState->m_DirtyFlags & s_pStateValidator->m_TextureMask) != 0) {
        __txv_validateTextureValidator(&pState->m_DirtyFlags);
    }
    if ((pState->m_DirtyFlags & s_pStateValidator->m_FramebufferMask) != 0) {
        __fbv_validateFBValidator(&pState->m_DirtyFlags);
    }
    if ((pState->m_DirtyFlags & s_pStateValidator->m_VertexBufferMask) != 0) {
        __vbv_validateVBValidator(&pState->m_DirtyFlags);
    }
    if ((pState->m_DirtyFlags & s_pStateValidator->m_ShaderMask) != 0) {
        __shv_validateShaderValidator(&pState->m_DirtyFlags);
    }
    pState->m_DirtyFlags = 0;
}

// 0x0012E3D4 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void __fbv_initializeFBValidator(u32* pDirtyMask)
{
    *pDirtyMask = DIRTY_FRAMEBUFFER_MASK;
}

// (the arrays are set up right away; the mask goes as the dirty flags, which that validator does not
// read)
// 0x001356E0 | nintendogs:bytes [tier A]
DECOMP_NOINLINE void __vbv_initializeVBValidator(u32* pDirtyMask)
{
    *pDirtyMask = DIRTY_VERTEX_BUFFER_MASK;
    __vbv_validateVBValidator(pDirtyMask);
}

// the addresses and strides of the vertex arrays
// 0x001356EC | nintendogs:bytes [tier A]
void __vbv_validateVBValidator(const u32* pDirtyFlags)
{
    GlState* pState = s_pGlState;
    if ((pState->m_IgnoredStates & STATE_VERTEX) != 0) {
        pState->m_IsVertexBufferUsable = pState->m_ArrayBufferName != 0;
    }
    pState->m_IsArrayMissing = false;
    if (pState->m_ElementBufferName != 0) {
        BufferObject* pObject = s_BufferManager.m_pTables->m_pElementBuffer;
        if (pObject != nullptr) {
            SetBufferArray(pState, ELEMENT_ARRAY, static_cast<BufferData*>(pObject->m_pData));
        } else {
            pState->m_IsArrayMissing = true;
            SetClientArray(ELEMENT_ARRAY);
        }
    } else {
        SetClientArray(ELEMENT_ARRAY);
    }
    for (u32 i = 0; i < VERTEX_ATTRIBUTE_COUNT; i++) {
        VertexAttribute* pAttribute = &pState->m_Attributes[i];
        if (!pAttribute->m_IsEnabled) {
            // the constant value
            pState->m_ArraySources[1 + i] = reinterpret_cast<u32>(pState->m_AttributeValues.m_Values[i]);
            pState->m_ArrayStrides[i] = 0;
            continue;
        }
        if (pAttribute->m_BufferName == 0) {
            SetClientArray(i);
            continue;
        }
        BufferObject* pObject = s_BufferManager.m_pTables->m_pAttributeBuffers[i];
        if (pObject != nullptr) {
            SetBufferArray(s_pGlState, i, static_cast<BufferData*>(pObject->m_pData));
        } else {
            pState->m_IsArrayMissing = true;
            SetClientArray(i);
        }
    }
}

// the gas commands of the previous program are finished when the mode changes
// 0x007802CC | nintendogs:bytes [tier B]
void __shv_preDraw()
{
    Program* pProgram = s_ShaderManager.m_pTables->m_pCurrentProgram;
    if (pProgram == nullptr) {
        return;
    }
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    if (pProgram->m_Mode == PROGRAM_MODE_GAS) {
        if (pTables->m_LastMode != PROGRAM_MODE_GAS) {
            PutCommand(0, 0xF0125);
        }
    } else if (pTables->m_LastMode == PROGRAM_MODE_GAS && pProgram->m_IsGasAutoAcc) {
        __sys_markGasAcmMax();
    }
    s_ShaderManager.m_pTables->m_LastMode = s_ShaderManager.m_pTables->m_pCurrentProgram->m_Mode;
}
} // extern "C"
