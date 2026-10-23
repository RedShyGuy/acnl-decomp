#include "nw/gfx/gfx_ParticleShape.h"

#include <cstring>
#include <new>

#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/gfx_ParticleSet.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace gfx {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | ((extraCount & 0xFF) << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_FRAMEBUFFER_INVALIDATE = 0x110;
const u32 REG_FRAMEBUFFER_FLUSH = 0x111;
const u32 REG_ATTRIBBUFFERS_LOC = 0x200;
const u32 REG_ATTRIBBUFFER0_CONFIG2 = 0x205;
const u32 REG_GEOSTAGE_CONFIG = 0x229;
const u32 REG_DRAWELEMENTS = 0x22F;
const u32 REG_VTX_FUNC = 0x231;
const u32 REG_FIXEDATTRIB_INDEX = 0x232;
const u32 REG_VSH_NUM_ATTR = 0x242;
const u32 REG_START_DRAW_FUNC0 = 0x245;
const u32 REG_GEOSTAGE_CONFIG2 = 0x253;
const u32 REG_PRIMITIVE_CONFIG = 0x25E;
const u32 REG_RESTART_PRIMITIVE = 0x25F;
const u32 REG_GSH_ATTRIBUTES_PERMUTATION_LOW = 0x28B;
const u32 REG_VSH_BOOLUNIFORM = 0x2B0;
const u32 REG_VSH_INPUTBUFFER_CONFIG = 0x2B9;
const u32 REG_VSH_ATTRIBUTES_PERMUTATION_LOW = 0x2BB;

// the GL enums used here (names after the OpenGL ES API)
namespace gl {
const u32 BYTE = 0x1400;
const u32 UNSIGNED_BYTE = 0x1401;
const u32 SHORT = 0x1402;
const u32 FLOAT = 0x1406;
} // namespace gl

// the memory area the vertex buffers count from
const u32 MEMORY_AREA_VRAM_A = 0x20000;

// the room of the command caches in m_CommandBuffer
const u32 ACTIVATE_COMMAND_CAPACITY = 544;
const u32 DEACTIVATE_COMMAND_CAPACITY = 384;

// the extra values in a stream (beyond the particles)
const int STREAM_EXTRA_VALUES = 8;

// (inline) the bytes of a value of the type as AddVertexParam/AddVertexStream store it
inline int GetTypeSize(u32 type)
{
    return type == gl::FLOAT ? 4 : 1;
}

// (inline) the type of an attribute in ATTRIBBUFFERS_FORMAT
inline u32 GetFormatType(u32 type)
{
    switch (type) {
    case gl::BYTE:
        return 0;
    case gl::UNSIGNED_BYTE:
        return 1;
    case gl::SHORT:
        return 2;
    case gl::FLOAT:
        return 3;
    default:
        return 0;
    }
}

// (inline) the bytes of a value of an attribute in a vertex buffer
inline u32 GetStride(u32 type, u32 dimension)
{
    switch (type) {
    case gl::BYTE:
    case gl::UNSIGNED_BYTE:
        return dimension;
    case gl::SHORT:
        return dimension * 2;
    case gl::FLOAT:
        return dimension * 4;
    default:
        return 0;
    }
}

// (inline) the bool uniforms b1-b4 of the vertex shader for how a set is drawn (the original leaves
// the value undefined for other render styles)
inline u32 GetRenderStyleBools(u32 typeInfo)
{
    u32 bools = 0;
    switch (typeInfo) {
    case 0x80000000:
        bools = 0;
        break;
    case 0x10000000:
        bools = 5;
        break;
    case 0x20000000:
        bools = 3;
        break;
    case 0x40000000:
        bools = 8;
        break;
    }
    return bools;
}
} // namespace

// 0x0097F48C
const TypeInfo nw::gfx::ParticleShape::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C200 (static initializer)

// 0x00494EC8 | nintendogs:bytes [tier A]
nw::gfx::ParticleShape::VertexAttribute* nw::gfx::ParticleShape::AddVertexParam(int usage, u32 type, int dimension, f32* pValues, u8** ppMemory)
{
    const int size = dimension * GetTypeSize(type);
    u8* pBuffer = ut::RoundUp(*ppMemory, 32);
    *ppMemory = pBuffer + size;
    nnnstdMemCpy(pBuffer, pValues, size);
    m_Attributes[m_AttributeCount].usage = usage;
    m_Attributes[m_AttributeCount].type = type;
    m_Attributes[m_AttributeCount].dimension = dimension;
    m_Attributes[m_AttributeCount].isStream = false;
    m_Attributes[m_AttributeCount].pBuffers[0] = pBuffer;
    m_Attributes[m_AttributeCount].pBuffers[1] = nullptr;
    m_Attributes[m_AttributeCount].pCommandValues[0] = nullptr;
    m_Attributes[m_AttributeCount].pCommandValues[1] = nullptr;
    ++m_AttributeCount;
    return &m_Attributes[m_AttributeCount - 1];
}

// 0x00494FB8 | nintendogs:bytes [tier A]
nw::gfx::ParticleShape::VertexAttribute* nw::gfx::ParticleShape::AddVertexStream(int usage, u32 type, int dimension, int maxParticles,
                                                                                 u8** ppMemory)
{
    const int size = (maxParticles + STREAM_EXTRA_VALUES) * dimension * GetTypeSize(type);
    u8* pFront = ut::RoundUp(*ppMemory, 32);
    u8* pBack = pFront + size;
    *ppMemory = pBack + size;
    std::memset(pFront, 0, size);
    std::memset(pBack, 0, size);
    m_Attributes[m_AttributeCount].usage = usage;
    m_Attributes[m_AttributeCount].type = type;
    m_Attributes[m_AttributeCount].dimension = dimension;
    m_Attributes[m_AttributeCount].isStream = true;
    m_Attributes[m_AttributeCount].pBuffers[0] = pFront;
    m_Attributes[m_AttributeCount].pBuffers[1] = pBack;
    m_Attributes[m_AttributeCount].pCommandValues[0] = nullptr;
    m_Attributes[m_AttributeCount].pCommandValues[1] = nullptr;
    ++m_AttributeCount;
    return &m_Attributes[m_AttributeCount - 1];
}

// 0x004950C4 | nintendogs:bytes [tier A]
int nw::gfx::ParticleShape::AddVertexParamSize(u32 type, int dimension, int size)
{
    return ut::RoundUp(size, 32) + dimension * GetTypeSize(type);
}

// 0x004950E8 | nintendogs:callseq [tier A]
void nw::gfx::ParticleShape::CreateCommandCache(ParticleSet* pParticleSet)
{
    // the activate commands of both sides: the attribute order and formats, a vertex buffer per
    // stream, then the constant values
    for (int i = 0; i < 2; ++i) {
        const u8 side = i;
        const u32 vramA = nngxGetPhysicalAddr(nngxGetVramStartAddr(MEMORY_AREA_VRAM_A));
        u32* pCommand = m_pActivateCommands[side];
        const int attributeCount = m_AttributeCount;
        pCommand[0] = 0xA0000000 | (attributeCount - 1);
        pCommand[1] = Header(REG_VSH_INPUTBUFFER_CONFIG, 0xB);
        pCommand[2] = attributeCount - 1;
        pCommand[3] = Header(REG_VSH_NUM_ATTR, 0x1);
        pCommand[4] = 0;
        pCommand[5] = Header(REG_VSH_ATTRIBUTES_PERMUTATION_LOW, 0xF, 1, true);
        pCommand[6] = 0;
        pCommand[7] = 0;
        pCommand[8] = vramA >> 3;
        pCommand[9] = Header(REG_ATTRIBBUFFERS_LOC, 0xF, 3 * attributeCount + 2, true);
        pCommand[10] = 0;
        pCommand[11] = (attributeCount - 1) << 28;
        u32* pPermutation = &pCommand[4]; // low and high with the header between them
        u32* pFormat = &pCommand[10];
        int index = 12;
        int attribute = 0;

        for (int j = 0; j < m_AttributeCount; ++j) {
            VertexAttribute& vertexAttribute = m_Attributes[j];
            if (!vertexAttribute.isStream) {
                continue;
            }
            pPermutation[attribute / 8 * 2] |= (vertexAttribute.usage & 0xF) << (attribute % 8 * 4);
            pFormat[attribute / 8] |= (GetFormatType(vertexAttribute.type) + (vertexAttribute.dimension - 1) * 4) << (attribute % 8 * 4);
            const u32 address = nngxGetPhysicalAddr(reinterpret_cast<uptr>(GetStreamBuffer(j, side)));
            vertexAttribute.pCommandValues[side] = &pCommand[index];
            pCommand[index] = address - vramA;
            pCommand[index + 1] = attribute;
            pCommand[index + 2] = 0x10000000 + (GetStride(vertexAttribute.type, vertexAttribute.dimension) << 16);
            ++attribute;
            index += 3;
        }
        // no buffers for the constant attributes
        const int constantCount = attributeCount - attribute;
        for (int j = 0; j < constantCount * 3; ++j) {
            pCommand[index + j] = 0;
        }
        index += constantCount * 3;
        if (index & 1) {
            pCommand[index++] = 0;
        }

        for (int j = 0; j < m_AttributeCount; ++j) {
            VertexAttribute& vertexAttribute = m_Attributes[j];
            if (vertexAttribute.isStream) {
                continue;
            }
            pPermutation[attribute / 8 * 2] |= (vertexAttribute.usage & 0xF) << (attribute % 8 * 4);
            u32 values[4] = {0, 0, 0, 0};
            const f32* pValues = static_cast<const f32*>(GetConstantBuffer(j));
            for (int k = 0; k < vertexAttribute.dimension; ++k) {
                values[k] = internal::Float32ToFloat24Clamped(pValues[k]);
            }
            vertexAttribute.pCommandValues[side] = &pCommand[index + 2];
            pCommand[index] = attribute;
            pCommand[index + 1] = Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true);
            pCommand[index + 2] = (values[3] << 8) | (values[2] >> 16);
            pCommand[index + 3] = (values[2] << 16) | (values[1] >> 8);
            pCommand[index + 4] = values[0] | (values[1] << 24);
            pCommand[index + 5] = 0;
            pFormat[1] |= 1 << (attribute + 16); // a fixed attribute
            ++attribute;
            index += 6;
        }
        pCommand[index] = 0x76543210;
        pCommand[index + 1] = Header(REG_GSH_ATTRIBUTES_PERMUTATION_LOW, 0xF, 1, true);
        pCommand[index + 2] = 0xFEDCBA98;
        pCommand[index + 3] = 0;
        m_ActivateCommandSizes[side] = (index + 4) * sizeof(u32);
    }

    // the deactivate command: the vertex buffers off, the fixed values 0
    u32* pCommand = m_pDeactivateCommand;
    int streamCount = 0;
    int index = 0;
    for (int i = 0; i < m_AttributeCount; ++i) {
        if (m_Attributes[i].isStream) {
            pCommand[index++] = 0;
            pCommand[index++] = Header(static_cast<u16>(REG_ATTRIBBUFFER0_CONFIG2 + 3 * streamCount));
            ++streamCount;
        }
    }
    for (int i = 0; i < streamCount; ++i) {
        pCommand[index] = i;
        pCommand[index + 1] = Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true);
        pCommand[index + 2] = 0;
        pCommand[index + 3] = 0;
        pCommand[index + 4] = 0;
        pCommand[index + 5] = 0;
        index += 6;
    }
    m_DeactivateCommandSize = index * sizeof(u32);

    // the primitive command: the shader mode of the render style, then the drawing of points as
    // geometry primitives
    const res::ResParticleSetData* pResource =
        res::DynamicCastData<res::ResParticleSetData>(pParticleSet->GetResSceneObject().ptr(), res::ResParticleSetData::TYPE);
    const u32 bools = GetRenderStyleBools(pResource->renderStyle.To<res::ResParticleRenderStyleData>()->typeInfo);
    pCommand = m_pPrimitiveCommand;
    pCommand[0] = 0x7FFF0000 | (bools << 1);
    pCommand[1] = Header(REG_VSH_BOOLUNIFORM, 0xF, 4, true);
    pCommand[2] = 0;
    pCommand[3] = 0;
    pCommand[4] = 0;
    pCommand[5] = 0;
    pCommand[6] = 0;
    pCommand[7] = Header(REG_GEOSTAGE_CONFIG, 0x2);
    pCommand[8] = 0;
    pCommand[9] = Header(REG_GEOSTAGE_CONFIG2, 0x2);
    pCommand[10] = 0x300;
    pCommand[11] = Header(REG_PRIMITIVE_CONFIG, 0x2);
    pCommand[12] = 1;
    pCommand[13] = Header(REG_RESTART_PRIMITIVE);
    pCommand[14] = 0;
    pCommand[15] = Header(REG_START_DRAW_FUNC0, 0x1);
    pCommand[16] = 1;
    pCommand[17] = Header(REG_DRAWELEMENTS);
    pCommand[18] = 1;
    pCommand[19] = Header(REG_START_DRAW_FUNC0, 0x1);
    pCommand[20] = 1;
    pCommand[21] = Header(REG_VTX_FUNC);
    pCommand[22] = 0;
    pCommand[23] = Header(REG_PRIMITIVE_CONFIG, 0x8);
    pCommand[24] = 0;
    pCommand[25] = Header(REG_PRIMITIVE_CONFIG, 0x8);
    pCommand[26] = 1;
    pCommand[27] = Header(REG_FRAMEBUFFER_FLUSH);
    pCommand[28] = 1;
    pCommand[29] = Header(REG_FRAMEBUFFER_INVALIDATE);
    m_PrimitiveCommandSize = 30 * sizeof(u32);
}

// 0x00495768 | nintendogs:bytes [tier A]
int nw::gfx::ParticleShape::AddVertexStreamSize(u32 type, int dimension, int maxParticles, int size)
{
    const int bufferSize = (maxParticles + STREAM_EXTRA_VALUES) * dimension * GetTypeSize(type);
    size = ut::RoundUp(size, 32) + bufferSize;
    return ut::RoundUp(size, 32) + bufferSize;
}

// 0x004957A4 | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, int /*maxParticles*/)
{
    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(32);
    pSize->Add(sizeof(ParticleShape));
    pSize->SetAlignment(alignment);
}

// 0x004957EC | nintendogs:bytes [tier A]
void nw::gfx::ParticleShape::GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, int maxParticles)
{
    const u32 bufferSize = GetIndexBufferSize(maxParticles);
    const u32 alignment = pSize->GetAlignment();
    // with room to align the first buffer in memory of a smaller alignment
    u32 size = alignment < 32 ? 32 - alignment : 0;
    size = ut::RoundUp(size, 32) + bufferSize;
    size = ut::RoundUp(size, 32) + bufferSize;
    pSize->SetAlignment(32);
    pSize->Add(size);
    pSize->SetAlignment(alignment);
}

// 0x00495860 | nintendogs:bytes [tier A]
nw::gfx::ParticleShape* nw::gfx::ParticleShape::Create(res::ResSceneObject resObject, int maxParticles, os::IAllocator* pAllocator,
                                                       os::IAllocator* pDeviceAllocator)
{
    const res::ResParticleShapeData* pResource = res::DynamicCastData<res::ResParticleShapeData>(resObject.ptr(), res::ResParticleShapeData::TYPE);
    const u32 bufferSize = GetIndexBufferSize(maxParticles);
    u8* pDeviceMemory = static_cast<u8*>(pDeviceAllocator->Alloc(ut::RoundUp(bufferSize, 32) + bufferSize, 32));
    if (pDeviceMemory == nullptr) {
        return nullptr;
    }
    void* pMemory = pAllocator->Alloc(sizeof(ParticleShape), 32);
    if (pMemory == nullptr) {
        pDeviceAllocator->Free(pDeviceMemory);
        return nullptr;
    }
    ParticleShape* pShape = new (pMemory) ParticleShape(pAllocator, res::ResSceneObject(pResource), maxParticles, pDeviceAllocator, pDeviceMemory);

    u8* pBuffer = pDeviceMemory;
    for (int i = 0; i < 2; ++i) {
        pShape->m_pIndexBuffers[i] = ut::RoundUp(pBuffer, 32);
        pBuffer = static_cast<u8*>(pShape->m_pIndexBuffers[i]) + bufferSize;
        const u32 vramA = nngxGetPhysicalAddr(nngxGetVramStartAddr(MEMORY_AREA_VRAM_A));
        pShape->m_IndexBufferOffsets[i] = nngxGetPhysicalAddr(reinterpret_cast<uptr>(pShape->m_pIndexBuffers[i])) - vramA;
    }

    u8* pCommand = pShape->m_CommandBuffer;
    for (int i = 0; i < 2; ++i) {
        pShape->m_pActivateCommands[i] = reinterpret_cast<u32*>(ut::RoundUp(pCommand, 32));
        pCommand = reinterpret_cast<u8*>(pShape->m_pActivateCommands[i]) + ACTIVATE_COMMAND_CAPACITY;
    }
    pShape->m_pDeactivateCommand = reinterpret_cast<u32*>(ut::RoundUp(pCommand, 32));
    pShape->m_pPrimitiveCommand = reinterpret_cast<u32*>(ut::RoundUp(reinterpret_cast<u8*>(pShape->m_pDeactivateCommand) + DEACTIVATE_COMMAND_CAPACITY, 32));
    return pShape;
}

// 0x00495A88 slot 0x00 | nintendogs:bytes
// 0x00495A14 (deleting dtor)
nw::gfx::ParticleShape::~ParticleShape()
{
    m_pDeviceAllocator->Free(m_pDeviceMemory);
    if (m_pActivateCommands[0] != nullptr) {
        m_pActivateCommands[0] = nullptr;
        m_ActivateCommandSizes[0] = 0;
    }
    if (m_pActivateCommands[1] != nullptr) {
        m_pActivateCommands[1] = nullptr;
        m_ActivateCommandSizes[1] = 0;
    }
    if (m_pDeactivateCommand != nullptr) {
        m_pDeactivateCommand = nullptr;
        m_DeactivateCommandSize = 0;
    }
    if (m_pPrimitiveCommand != nullptr) {
        m_pPrimitiveCommand = nullptr;
        m_PrimitiveCommandSize = 0;
    }
}

} // namespace gfx
} // namespace nw
