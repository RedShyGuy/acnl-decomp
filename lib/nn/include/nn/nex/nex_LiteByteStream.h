#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_LiteBuffer.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::LiteByteStream<20>  typeinfo 0x008CE33C  vtable 0x008FCBB0
//   nn::nex::LiteByteStream<52>  typeinfo 0x008CE348  vtable 0x008FCBC0
//   nn::nex::LiteByteStream<6>  typeinfo 0x008CE354  vtable 0x008FCBD0
//
// A byte stream over a LiteBuffer (inline, some functions out of line per size). The member names
// are ours.
template <int SIZE>
class LiteByteStream : public ::nn::nex::RootObject
{
public:
    LiteByteStream() : m_Position(0), m_IsError(false) {}
    virtual ~LiteByteStream() {}

    // (out of line)
    DECOMP_NOINLINE bool Append(const unsigned char& value)
    {
        u8 byte = value;
        return m_Buffer.AppendData(&byte, sizeof(byte), m_Buffer.m_Size);
    }

    u32 m_Position;           // 0x04
    LiteBuffer<SIZE> m_Buffer; // 0x08
    bool m_IsError;           // after the buffer
};
} // namespace nex
} // namespace nn
