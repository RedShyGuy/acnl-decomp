#include "nw/gfx/gfx_ParticleCollection.h"

#include <cstring>
#include <new>

#include "nn/nstd/nstd_String.h"
#include "nw/gfx/gfx_ParticleSet.h"
#include "nw/gfx/gfx_ParticleShape.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace gfx {
namespace {
// GL_FLOAT (name after the OpenGL ES API)
const u32 GL_FLOAT_TYPE = 0x1406;

// the bytes of the buffers of the collection itself (names are ours)
inline int GetIndexListSize(int capacity)
{
    return capacity * sizeof(u16) + 24;
}
inline int GetWordStreamSize(int capacity)
{
    return capacity * sizeof(u32) + 40;
}
inline int GetUsage12StreamSize(int capacity)
{
    return capacity * 12 + 104;
}
} // namespace

// 0x00982ED4
const TypeInfo nw::gfx::ParticleCollection::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x0079EDAC (static initializer)

// 0x0049E9E0 (name is ours)
void nw::gfx::ParticleCollection::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleCollection resource)
{
    const res::ResParticleCollectionData* pData = resource.ptr();
    const int capacity = pData->capacity;
    bool isGiven[MAX_ATTRIBUTES] = {};
    isGiven[USAGE_DRAW_INDEX] = true;
    isGiven[USAGE_FREE_INDEX] = true;
    isGiven[USAGE_15] = true;

    // the object and the buffers of usages 14 and 15
    int size = ut::RoundUp(sizeof(ParticleCollection) + GetIndexListSize(capacity), 4);
    size = ut::RoundUp(size, 8) + GetWordStreamSize(capacity);

    for (int i = 0; i < pData->attributes.count; ++i) {
        const res::ResParticleAttributeData* pAttribute = static_cast<const res::ResParticleAttributeData*>(pData->attributes.GetData(i));
        const int usage = pAttribute->usage;
        if (pAttribute->typeInfo == res::ResParticleAttributeData::TYPE_CONSTANT) {
            switch (usage) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 8:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 3, size);
                break;
            case 4:
            case 7:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 1, size);
                break;
            case 5:
            case 6:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 2, size);
                break;
            case 10:
            case 11:
                size = ut::RoundUp(size, 4) + sizeof(u32);
                break;
            }
        } else {
            switch (usage) {
            case 10:
            case 11:
                size = ut::RoundUp(size, 8) + GetWordStreamSize(capacity);
                break;
            case 12:
                size = ut::RoundUp(size, 8) + GetUsage12StreamSize(capacity);
                break;
            }
        }
        isGiven[usage] = true;
    }

    // the defaults of the attributes the resource does not give
    for (int usage = 0; usage < MAX_ATTRIBUTES; ++usage) {
        if (isGiven[usage]) {
            continue;
        }
        switch (usage) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 8:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 3, size);
            break;
        case 4:
        case 7:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 1, size);
            break;
        case 5:
        case 6:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 2, size);
            break;
        }
        isGiven[usage] = false;
    }

    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(32);
    pSize->Add(size);
    pSize->SetAlignment(alignment);
}

// 0x0049ECA4 (name is ours)
void nw::gfx::ParticleCollection::GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleCollection resource)
{
    const res::ResParticleCollectionData* pData = resource.ptr();
    const int capacity = pData->capacity;
    int size = 0;
    for (int i = 0; i < pData->attributes.count; ++i) {
        const res::ResParticleAttributeData* pAttribute = static_cast<const res::ResParticleAttributeData*>(pData->attributes.GetData(i));
        if (pAttribute->typeInfo == res::ResParticleAttributeData::TYPE_CONSTANT) {
            continue;
        }
        switch (pAttribute->usage) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 8:
            size = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 3, capacity, size);
            break;
        case 4:
        case 7:
            size = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 1, capacity, size);
            break;
        case 5:
        case 6:
            size = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 2, capacity, size);
            break;
        }
    }

    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(32);
    pSize->Add(size);
    pSize->SetAlignment(alignment);
}

// 0x0049EDE4 | nintendogs:bytes [tier A]
void nw::gfx::ParticleCollection::Clear()
{
    const int capacity = m_Capacity;
    m_Count = 0;
    m_Unknown0x1A4 = 0xFFFF;
    m_Unknown0x1A6 = 0;
    for (int i = 0; i < 2; ++i) {
        std::memset(GetStreamBuffer(USAGE_DRAW_INDEX, i), 0xFF, capacity * sizeof(u16));
    }
    u16* pFreeIndices = static_cast<u16*>(GetStreamBuffer(USAGE_FREE_INDEX, 0));
    for (int i = 0; i < capacity; ++i) {
        pFreeIndices[i] = capacity - 1 - i;
    }
}

// 0x0049EED4 | nintendogs:callseq [tier A]
nw::gfx::ParticleCollection* nw::gfx::ParticleCollection::Create(ParticleSet* pParticleSet, res::ResParticleCollection resource,
                                                                 os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator,
                                                                 ParticleShape* pShape)
{
    const res::ResParticleCollectionData* pData = resource.ptr();
    const int capacity = pData->capacity;

    // the sizes of the memory block and of the streams of the shape
    int deviceSize = 0;
    bool isGiven[MAX_ATTRIBUTES] = {};
    isGiven[USAGE_DRAW_INDEX] = true;
    isGiven[USAGE_FREE_INDEX] = true;
    isGiven[USAGE_15] = true;
    int size = ut::RoundUp(sizeof(ParticleCollection) + GetIndexListSize(capacity), 4);
    size = ut::RoundUp(size, 8) + GetWordStreamSize(capacity);
    for (int i = 0; i < pData->attributes.count; ++i) {
        const res::ResParticleAttributeData* pAttribute = static_cast<const res::ResParticleAttributeData*>(pData->attributes.GetData(i));
        const int usage = pAttribute->usage;
        if (pAttribute->typeInfo == res::ResParticleAttributeData::TYPE_CONSTANT) {
            switch (usage) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 8:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 3, size);
                break;
            case 4:
            case 7:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 1, size);
                break;
            case 5:
            case 6:
                size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 2, size);
                break;
            case 10:
            case 11:
                size = ut::RoundUp(size, 4) + sizeof(u32);
                break;
            }
        } else {
            switch (usage) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 8:
                deviceSize = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 3, capacity, deviceSize);
                break;
            case 4:
            case 7:
                deviceSize = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 1, capacity, deviceSize);
                break;
            case 5:
            case 6:
                deviceSize = ParticleShape::AddVertexStreamSize(GL_FLOAT_TYPE, 2, capacity, deviceSize);
                break;
            case 10:
            case 11:
                size = ut::RoundUp(size, 8) + GetWordStreamSize(capacity);
                break;
            case 12:
                size = ut::RoundUp(size, 8) + GetUsage12StreamSize(capacity);
                break;
            }
        }
        isGiven[usage] = true;
    }
    for (int usage = 0; usage < MAX_ATTRIBUTES; ++usage) {
        if (isGiven[usage]) {
            continue;
        }
        switch (usage) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 8:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 3, size);
            break;
        case 4:
        case 7:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 1, size);
            break;
        case 5:
        case 6:
            size = ParticleShape::AddVertexParamSize(GL_FLOAT_TYPE, 2, size);
            break;
        }
        isGiven[usage] = false;
    }

    u8* pDeviceMemory = nullptr;
    if (deviceSize > 0) {
        pDeviceMemory = static_cast<u8*>(pDeviceAllocator->Alloc(deviceSize, 32));
        if (pDeviceMemory == nullptr) {
            return nullptr;
        }
    }
    u8* pMemory = static_cast<u8*>(pAllocator->Alloc(size, 32));
    if (pMemory == nullptr) {
        if (pDeviceMemory != nullptr) {
            pDeviceAllocator->Free(pDeviceMemory);
        }
        return nullptr;
    }
    ParticleCollection* pCollection = new (pMemory) ParticleCollection(pAllocator, resource, pDeviceAllocator, pDeviceMemory);
    u8* pBuffer = ut::RoundUp(pMemory + sizeof(ParticleCollection), 4);
    pCollection->m_Capacity = capacity;
    pCollection->m_pShape = pShape;

    // 13: the index buffers of the shape
    pCollection->m_IsStream[USAGE_DRAW_INDEX] = true;
    pCollection->m_Strides[USAGE_DRAW_INDEX] = sizeof(u16);
    pCollection->m_pBuffers[USAGE_DRAW_INDEX][0] = pShape->m_pIndexBuffers[pShape->m_IsBufferSwapped ? 1 : 0];
    pCollection->m_pBuffers[USAGE_DRAW_INDEX][1] = pShape->m_pIndexBuffers[pShape->m_IsBufferSwapped ? 0 : 1];
    isGiven[USAGE_DRAW_INDEX] = true;

    // 14 and 15: one buffer for both sides
    Attribute& freeIndex = pCollection->m_Attributes[USAGE_FREE_INDEX];
    void* pFreeIndices = ut::RoundUp(pBuffer, 8);
    pBuffer = static_cast<u8*>(pFreeIndices) + GetIndexListSize(capacity);
    std::memset(pFreeIndices, 0, GetIndexListSize(capacity));
    freeIndex.usage = USAGE_FREE_INDEX;
    freeIndex.isStream = true;
    freeIndex.pBuffer = pFreeIndices;
    pCollection->m_IsStream[USAGE_FREE_INDEX] = true;
    pCollection->m_Strides[USAGE_FREE_INDEX] = sizeof(u16);
    pCollection->m_pBuffers[USAGE_FREE_INDEX][0] = freeIndex.pBuffer;
    pCollection->m_pBuffers[USAGE_FREE_INDEX][1] = freeIndex.pBuffer;
    isGiven[USAGE_FREE_INDEX] = true;

    Attribute& attribute15 = pCollection->m_Attributes[USAGE_15];
    void* pBuffer15 = ut::RoundUp(pBuffer, 8);
    pBuffer = static_cast<u8*>(pBuffer15) + GetWordStreamSize(capacity);
    std::memset(pBuffer15, 0, GetWordStreamSize(capacity));
    attribute15.usage = USAGE_15;
    attribute15.isStream = true;
    attribute15.pBuffer = pBuffer15;
    pCollection->m_IsStream[USAGE_15] = true;
    pCollection->m_Strides[USAGE_15] = sizeof(u16);
    pCollection->m_pBuffers[USAGE_15][0] = attribute15.pBuffer;
    pCollection->m_pBuffers[USAGE_15][1] = attribute15.pBuffer;
    isGiven[USAGE_15] = true;

    // the attributes of the resource: the float attributes are vertex attributes of the shape
    for (int i = 0; i < pData->attributes.count; ++i) {
        const res::ResParticleAttributeData* pAttribute = static_cast<const res::ResParticleAttributeData*>(pData->attributes.GetData(i));
        const int usage = pAttribute->usage;
        if (pAttribute->typeInfo == res::ResParticleAttributeData::TYPE_CONSTANT) {
            f32* pValue = const_cast<f32*>(pAttribute->value.To<f32>());
            void* pConstant = nullptr;
            switch (usage) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 8:
                pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 3, pValue, &pBuffer)->pBuffers[0];
                break;
            case 4:
            case 7:
                pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 1, pValue, &pBuffer)->pBuffers[0];
                break;
            case 5:
            case 6:
                pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 2, pValue, &pBuffer)->pBuffers[0];
                break;
            case 10:
            case 11: {
                Attribute& word = pCollection->m_Attributes[usage];
                u32* pWord = ut::RoundUp(reinterpret_cast<u32*>(pBuffer), 4);
                pBuffer = reinterpret_cast<u8*>(pWord + 1);
                nnnstdMemCpy(pWord, pValue, sizeof(u32));
                word.usage = usage;
                word.pBuffer = pWord;
                word.isStream = false;
                *pWord = *pAttribute->value.To<u32>();
                pConstant = pWord;
                break;
            }
            }
            isGiven[usage] = true;
            pCollection->m_IsStream[usage] = false;
            pCollection->m_pBuffers[usage][0] = pConstant;
            pCollection->m_pBuffers[usage][1] = pConstant;
            pCollection->m_Strides[usage] = 0;
        } else {
            switch (usage) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 8: {
                ParticleShape::VertexAttribute* pStream = pShape->AddVertexStream(usage, GL_FLOAT_TYPE, 3, capacity, &pDeviceMemory);
                pCollection->m_pBuffers[usage][0] = pStream->pBuffers[0];
                pCollection->m_pBuffers[usage][1] = pStream->pBuffers[1];
                pCollection->m_Strides[usage] = 3 * sizeof(f32);
                break;
            }
            case 4:
            case 7: {
                ParticleShape::VertexAttribute* pStream = pShape->AddVertexStream(usage, GL_FLOAT_TYPE, 1, capacity, &pDeviceMemory);
                pCollection->m_pBuffers[usage][0] = pStream->pBuffers[0];
                pCollection->m_pBuffers[usage][1] = pStream->pBuffers[1];
                pCollection->m_Strides[usage] = sizeof(f32);
                break;
            }
            case 5:
            case 6: {
                ParticleShape::VertexAttribute* pStream = pShape->AddVertexStream(usage, GL_FLOAT_TYPE, 2, capacity, &pDeviceMemory);
                pCollection->m_pBuffers[usage][0] = pStream->pBuffers[0];
                pCollection->m_pBuffers[usage][1] = pStream->pBuffers[1];
                pCollection->m_Strides[usage] = 2 * sizeof(f32);
                break;
            }
            case 10:
            case 11: {
                Attribute& word = pCollection->m_Attributes[usage];
                void* pStream = ut::RoundUp(pBuffer, 8);
                pBuffer = static_cast<u8*>(pStream) + GetWordStreamSize(capacity);
                std::memset(pStream, 0, GetWordStreamSize(capacity));
                word.usage = usage;
                word.pBuffer = pStream;
                word.isStream = true;
                pCollection->m_pBuffers[usage][0] = pStream;
                pCollection->m_pBuffers[usage][1] = word.pBuffer;
                pCollection->m_Strides[usage] = sizeof(u32);
                break;
            }
            case 12: {
                Attribute& attribute12 = pCollection->m_Attributes[12];
                void* pStream = ut::RoundUp(pBuffer, 8);
                pBuffer = static_cast<u8*>(pStream) + GetUsage12StreamSize(capacity);
                std::memset(pStream, 0, GetUsage12StreamSize(capacity));
                attribute12.usage = usage;
                attribute12.pBuffer = pStream;
                attribute12.isStream = true;
                pCollection->m_pBuffers[12][0] = pStream;
                pCollection->m_Strides[12] = 12;
                pCollection->m_pBuffers[12][1] = attribute12.pBuffer;
                break;
            }
            }
            pCollection->m_IsStream[usage] = true;
            isGiven[usage] = true;
        }
    }

    // the defaults of the other attributes: constants of 0 or 1
    for (int usage = 0; usage < MAX_ATTRIBUTES; ++usage) {
        if (isGiven[usage]) {
            continue;
        }
        f32 zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        // 0x008C6468
        f32 one[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        void* pConstant = nullptr;
        switch (usage) {
        case 0:
        case 2:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 3, zero, &pBuffer)->pBuffers[0];
            break;
        case 1:
        case 3:
        case 8:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 3, one, &pBuffer)->pBuffers[0];
            break;
        case 4:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 1, one, &pBuffer)->pBuffers[0];
            break;
        case 7:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 1, zero, &pBuffer)->pBuffers[0];
            break;
        case 5:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 2, zero, &pBuffer)->pBuffers[0];
            break;
        case 6:
            pConstant = pShape->AddVertexParam(usage, GL_FLOAT_TYPE, 2, one, &pBuffer)->pBuffers[0];
            break;
        }
        isGiven[usage] = true;
        pCollection->m_IsStream[usage] = false;
        pCollection->m_pBuffers[usage][0] = pConstant;
        pCollection->m_pBuffers[usage][1] = pConstant;
        pCollection->m_Strides[usage] = 0;
    }

    pCollection->Clear();
    if (pParticleSet != nullptr) {
        if (pParticleSet->m_pParticleCollection == nullptr) {
            pParticleSet->m_pParticleCollection = pCollection;
        }
        pCollection->m_pParticleSet = pParticleSet;
    }
    return pCollection;
}

// 0x0049FA00 slot 0x00 | nintendogs:bytes
// 0x0049F998 (deleting dtor)
nw::gfx::ParticleCollection::~ParticleCollection()
{
    if (m_pDeviceMemory != nullptr) {
        m_pDeviceAllocator->Free(m_pDeviceMemory);
    }
    for (int i = 0; i < MAX_ATTRIBUTES; ++i) {
        if (m_Attributes[i].pBuffer != nullptr) {
            m_Attributes[i].pBuffer = nullptr;
        }
        m_pBuffers[i][0] = nullptr;
        m_pBuffers[i][1] = nullptr;
    }
}

} // namespace gfx
} // namespace nw
