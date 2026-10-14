#pragma once

#include "decomp.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class EndPoint;

// A message of a protocol: a ByteStream whose first 4 bytes are the size of the rest (SetSize
// writes it). The names of the members and of the functions marked so are ours.
// RTTI N2nn3nex7MessageE @ 0x008CF674
// vtable 0x008FFB58 (vptr 0x008FFB60), offset_to_top 0, 2 entries
class Message : public ::nn::nex::ByteStream
{
public:
    // the size of the size field in front of the message
    static const u32 SIZE_LENGTH = 4;

    // (symbols.json names this ScopedCS::ScopedCS)
    Message(); // 0x003D2490 (name from the reference symbols, see above)
    explicit Message(nn::nex::Buffer* pBuffer); // 0x003D23CC | fefates:bytes-fuzzy [tier B]
    virtual ~Message(); // 0x003D2600 slot 0x00 | fefates:bytes-fuzzy [tier B]
    // 0x003D25AC slot 0x04 (deleting dtor)

    // appends the content of the other message (without its size field)
    void AddDataFromMessage(const nn::nex::Message& other); // 0x003D2324 | fefates:bytes [tier B]
    // writes the size of the rest into the size field
    void SetSize(); // 0x003D2348 | fefates:bytes [tier B]
    nn::nex::Buffer* GetBuffer(); // 0x003D23B8 | fefates:bytes [tier B]

    // (inline) reading starts behind the size field
    void Rewind()
    {
        if (m_pBuffer->m_ContentSize >= SIZE_LENGTH) {
            m_Position = SIZE_LENGTH;
        }
    }

    qResult m_Result;           // 0x10
    u32 m_Unknown0x1C;          // 0x1C (not set by the constructors)
    u64 m_Unknown0x20;          // 0x20
    u32 m_Unknown0x28;          // 0x28
    u32 m_Unknown0x2C;          // 0x2C
    u32 m_Unknown0x30;          // 0x30
    // the end point of the message (its stream closes it with the message)
    EndPoint* m_pEndPoint;      // 0x34
    u8 m_Unknown0x38;           // 0x38
    u8 m_Unknown0x39;           // 0x39
    u8 m_Unknown0x3A;           // 0x3A
    // the DDL structures have no header (version and size): the peer predates them
    bool m_IsWithoutStructureHeader; // 0x3B
};
ASSERT_OFFSET(Message, m_pEndPoint, 0x34);
ASSERT_SIZE(Message, 0x40); // (the u64 member aligns the size to 8)
} // namespace nex
} // namespace nn
