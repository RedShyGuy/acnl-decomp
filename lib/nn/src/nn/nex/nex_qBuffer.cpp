#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/nex_MemoryManager.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x003D3BD0 | fefates:bytes [tier B]
void nn::nex::qBuffer::initialize(unsigned int capacity)
{
    if (capacity != 0) {
        if (capacity < MIN_CAPACITY) {
            capacity = MIN_CAPACITY;
        }
        m_pStorage = static_cast<u8*>(MemoryManager::Allocate(capacity));
    } else {
        m_pStorage = nullptr;
    }
    m_pStorageEnd = m_pStorage + capacity;
    Reset();
}

// 0x003D3C38 | fefates:bytes [tier B]
void nn::nex::qBuffer::clear()
{
    Reset();
}

// 0x003D3C64 | fefates:bytes [tier B]
void nn::nex::qBuffer::resize(unsigned int capacity, const unsigned char* pPrefix, unsigned int prefixSize)
{
    u8* pOldStorage = m_pStorage;
    int half = static_cast<int>(GetCapacity()) / 2;
    if (half != 0) {
        capacity = (capacity / half + 1) * half;
    }
    if (capacity < MIN_CAPACITY) {
        capacity = MIN_CAPACITY;
    }
    u32 oldSize = size();
    initialize(capacity);
    if (pPrefix != nullptr) {
        push_back(pPrefix, prefixSize);
    }
    push_back(pOldStorage, oldSize);
    MemoryManager::FreeInline(pOldStorage);
}

// 0x003D3D2C | fefates:bytes [tier B]
void nn::nex::qBuffer::push_back(const unsigned char* pData, unsigned int size)
{
    if (size == 0) {
        return;
    }
    while (GetFreeSize() < size) {
        resize(GetUsedSize() + size, nullptr, 0);
    }
    memcpy(m_pEnd, pData, size);
    m_pEnd += size;
}

// 0x003D3DAC | fefates:bytes [tier B]
nn::nex::qBuffer::qBuffer(const nn::nex::qBuffer& other) : m_pStorage(nullptr), m_pBegin(nullptr), m_pEnd(nullptr), m_pStorageEnd(nullptr), m_ReservedSize(0)
{
    initialize(other.GetCapacity());
    *this = other;
}

// 0x003D3E60 | fefates:bytes [tier B]
nn::nex::qBuffer::qBuffer(unsigned int capacity, unsigned int reservedSize)
    : m_pStorage(nullptr), m_pBegin(nullptr), m_pEnd(nullptr), m_pStorageEnd(nullptr), m_ReservedSize(reservedSize)
{
    initialize(capacity);
}

// 0x003D3F20 slot 0x00 | fefates:bytes
// 0x003D3EAC (deleting dtor)
nn::nex::qBuffer::~qBuffer()
{
    MemoryManager::FreeInline(m_pStorage);
}

// 0x003D3F78 | fefates:bytes [tier B]
nn::nex::qBuffer& nn::nex::qBuffer::operator=(const nn::nex::qBuffer& other)
{
    if (&other != this) {
        Reset();
        push_back(other.m_pBegin, other.size());
    }
    return *this;
}

} // namespace nex
} // namespace nn
