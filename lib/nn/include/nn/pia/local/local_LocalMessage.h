#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local12LocalMessageE @ 0x008CFAC8
// vtable 0x00900868 (vptr 0x00900870), offset_to_top 0, 4 entries
//
// A message of the local network in a buffer of the caller: a header (the type, the size of the
// data and a CRC-16 of the header), then the data. The layout is from the constructor; the member
// names and the header type are ours.
class LocalMessage : public ::nn::pia::common::RootObject
{
public:
    // the header in the buffer
    struct Header
    {
        u8 m_Version;     // 0x0, always 1
        u8 m_Type;        // 0x1
        u16 m_DataSize;   // 0x2
        u8 m_Reserved[6]; // 0x4
        u16 m_Checksum;   // 0xA, CRC-16 of the bytes before it
    };

    static const u8 VERSION = 1;
    static const u16 HEADER_SIZE = 12;
    // the bytes of the header the checksum covers
    static const u32 CHECKSUM_SIZE = 10;

    LocalMessage(u8* pBuffer, u32 bufferSize); // 0x00414A74 | fefates:bytes [tier B]
    virtual ~LocalMessage(); // 0x00414AB0 slot 0x00
    // 0x00414AAC slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00414980 slot 0x08 | fefates:bytes
    // false if the checksum does not match
    virtual bool ParseMessageHeader(); // 0x00414940 slot 0x0C | fefates:callseq-callee

    // the data at the offset; the data size becomes offset + size
    void SetData(const void* pData, int offset, u16 size); // 0x004149C0 | fefates:bytes [tier B]
    // the data after the data so far
    void SetData(const void* pData, u16 size); // 0x00414A2C | fefates:bytes [tier B]
    void GetData(void* pData, int offset, u16 size) const; // 0x0072FBB0 | fefates:bytes [tier B]
    // the size of header and data (name is ours)
    u16 GetMessageSize() const { return m_HeaderSize + m_DataSize; }

    u8* m_pBuffer;     // 0x04
    u32 m_BufferSize;  // 0x08
    u8 m_Type;         // 0x0C
    u16 m_HeaderSize;  // 0x0E
    u16 m_DataSize;    // 0x10
};
ASSERT_SIZE(LocalMessage::Header, 0xC);
ASSERT_OFFSET(LocalMessage, m_DataSize, 0x10);
ASSERT_SIZE(LocalMessage, 0x14);
} // namespace local
} // namespace pia
} // namespace nn
