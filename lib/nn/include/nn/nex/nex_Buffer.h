#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_SystemError.h"
#include <cstring>

namespace nn {
namespace nex {
// A growable byte buffer: the data starts after a reserved header (m_HeaderSize bytes) and has
// m_ContentSize bytes. With FLAG_USE_POOL the memory comes from the PacketBufferManager. The
// names of the members, of the flags and of the functions without a reference name are ours.
// RTTI N2nn3nex6BufferE @ 0x008CF570
// vtable 0x008FF9FC (vptr 0x008FFA04), offset_to_top 0, 2 entries
class Buffer : public ::nn::nex::RefCountedObject
{
public:
    static const u8 FLAG_USE_POOL = 1 << 2;

    Buffer(); // 0x003D0010 | mk7dlp:callseq-callee [tier A]
    explicit Buffer(unsigned int size); // 0x003CFFBC | fefates:bytes [tier B]
    Buffer(const nn::nex::Buffer& other); // 0x003CFF38 | mk7dlp:callseq-callee [tier A]
    // the bytes of a string of hexadecimal digits (upper case)
    explicit Buffer(const nn::nex::String& string); // 0x003CFDB8
    virtual ~Buffer(); // 0x003D0078 slot 0x00 | mk7dlp:callseq-callee
    // 0x003D0044 slot 0x04 (deleting dtor)

    // copies size bytes to offset (-1: the end) of the content
    bool AppendData(const void* pData, unsigned size, unsigned offset); // 0x003CF8F8 | mk7dlp:callgraph [tier A]
    void Initialize(unsigned int size, unsigned char flags); // 0x003CF97C | fefates:bytes [tier B]
    // room for size more bytes
    bool AttemptExpand(unsigned int size); // 0x003CF9AC | fefates:callgraph [tier C]
    // the byte sum of the first size bytes of the content plus initial (the words are summed first)
    u8 ComputeCheckSum(unsigned int size, unsigned char initial); // 0x003CFA38 | fefates:bytes [tier B]
    void ResizeByRealSize(unsigned int size); // 0x003CFC30 | fefates:bytes [tier B]
    void* AllocateDataBuffer(unsigned int size); // 0x003CFCBC | fefates:callgraph [tier C]
    static u32 GetDefaultBufferSize(); // 0x003CFDA8 | fefates:callgraph [tier C]
    // the size of the memory for size bytes: a size of the pools, else current doubled
    static u32 GetAllocationSize(u32 current, u32 size); // 0x003CFB18
    nn::nex::Buffer& operator=(const nn::nex::Buffer& other); // 0x003D00D0 | mk7dlp:callgraph [tier A]
    // (the content grows to the index)
    u8& operator[](unsigned index); // 0x003D016C | mk7dlp:callseq-callee [tier A]
    // the content in hexadecimal digits
    nn::nex::String ToString() const; // 0x0072E0A8 | fefates:bytes [tier B]

    // (inline in the original; Buffer.cpp) copies size bytes from offset of the content
    void CopyContent(void* pDestination, u32 offset, u32 size) const
    {
        if (offset + size > m_ContentSize) {
            SystemError::SignalError(const_cast<wchar_t*>(L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/Buffer.cpp"), 198,
                                     SystemError::ERROR_BUFFER_OVERRUN, 0);
            return;
        }
        memcpy(pDestination, m_pData + m_HeaderSize + offset, size);
    }

    // (inline; the name is ours) moves the start of the content (the bytes before it become the
    // header and the other way round)
    void SetHeaderSize(u32 size)
    {
        m_ContentSize = m_ContentSize + m_HeaderSize - size;
        m_HeaderSize = size;
    }

protected:
    // (inline; PacketBuffer)
    Buffer(unsigned int size, unsigned char flags)
    {
        m_ContentSize = 0;
        m_Size = size;
        m_HeaderSize = 0;
        m_Flags = flags;
        m_pData = static_cast<u8*>(AllocateDataBuffer(size));
        m_TTL = 0;
        m_Unknown0x1E = 0;
    }

public:
    u8* m_pData;        // 0x0C
    u32 m_ContentSize;  // 0x10
    u32 m_Size;         // 0x14
    u32 m_HeaderSize;   // 0x18
    u8 m_Flags;         // 0x1C
    u8 m_TTL;           // 0x1D (the time to live of the packet; 0: the default one)
    u8 m_Unknown0x1E;   // 0x1E

    static u32 s_DefaultBufferSize;
};
ASSERT_SIZE(Buffer, 0x20);
} // namespace nex
} // namespace nn
