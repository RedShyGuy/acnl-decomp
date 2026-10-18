#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Buffer;

// Reads and writes bits (the high bit of a byte first) in a Buffer. The names of the members are
// ours.
// RTTI N2nn3nex9BitStreamE @ 0x008CF738
// vtable 0x008FFCF4 (vptr 0x008FFCFC), offset_to_top 0, 2 entries
class BitStream : public ::nn::nex::RootObject
{
public:
    // the stream takes a reference to the buffer
    explicit BitStream(nn::nex::Buffer* pBuffer); // 0x003D6618 | fefates:bytes [tier B]
    BitStream(); // 0x003D6670 | fefates:bytes [tier B]
    virtual ~BitStream(); // 0x003D6734 slot 0x00
    // 0x003D66B4 (deleting dtor)
    // count values of size bytes
    void Extract(unsigned char* pData, unsigned int count, unsigned int size); // 0x003D620C | fefates:callgraph [tier C]
    // the bits (the last byte gets the remaining bits from its high bit)
    void ExtractRaw(unsigned char* pData, unsigned int bits); // 0x003D6218 | fefates:bytes-fuzzy [tier B]
    // the length is the content of the buffer
    void AdjustLength(); // 0x003D63FC | fefates:callgraph [tier C]
    void Append(const unsigned char* pData, unsigned int count, unsigned int size); // 0x003D6410 | fefates:callgraph [tier C]
    void AppendRaw(const unsigned char* pData, unsigned bits); // 0x003D641C | mk7dlp:callseq [tier A]
    // a u32 size and the bytes
    nn::nex::BitStream& operator<<(const nn::nex::Buffer& buffer); // 0x003D679C | mk7dlp:bytes [tier A]
    nn::nex::BitStream& operator>>(nn::nex::Buffer& buffer); // 0x003D67EC | fefates:bytes [tier B]
    nn::nex::BitStream& operator>>(nn::nex::BitStream& stream); // 0x003D687C | fefates:bytes [tier B]

    // (inline) the bits read or written so far
    u32 GetPosition() const { return m_ByteIndex * 8 + 7 - m_BitIndex; }

    bool m_IsError;     // 0x04
    Buffer* m_pBuffer;  // 0x08
    u32 m_ByteIndex;    // 0x0C
    u32 m_BitIndex;     // 0x10 (7: the high bit)
    u32 m_BitLength;    // 0x14
};
ASSERT_SIZE(BitStream, 0x18);
} // namespace nex
} // namespace nn
