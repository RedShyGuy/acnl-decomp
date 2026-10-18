#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// Reads and writes values little endian in a Buffer (written at its end, read from m_Position).
// A read behind the end sets m_IsError. The member names are ours.
// RTTI N2nn3nex10ByteStreamE @ 0x008CDF30
// vtable 0x008FC130 (vptr 0x008FC138), offset_to_top 0, 2 entries
class ByteStream : public ::nn::nex::RootObject
{
public:
    // (inline)
    ByteStream() : m_IsError(false), m_pBuffer(new Buffer()), m_Position(0) {}
    // (inline) with a new buffer of the size
    explicit ByteStream(u32 size) : m_IsError(false), m_pBuffer(new Buffer(size)), m_Position(0) {}
    // (inline) the stream takes a reference to the buffer
    explicit ByteStream(Buffer* pBuffer) : m_pBuffer(pBuffer)
    {
        pBuffer->AcquireRef();
        m_IsError = false;
        m_Position = 0;
    }
    virtual ~ByteStream(); // 0x00354DF0 slot 0x00 | fefates:callgraph
    // 0x00354DC0 slot 0x04 | fefates:callseq (deleting dtor)

    bool ExtractRaw(unsigned char* pData, unsigned int size); // 0x00354C70 | fefates:bytes-fuzzy [tier B]
    bool Append(const nn::nex::Time* pTime); // 0x00354D20 | fefates:bytes [tier B]
    // sets the size of the content (at most the room of the buffer)
    bool SetLength(unsigned int length); // 0x00354DA0 | fefates:bytes [tier B]
    // (only a nop in the original that falls into Buffer::AppendData)
    bool AppendRaw(const unsigned char* pData, unsigned size); // 0x003CF8EC | mk7dlp:callgraph [tier A]
    nn::nex::ByteStream& operator<<(const bool& value); // 0x00354E58 | mk7dlp:bytes [tier A]
    nn::nex::ByteStream& operator>>(bool& value); // 0x00354E8C | fefates:bytes [tier B]

    // (out of line per type; inline in some blobs: ExtractPrimitiveInline)
    // (noipa: GCC would clone it per caller)
    template <typename T>
    DECOMP_NOIPA bool ExtractPrimitive(T* pValue)
    {
        return ExtractPrimitiveInline(pValue);
    }
    // (an unaligned read of the value)
    template <typename T>
    bool ExtractPrimitiveInline(T* pValue)
    {
        if (m_pBuffer->m_ContentSize - m_Position < sizeof(T)) {
            m_IsError = true;
            *pValue = 0;
            return false;
        }
        const u8* p = m_pBuffer->m_pData + m_pBuffer->m_HeaderSize + m_Position;
        T value;
        memcpy(&value, p, sizeof(T));
        *pValue = value;
        m_Position += sizeof(T);
        return true;
    }

    bool m_IsError;      // 0x04
    Buffer* m_pBuffer;   // 0x08
    u32 m_Position;      // 0x0C
};
ASSERT_SIZE(ByteStream, 0x10);
} // namespace nex
} // namespace nn
