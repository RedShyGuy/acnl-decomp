#include "nn/gr/CTR/gr_Vertex.h"
#include <string.h>
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gr/CTR/gr_BindSymbol.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

namespace detail {
// 0x00975F44
u32 s_VertexBufferBase;
} // namespace detail

namespace {
// x, y, z, w of a fixed attribute with fewer components
// 0x0089E9E4
const f32 DEFAULT_CONST_ATTR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

inline u32 GetAttrByteSize(PicaDataVertexAttrType type)
{
    switch (type) {
    case 0:  // s8
    case 1:  // u8
        return 1;
    case 2:  // s16
    case 4:  // s8 x2
    case 5:  // u8 x2
        return 2;
    case 3:  // f32
    case 6:  // s16 x2
    case 12: // s8 x4
    case 13: // u8 x4
        return 4;
    case 7:  // f32 x2
    case 14: // s16 x4
        return 8;
    case 8:  // s8 x3
    case 9:  // u8 x3
        return 3;
    case 10: // s16 x3
        return 6;
    case 11: // f32 x3
        return 12;
    case 15: // f32 x4
        return 16;
    default:
        return 0;
    }
}
} // namespace

// 0x0034AC30 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::gr::CTR::Vertex::DisableAttr_(u32 index)
{
    if (!m_IsEnabled[index]) {
        return;
    }
    m_CommandCacheSize = 0;
    if (m_ConstAttrs[index].m_Dimension != 0) {
        m_ConstAttrs[index].m_Dimension = 0;
        m_IsEnabled[index] = false;
        return;
    }

    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        LoadArray& array = m_LoadArrays[i];
        if (array.m_PhysicalAddr == 0) {
            continue;
        }
        for (s32 j = 0; j < ATTRIBUTE_COUNT; ++j) {
            if (array.m_AttrIndex[j] != static_cast<s32>(index)) {
                continue;
            }
            array.m_AttrIndex[j] = -1;
            m_IsEnabled[index] = false;

            // release the array once it has no attribute left
            if (array.m_PhysicalAddr == 0) {
                return;
            }
            for (s32 k = 0; k < ATTRIBUTE_COUNT; ++k) {
                if (array.m_AttrIndex[k] != -1) {
                    return;
                }
            }
            array.m_PhysicalAddr = 0;
            for (s32 k = 0; k < ATTRIBUTE_COUNT; ++k) {
                array.m_ByteSizes[k] = 0;
            }
            return;
        }
    }
}

// 0x0034AD70 | tier C
void nn::gr::CTR::Vertex::EnableAttrAsArray(const BindSymbolVSInput& symbol, u32 physicalAddr, PicaDataVertexAttrType type)
{
    const u32 index = symbol.m_Start;
    const u32 size = GetAttrByteSize(type);
    DisableAttr_(index);

    LoadArray* array = NULL;
    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        if (m_LoadArrays[i].m_PhysicalAddr == 0) {
            array = &m_LoadArrays[i];
            break;
        }
    }
    m_IsEnabled[index] = true;
    array->m_PhysicalAddr = physicalAddr;
    array->m_Types[0] = type;
    array->m_AttrIndex[0] = index;
    array->m_ByteSizes[0] = size;
    for (s32 i = 1; i < ATTRIBUTE_COUNT; ++i) {
        array->m_ByteSizes[i] = 0;
        array->m_AttrIndex[i] = -1;
    }
    m_CommandCacheSize = 0;
}

// 0x0034AEA0 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::gr::CTR::Vertex::EnableAttrAsConst(const BindSymbolVSInput& symbol, u8 dimension, const f32* values)
{
    const u32 index = symbol.m_Start;
    DisableAttr_(index);
    m_IsEnabled[index] = true;
    ConstAttr& attr = m_ConstAttrs[index];
    attr.m_Dimension = dimension;
    attr.m_Value[0] = (dimension > 0) ? values[0] : DEFAULT_CONST_ATTR[0];
    attr.m_Value[1] = (dimension > 1) ? values[1] : DEFAULT_CONST_ATTR[1];
    attr.m_Value[2] = (dimension > 2) ? values[2] : DEFAULT_CONST_ATTR[2];
    attr.m_Value[3] = (dimension > 3) ? values[3] : DEFAULT_CONST_ATTR[3];
    m_CommandCacheSize = 0;
}

// 0x0034AF38 (name is ours)
void nn::gr::CTR::Vertex::EnableInterleavedArray(const InterleaveInfo& info, u32 physicalAddr)
{
    LoadArray* array = NULL;
    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        if (m_LoadArrays[i].m_PhysicalAddr == 0) {
            array = &m_LoadArrays[i];
            break;
        }
    }

    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        if (info.m_Symbols[i] == NULL) {
            array->m_AttrIndex[i] = -1;
        } else {
            DisableAttr_(info.m_Symbols[i]->m_Start);
            m_IsEnabled[info.m_Symbols[i]->m_Start] = true;
            array->m_AttrIndex[i] = info.m_Symbols[i]->m_Start;
            array->m_PhysicalAddr = physicalAddr;
        }
        const PicaDataVertexAttrType type = info.m_Types[i];
        array->m_Types[i] = type;
        array->m_ByteSizes[i] = (i < info.m_Count) ? GetAttrByteSize(type) : 0;
    }
    m_CommandCacheSize = 0;
}

// 0x0034B0A0 (name is ours)
nn::gr::CTR::Vertex::ConstAttr::ConstAttr() : m_Dimension(0)
{
}

// 0x0034AC1C (not in the symbols)
nn::gr::CTR::Vertex::IndexStream::IndexStream() : m_PhysicalAddr(0), m_DrawVertexCount(0), m_IsUnsignedByte(false)
{
}

// 0x0034B110 (not in the symbols)
nn::gr::CTR::Vertex::Vertex()
{
    m_CommandCacheSize = 0;
    memset(m_CommandCache, 0, sizeof(m_CommandCache));
    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        m_IsEnabled[i] = false;
        m_LoadArrays[i].DisableAll();
        m_ConstAttrs[i].m_Dimension = 0;
        m_ConstAttrs[i].m_Value[0] = 0.0f;
        m_ConstAttrs[i].m_Value[1] = 0.0f;
        m_ConstAttrs[i].m_Value[2] = 0.0f;
        m_ConstAttrs[i].m_Value[3] = 0.0f;
    }
}

// 0x00727B4C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
u32* nn::gr::CTR::Vertex::MakeDrawCommand(u32* command, const IndexStream& stream) const
{
    *command++ = 1;
    *command++ = CommandHeader(0x25F);
    *command++ = (stream.m_IsUnsignedByte ? 0 : 0x80000000) | (stream.m_PhysicalAddr - (detail::s_VertexBufferBase << 3));
    *command++ = CommandHeader(0x227);
    *command++ = stream.m_DrawVertexCount;
    *command++ = CommandHeader(0x228);
    *command++ = 0;
    *command++ = CommandHeader(0x245, 0x1);
    *command++ = 1;
    *command++ = CommandHeader(0x22F);
    *command++ = 1;
    *command++ = CommandHeader(0x245, 0x1);
    *command++ = 1;
    *command++ = CommandHeader(0x231);
    *command++ = 0;
    *command++ = CommandHeader(0x25E, 0x8);
    *command++ = 0;
    *command++ = CommandHeader(0x25E, 0x8);
    return command;
}

// 0x00727BEC | mk7dlp:callgraph [confirmed by fefates] [tier A]
u32* nn::gr::CTR::Vertex::MakeEnableAttrCommand_(u32* command) const
{
    u32* const attributeCount = &command[0];
    command[1] = CommandHeader(0x2B9, 0xB);
    u32* const inputCount = &command[2];
    command[3] = CommandHeader(0x242, 0x1);
    // the input permutation, low word at [4] and high word at [6]
    u32* const permutation = &command[4];
    command[4] = 0;
    command[5] = CommandHeader(0x2BB);
    command[6] = 0;
    command[7] = CommandHeader(0x2BC);
    const u32 base = detail::s_VertexBufferBase;
    command[8] = base;
    u32* const arrayHeader = &command[9];
    // the attribute formats; the upper word is only updated (original behavior)
    u32* const format = &command[10];
    command[10] = 0;
    command += 12;

    s32 arrayCount = 0;
    s32 attrCount = 0;
    for (s32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
        const LoadArray& array = m_LoadArrays[i];
        if (array.m_PhysicalAddr == 0) {
            continue;
        }
        ++arrayCount;
        u32 components[2] = { 0, 0 };
        u32 offset = 0;
        u32 alignment = 1;
        u32 componentCount = 0;
        for (s32 j = 0; j < ATTRIBUTE_COUNT; ++j) {
            switch (array.m_Types[j]) {
            case 2:
            case 6:
            case 10:
            case 14: // s16
                offset = (offset + 1) & ~1;
                if (alignment < 2) {
                    alignment = 2;
                }
                break;
            case 3:
            case 7:
            case 11:
            case 15: // f32
                offset = (offset + 3) & ~3;
                if (alignment < 4) {
                    alignment = 4;
                }
                break;
            default:
                break;
            }
            const u32 size = array.m_ByteSizes[j];
            if (size == 0) {
                break;
            }
            offset += size;
            if (array.m_AttrIndex[j] >= 0) {
                components[j / 8] |= attrCount << ((j % 8) * 4);
                format[attrCount / 8] |= array.m_Types[j] << ((attrCount % 8) * 4);
                u32& value = permutation[(attrCount / 8) * 2];
                value = (value & ~(0xF << ((attrCount % 8) * 4))) | (array.m_AttrIndex[j] << ((attrCount % 8) * 4));
                ++attrCount;
            } else {
                // padding of 4, 8, 12 or 16 bytes
                components[j / 8] |= (11 + (size >> 2)) << ((j % 8) * 4);
            }
            ++componentCount;
        }
        *command++ = array.m_PhysicalAddr - (base << 3);
        *command++ = components[0];
        *command++ = (((offset + alignment - 1) & ~(alignment - 1)) << 16) | (componentCount << 28) | components[1];
    }
    *arrayHeader = CommandHeader(0x200, 0xF, arrayCount * 3 + 2, true);
    if (arrayCount & 1) {
        *command++ = 0;
    }

    for (s32 i = ATTRIBUTE_COUNT - 1; i >= 0; --i) {
        const ConstAttr& attr = m_ConstAttrs[i];
        if (attr.m_Dimension == 0) {
            continue;
        }
        *command++ = attrCount;
        *command++ = CommandHeader(0x232, 0xF, 3, true);
        // four float24 values in three words, w first
        *command++ = ((Float32ToFloat24(attr.m_Value[2]) >> 16) & 0xFF) | (Float32ToFloat24(attr.m_Value[3]) << 8);
        *command++ = (Float32ToFloat24(attr.m_Value[2]) << 16) | ((Float32ToFloat24(attr.m_Value[1]) >> 8) & 0xFFFF);
        *command++ = (Float32ToFloat24(attr.m_Value[0]) & 0xFFFFFF) | (Float32ToFloat24(attr.m_Value[1]) << 24);
        *command++ = 0;
        u32& value = permutation[(attrCount / 8) * 2];
        value = (value & ~(0xF << ((attrCount % 8) * 4))) | (i << ((attrCount % 8) * 4));
        format[1] |= 1 << (16 + attrCount);
        ++attrCount;
    }

    *attributeCount = (attrCount - 1) | 0xA0000000;
    *inputCount = attrCount - 1;
    format[1] = (format[1] & 0x0FFFFFFF) | ((attrCount - 1) << 28);
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
