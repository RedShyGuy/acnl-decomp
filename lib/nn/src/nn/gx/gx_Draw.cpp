#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"

// The draw functions of the GL (glDrawArrays / glDrawElements): the state is validated, then the
// draw commands (the registers after 3dbrew). Without an array buffer the vertices go into the
// command buffer one by one as fixed attributes.

using namespace nn::gx::detail;

namespace {
// the draw modes (the OpenGL ES values; 0x6010 the geometry shader primitive, name ours)
const GLenum MODE_TRIANGLES = 4;
const GLenum MODE_TRIANGLE_STRIP = 5;
const GLenum MODE_TRIANGLE_FAN = 6;
const GLenum MODE_GEOMETRY_PRIMITIVE = 0x6010;
const GLenum CULL_FRONT_AND_BACK = 0x0408;
// the types of the vertex attributes (the others in gx_Surface.h)
const GLenum TYPE_BYTE = 0x1400;
const GLenum TYPE_SHORT = 0x1402;
const GLenum TYPE_FLOAT = 0x1406;
// the dirty flag of a geometry shader draw
const u32 DIRTY_GEOMETRY_PRIMITIVE = 0x80000;
// the commands
const u32 COMMAND_PRIMITIVE_CONFIG = 0x2025E;
const u32 COMMAND_PRIMITIVE_CONFIG_TOP = 0x8025E;
const u32 COMMAND_RESTART_PRIMITIVE = 0xF025F;
const u32 COMMAND_TEXTURE_CONFIG = 0x10080;
const u32 COMMAND_INDEX_BUFFER = 0xF0227;
const u32 COMMAND_VERTEX_COUNT = 0xF0228;
const u32 COMMAND_VERTEX_OFFSET = 0xF022A;
const u32 COMMAND_DRAW_ARRAYS = 0xF022E;
const u32 COMMAND_DRAW_ELEMENTS = 0xF022F;
const u32 COMMAND_CLEAR_POST_VERTEX_CACHE = 0xF0231;
const u32 COMMAND_FIXED_ATTRIBUTE_INDEX = 0xF0232;
const u32 COMMAND_START_DRAW = 0x10245;
const u32 COMMAND_DRAW_FLAG = 0x10253;
const u32 COMMAND_GEOMETRY_CONFIG = 0x20229;
const u32 COMMAND_GEOMETRY_DRAW_FLAG = 0x20253;
const u32 COMMAND_FRAMEBUFFER_FLUSH = 0xF0111;
const u32 COMMAND_VERTEX_ENTRY_END = 0xC02BA;
const u32 COMMAND_GEOMETRY_ENTRY_END = 0xC028A;
const u32 REGISTER_FIXED_ATTRIBUTE_DATA = 0x233;

inline u32 GetPrimitiveMode(GLenum mode)
{
    switch (mode) {
    case MODE_TRIANGLES:
        return 0x000;
    case MODE_TRIANGLE_STRIP:
        return 0x100;
    case MODE_TRIANGLE_FAN:
        return 0x200;
    default:
        return 0x300;
    }
}

// the vertices one by one as fixed attributes (index size: 0 none, 1 byte, 2 short)
// 0x007A10E8 (name is ours)
DECOMP_NOINLINE void PutImmediateVertices(u32 count, u32 indexSize, u32 firstOrIndices, bool isIndexed)
{
    GlState* pState = s_pGlState;
    PutCommand(15, COMMAND_FIXED_ATTRIBUTE_INDEX);
    PutCommand(1, COMMAND_DRAW_FLAG);
    PutCommand(0, COMMAND_START_DRAW);
    GLint sizes[16];
    GLenum types[16];
    for (u32 i = 0; i < pState->m_InputCount; i++) {
        const VertexAttribute* pAttribute = &pState->m_Attributes[pState->m_InputAttributes[i]];
        if (!pAttribute->m_IsEnabled) {
            sizes[i] = 4;
            types[i] = TYPE_FLOAT;
        } else {
            sizes[i] = pAttribute->m_Size;
            types[i] = pAttribute->m_Type;
        }
    }
    for (u32 vertex = 0;; vertex++) {
        u32 index = 0;
        if (!isIndexed) {
            index = firstOrIndices + vertex;
        } else if (indexSize == 1) {
            if (pState->m_ElementBufferName != 0) {
                u8 value;
                __mem_readMemory(reinterpret_cast<const void*>(firstOrIndices + vertex + pState->m_ArrayAddresses[0]), &value, 1);
                index = value;
            } else {
                index = reinterpret_cast<const u8*>(pState->m_ArraySources[0])[firstOrIndices + vertex];
            }
        } else if (indexSize == 2) {
            if (pState->m_ElementBufferName != 0) {
                u16 value;
                __mem_readMemory(reinterpret_cast<const void*>(firstOrIndices + vertex * 2 + pState->m_ArrayAddresses[0]), &value, 2);
                index = value;
            } else {
                index = *reinterpret_cast<const u16*>(pState->m_ArraySources[0] + firstOrIndices + vertex * 2);
            }
        }
        for (u32 i = 0; i < pState->m_InputCount; i++) {
            s32 attribute = pState->m_InputAttributes[i];
            const VertexAttribute* pAttribute = &pState->m_Attributes[attribute];
            const void* pData;
            u8 buffer[16];
            if (pAttribute->m_BufferName != 0 && pAttribute->m_IsEnabled) {
                __mem_readMemory(reinterpret_cast<const void*>(pState->m_ArrayAddresses[1 + attribute] + pState->m_ArrayStrides[attribute] * index), buffer,
                                 16);
                pData = buffer;
            } else {
                pData = reinterpret_cast<const void*>(pState->m_ArraySources[1 + attribute] + pState->m_ArrayStrides[attribute] * index);
            }
            f32 values[4];
            switch (types[i]) {
            case TYPE_FLOAT:
                for (u32 j = 0; j < 4; j++) {
                    values[j] = static_cast<const f32*>(pData)[j];
                }
                break;
            case TYPE_SHORT:
                for (u32 j = 0; j < 4; j++) {
                    values[j] = static_cast<f32>(static_cast<const s16*>(pData)[j]);
                }
                break;
            case TYPE_BYTE:
                for (u32 j = 0; j < 4; j++) {
                    values[j] = static_cast<f32>(static_cast<const s8*>(pData)[j]);
                }
                break;
            case TYPE_UNSIGNED_BYTE:
                for (u32 j = 0; j < 4; j++) {
                    values[j] = static_cast<f32>(static_cast<const u8*>(pData)[j]);
                }
                break;
            default:
                break;
            }
            // the missing components are 0, 0, 1
            u32 floats[4];
            GLint size = sizes[i];
            if (size == 1) {
                floats[1] = 0;
            }
            if (size == 1 || size == 2) {
                floats[2] = 0;
            }
            if (size == 1 || size == 2 || size == 3) {
                floats[3] = 0x3F0000;
            }
            for (s32 j = 0; j < size; j++) {
                floats[j] = Float32ToFloat24(values[j]);
            }
            u32 packed[3];
            packed[0] = (floats[2] >> 16) | (floats[3] << 8);
            packed[1] = ((floats[1] << 8) >> 16) | (floats[2] << 16);
            packed[2] = floats[0] | (floats[1] << 24);
            __cb_writeRegs(REGISTER_FIXED_ATTRIBUTE_DATA, 3, packed);
        }
        if (vertex >= count - 1) {
            break;
        }
    }
    PutCommand(1, COMMAND_START_DRAW);
    PutCommand(1, COMMAND_CLEAR_POST_VERTEX_CACHE);
    PutCommand(0, COMMAND_DRAW_FLAG);
    PutCommand(1, COMMAND_FRAMEBUFFER_FLUSH);
}

// the primitive mode of the geometry shader for triangles (the state flag of glEnable 0x6502)
inline void PutTriangleGeometryReset()
{
    PutCommand(0, COMMAND_GEOMETRY_CONFIG);
    PutCommand(0, COMMAND_GEOMETRY_DRAW_FLAG);
    PutCommand(0, COMMAND_PRIMITIVE_CONFIG);
}

inline bool IsDrawMode(GLenum mode)
{
    return mode == MODE_TRIANGLES || mode == MODE_TRIANGLE_STRIP || mode == MODE_TRIANGLE_FAN;
}
} // namespace

// 0x007A980C | nintendogs:bytes [tier B]
extern "C" void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    GlState* pState = s_pGlState;
    if (!IsDrawMode(mode)) {
        if (mode != MODE_GEOMETRY_PRIMITIVE) {
            return;
        }
        pState->m_DirtyFlags |= DIRTY_GEOMETRY_PRIMITIVE;
    }
    GLenum previous = pState->m_DrawMode;
    pState->m_DrawMode = mode;
    pState->m_IsValidating = true;
    pState->m_PreviousDrawMode = previous;
    pState->m_IsFullValidation = true;
    __vldtr_validateAllState();
    if (pState->m_IsCullFaceEnabled == true && pState->m_CullFaceMode == CULL_FRONT_AND_BACK) {
        return;
    }
    if (count == 0) {
        return;
    }
    __shv_preDraw();
    PutCommand(GetPrimitiveMode(mode), COMMAND_PRIMITIVE_CONFIG);
    PutCommand(1, COMMAND_RESTART_PRIMITIVE);
    u32 config = pState->m_TextureRegisters[0] & 7;
    if (config != 0) {
        PutCommand(config, COMMAND_TEXTURE_CONFIG);
    }
    if (pState->m_Unknown0x81C) {
        if ((pState->m_IgnoredStates & STATE_VERTEX) == 0) {
            u32 offset = (count > 16 ? count * 2 - 32 : 0) + pState->m_ArrayBaseAddress;
            PutCommand((offset & 0xFFF) >= 0xFE0 ? 0x80000020 : 0x80000000, COMMAND_INDEX_BUFFER);
        }
        PutCommand(1, COMMAND_DRAW_FLAG);
        PutCommand(count, COMMAND_VERTEX_COUNT);
        PutCommand(first, COMMAND_VERTEX_OFFSET);
        PutCommand(0, COMMAND_START_DRAW);
        PutCommand(1, COMMAND_DRAW_ARRAYS);
        PutCommand(1, COMMAND_START_DRAW);
        PutCommand(1, COMMAND_CLEAR_POST_VERTEX_CACHE);
        PutCommand(0, COMMAND_PRIMITIVE_CONFIG_TOP);
        PutCommand(0, COMMAND_PRIMITIVE_CONFIG_TOP);
        PutCommand(0, COMMAND_DRAW_FLAG);
        PutCommand(1, COMMAND_FRAMEBUFFER_FLUSH);
    } else {
        PutImmediateVertices(count, 2, first, false);
    }
    if (config != 0) {
        PutCommand(0, COMMAND_TEXTURE_CONFIG);
    }
    PutCommand(0x7FFF0000, COMMAND_VERTEX_ENTRY_END);
    if (mode == MODE_GEOMETRY_PRIMITIVE) {
        PutCommand(0x7FFF0000, COMMAND_GEOMETRY_ENTRY_END);
    }
    pState->m_IsValidating = false;
    pState->m_IsFullValidation = false;
    if (pState->m_UpdatedStates != 0) {
        pState->m_UpdatedStates = 0;
    }
}

// 0x007A9C98 (name after the OpenGL ES API)
extern "C" void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* pIndices)
{
    GlState* pState = s_pGlState;
    if (!IsDrawMode(mode)) {
        if (mode != MODE_GEOMETRY_PRIMITIVE) {
            return;
        }
        pState->m_DirtyFlags |= DIRTY_GEOMETRY_PRIMITIVE;
    }
    bool isShort;
    if (type == TYPE_UNSIGNED_BYTE) {
        isShort = false;
    } else if (type == TYPE_UNSIGNED_SHORT) {
        isShort = true;
    } else {
        return;
    }
    pState->m_PreviousDrawMode = pState->m_DrawMode;
    pState->m_DrawMode = mode;
    pState->m_IsValidating = true;
    __vldtr_validateAllState();
    if (pState->m_IsCullFaceEnabled == true && pState->m_CullFaceMode == CULL_FRONT_AND_BACK) {
        return;
    }
    if (count == 0) {
        return;
    }
    __shv_preDraw();
    if (mode == MODE_TRIANGLES) {
        if (pState->m_Unknown0x1A) {
            PutCommand(0x100, COMMAND_GEOMETRY_CONFIG);
            PutCommand(0x100, COMMAND_GEOMETRY_DRAW_FLAG);
            PutCommand(0x300, COMMAND_PRIMITIVE_CONFIG);
        } else {
            PutCommand(0, COMMAND_PRIMITIVE_CONFIG);
        }
    } else {
        PutCommand(GetPrimitiveMode(mode), COMMAND_PRIMITIVE_CONFIG);
    }
    PutCommand(1, COMMAND_RESTART_PRIMITIVE);
    u32 config = pState->m_TextureRegisters[0] & 7;
    if (config != 0) {
        PutCommand(config, COMMAND_TEXTURE_CONFIG);
    }
    if (pState->m_Unknown0x81C) {
        if ((pState->m_IgnoredStates & STATE_VERTEX) == 0) {
            u32 address = reinterpret_cast<u32>(pIndices) + (pState->m_ArrayAddresses[0] - pState->m_ArrayBaseAddress);
            PutCommand((address & ~0xF0000000) | (static_cast<u32>(isShort) << 31), COMMAND_INDEX_BUFFER);
        }
        PutCommand(count, COMMAND_VERTEX_COUNT);
        PutCommand(0, COMMAND_START_DRAW);
        PutCommand(1, COMMAND_DRAW_ELEMENTS);
        PutCommand(1, COMMAND_START_DRAW);
        PutCommand(1, COMMAND_CLEAR_POST_VERTEX_CACHE);
        PutCommand(0, COMMAND_PRIMITIVE_CONFIG_TOP);
        PutCommand(0, COMMAND_PRIMITIVE_CONFIG_TOP);
        PutCommand(1, COMMAND_FRAMEBUFFER_FLUSH);
    } else {
        if (mode == MODE_TRIANGLES && pState->m_Unknown0x1A) {
            PutTriangleGeometryReset();
        }
        PutImmediateVertices(count, isShort + 1, reinterpret_cast<u32>(pIndices), true);
    }
    if (mode == MODE_TRIANGLES && pState->m_Unknown0x1A) {
        PutTriangleGeometryReset();
    }
    if (config != 0) {
        PutCommand(0, COMMAND_TEXTURE_CONFIG);
    }
    PutCommand(0x7FFF0000, COMMAND_VERTEX_ENTRY_END);
    if (mode == MODE_GEOMETRY_PRIMITIVE) {
        PutCommand(0x7FFF0000, COMMAND_GEOMETRY_ENTRY_END);
    }
    pState->m_IsValidating = false;
    if (pState->m_UpdatedStates != 0) {
        pState->m_UpdatedStates = 0;
    }
}
