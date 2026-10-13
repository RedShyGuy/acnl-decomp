#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include <cstring>
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the value of a hexadecimal digit (upper case)
u8 GetHexValue(wchar_t c)
{
    switch (c) {
    case L'A':
        return 10;
    case L'B':
        return 11;
    case L'C':
        return 12;
    case L'D':
        return 13;
    case L'E':
        return 14;
    case L'F':
        return 15;
    default:
        return c - L'0';
    }
}
} // namespace

// 0x0096CA58 (name is ours)
u32 Buffer::s_DefaultBufferSize = 0x554;

// 0x003CF8F8 | mk7dlp:callgraph [tier A]
bool nn::nex::Buffer::AppendData(const void* pData, unsigned size, unsigned offset)
{
    if (size == 0) {
        return true;
    }
    u32 contentSize = m_ContentSize;
    if (offset == static_cast<unsigned>(-1)) {
        offset = contentSize;
    }
    if (offset > contentSize) {
        contentSize = offset + size;
    } else if (offset + size > contentSize) {
        contentSize = offset + size;
    }
    ResizeByRealSize(GetAllocationSize(m_Size, m_HeaderSize + contentSize));
    m_ContentSize = contentSize;
    memcpy(m_pData + m_HeaderSize + offset, pData, size);
    return true;
}

// 0x003CF97C | fefates:bytes [tier B]
void nn::nex::Buffer::Initialize(unsigned int size, unsigned char flags)
{
    m_Flags = flags;
    m_ContentSize = 0;
    m_Size = size;
    m_HeaderSize = 0;
    m_pData = static_cast<u8*>(AllocateDataBuffer(size));
    m_Unknown0x1D = 0;
    m_Unknown0x1E = 0;
}

// 0x003CF9AC | fefates:callgraph [tier C]
bool nn::nex::Buffer::AttemptExpand(unsigned int size)
{
    u32 newSize = GetAllocationSize(m_Size, m_ContentSize + m_HeaderSize + size);
    if (m_Size >= newSize) {
        return true;
    }
    u8* pData = static_cast<u8*>(AllocateDataBuffer(newSize));
    if (pData == nullptr) {
        return false;
    }
    memcpy(pData, m_pData, m_Size);
    MemoryManager::FreeInline(m_pData);
    m_pData = pData;
    m_Size = newSize;
    return true;
}

// 0x003CFA38 | fefates:bytes [tier B]
u8 nn::nex::Buffer::ComputeCheckSum(unsigned int size, unsigned char initial)
{
    u32 sum = 0;
    const u8* p = m_pData + m_HeaderSize;
    u32 words = size / sizeof(u32);
    for (u32 i = 0; i < words; i++) {
        sum += reinterpret_cast<const u32*>(p)[i];
    }
    p += words * sizeof(u32);
    u8 checksum = initial;
    for (u32 i = 0; i < size % sizeof(u32); i++) {
        checksum += p[i];
    }
    const u8* pSum = reinterpret_cast<const u8*>(&sum);
    return checksum + pSum[0] + pSum[1] + pSum[2] + pSum[3];
}

// 0x003CFB18 (name is ours)
u32 nn::nex::Buffer::GetAllocationSize(u32 current, u32 size)
{
    if (current >= size) {
        return current;
    }
    u32 newSize = current;
    if (newSize == 0) {
        newSize = 1;
    }
    u32 unitSize = 0;
    if (PacketBufferManager::GetInstance() != nullptr) {
        PacketBufferManager::Pool* pPool = PacketBufferManager::GetInstance()->m_pPool;
        if (pPool == nullptr) {
            unitSize = 0;
        } else {
            unitSize = 0;
            if (size <= pPool->m_MaxUnitSize) {
                for (u32 i = 0; i < pPool->m_UnitCount; i++) {
                    if (size <= pPool->m_pUnits[i].m_UnitSize) {
                        unitSize = pPool->m_pUnits[i].m_UnitSize;
                        break;
                    }
                }
            }
        }
    }
    if (size <= unitSize) {
        newSize = unitSize;
    }
    while (newSize < size) {
        newSize <<= 1;
    }
    return newSize;
}

// 0x003CFC30 | fefates:bytes [tier B]
void nn::nex::Buffer::ResizeByRealSize(unsigned int size)
{
    if (m_Size == size) {
        return;
    }
    u8* pData = static_cast<u8*>(AllocateDataBuffer(size));
    if (pData == nullptr) {
        m_Flags &= ~FLAG_USE_POOL;
        pData = static_cast<u8*>(AllocateDataBuffer(size));
    }
    memcpy(pData, m_pData, m_Size < size ? m_Size : size);
    MemoryManager::FreeInline(m_pData);
    m_pData = pData;
    m_Size = size;
}

// 0x003CFCBC | fefates:callgraph [tier C]
void* nn::nex::Buffer::AllocateDataBuffer(unsigned int size)
{
    if (PacketBufferManager::GetInstance() == nullptr || PacketBufferManager::GetInstance()->GetMaxUnitSize() < size) {
        m_Flags &= ~FLAG_USE_POOL;
    }
    if (m_Flags & FLAG_USE_POOL) {
        void* (*pAllocate)(u32) = PacketBufferManager::GetInstance()->m_pAllocateFunction;
        if (pAllocate != nullptr) {
            return pAllocate(size);
        }
    }
    return MemoryManager::Allocate(size);
}

// 0x003CFDA8 | fefates:callgraph [tier C]
u32 nn::nex::Buffer::GetDefaultBufferSize()
{
    return s_DefaultBufferSize;
}

// 0x003CFDB8
nn::nex::Buffer::Buffer(const nn::nex::String& string)
{
    m_HeaderSize = 0;
    if (string.m_pString == nullptr || wcslen(string.m_pString) == 0) {
        m_Flags = 0;
        m_Size = 0;
        m_pData = nullptr;
        m_ContentSize = 0;
    } else {
        u32 length = wcslen(string.m_pString);
        m_Flags = 0;
        m_Size = length / 2;
        m_ContentSize = length / 2;
        m_pData = static_cast<u8*>(AllocateDataBuffer(m_Size));
        for (u32 i = 0; i < m_Size; i++) {
            m_pData[i] = GetHexValue(string.m_pString[i * 2 + 1]) | (GetHexValue(string.m_pString[i * 2]) << 4);
        }
    }
    m_Unknown0x1D = 0;
    m_Unknown0x1E = 0;
}

// 0x003CFF38 | mk7dlp:callseq-callee [tier A]
nn::nex::Buffer::Buffer(const nn::nex::Buffer& other)
{
    m_Size = other.m_Size;
    m_Flags = 0;
    m_pData = static_cast<u8*>(AllocateDataBuffer(m_Size));
    m_ContentSize = other.m_ContentSize;
    m_HeaderSize = 0;
    *this = other;
    m_Flags = other.m_Flags & ~FLAG_USE_POOL;
    m_Unknown0x1D = other.m_Unknown0x1D;
    m_Unknown0x1E = other.m_Unknown0x1E;
}

// 0x003CFFBC | fefates:bytes [tier B]
nn::nex::Buffer::Buffer(unsigned int size)
{
    Initialize(size, 0);
}

// 0x003D0010 | mk7dlp:callseq-callee [tier A]
nn::nex::Buffer::Buffer()
{
    Initialize(GetDefaultBufferSize(), 0);
}

// 0x003D0078 | mk7dlp:callseq-callee
// 0x003D0044 (deleting dtor)
nn::nex::Buffer::~Buffer()
{
    MemoryManager::FreeInline(m_pData);
}

// 0x003D00D0 | mk7dlp:callgraph [tier A]
nn::nex::Buffer& nn::nex::Buffer::operator=(const nn::nex::Buffer& other)
{
    if (this == &other) {
        return *this;
    }
    u32 size = other.m_ContentSize + other.m_HeaderSize;
    m_ContentSize = 0;
    m_HeaderSize = 0;
    if (size > m_Size) {
        ResizeByRealSize(size);
    }
    AppendData(other.m_pData, size, static_cast<unsigned>(-1));
    u32 headerSize = other.m_HeaderSize;
    u32 oldHeaderSize = m_HeaderSize;
    m_HeaderSize = headerSize;
    m_ContentSize = oldHeaderSize - headerSize + m_ContentSize;
    m_Flags = (other.m_Flags & ~FLAG_USE_POOL) | (m_Flags & FLAG_USE_POOL);
    m_Unknown0x1D = other.m_Unknown0x1D;
    return *this;
}

// 0x003D016C | mk7dlp:callseq-callee [tier A]
u8& nn::nex::Buffer::operator[](unsigned index)
{
    if (m_Size == 0) {
        ResizeByRealSize(5);
    }
    if (m_HeaderSize + index >= m_Size) {
        ResizeByRealSize((m_HeaderSize + index) * 15 / 10);
    }
    if (m_ContentSize <= index) {
        m_ContentSize = index + 1;
    }
    return m_pData[m_HeaderSize + index];
}

// 0x0072E0A8 | fefates:bytes [tier B]
nn::nex::String nn::nex::Buffer::ToString() const
{
    String string;
    string.Reserve(m_ContentSize * 2 + 1);
    const u8* pData = m_pData + m_HeaderSize;
    u32 position = 0;
    for (u32 i = 0; i < m_ContentSize; i++) {
        position += swprintf(string.m_pString + position, 3, L"%02X", pData[i]);
    }
    return string;
}

} // namespace nex
} // namespace nn
