#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// A growable byte buffer with room reserved in front of the content (at least 10 bytes of storage).
// The member names and the names of the inline helpers are ours.
// RTTI N2nn3nex7qBufferE @ 0x008CF698
// vtable 0x008FFB88 (vptr 0x008FFB90), offset_to_top 0, 2 entries
class qBuffer : public ::nn::nex::RefCountedObject
{
public:
    static const u32 MIN_CAPACITY = 10;

    // (inline)
    qBuffer() : m_pStorage(nullptr), m_pBegin(nullptr), m_pEnd(nullptr), m_pStorageEnd(nullptr), m_ReservedSize(0) {}
    qBuffer(const nn::nex::qBuffer& other); // 0x003D3DAC | fefates:bytes [tier B]
    qBuffer(unsigned int capacity, unsigned int reservedSize); // 0x003D3E60 | fefates:bytes [tier B]
    virtual ~qBuffer(); // 0x003D3F20 slot 0x00 | fefates:bytes
    // 0x003D3EAC slot 0x04 | fefates:bytes (deleting dtor)

    // new storage of the capacity (the content is lost)
    DECOMP_NOINLINE void initialize(unsigned int capacity); // 0x003D3BD0 | fefates:bytes [tier B]
    void clear(); // 0x003D3C38 | fefates:bytes [tier B]
    // new storage of at least the capacity (a multiple of half the old one) with the prefix and the
    // old content
    void resize(unsigned int capacity, const unsigned char* pPrefix, unsigned int prefixSize); // 0x003D3C64 | fefates:bytes [tier B]
    void push_back(const unsigned char* pData, unsigned int size); // 0x003D3D2C | fefates:bytes [tier B]
    nn::nex::qBuffer& operator=(const nn::nex::qBuffer& other); // 0x003D3F78 | fefates:bytes [tier B]

    // (inline)
    u32 size() const { return m_pEnd == m_pBegin ? 0 : m_pEnd - m_pBegin; }
    u32 GetCapacity() const { return m_pStorageEnd == m_pStorage ? 0 : m_pStorageEnd - m_pStorage; }
    u32 GetFreeSize() const { return m_pStorageEnd == m_pEnd ? 0 : m_pStorageEnd - m_pEnd; }
    u32 GetUsedSize() const { return m_pEnd == m_pStorage ? 0 : m_pEnd - m_pStorage; }
    // (inline) the content becomes empty after the reserved room
    void Reset()
    {
        if (m_ReservedSize != 0) {
            m_pBegin = m_pEnd = m_pStorage + m_ReservedSize;
        } else {
            m_pBegin = m_pEnd = m_pStorage;
        }
    }

    u8* m_pStorage;     // 0x0C
    u8* m_pBegin;       // 0x10
    u8* m_pEnd;         // 0x14
    u8* m_pStorageEnd;  // 0x18
    u32 m_ReservedSize; // 0x1C
};
ASSERT_SIZE(qBuffer, 0x20);
} // namespace nex
} // namespace nn
