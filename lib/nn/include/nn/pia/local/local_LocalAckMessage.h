#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local15LocalAckMessageE @ 0x008CFAF8
// vtable 0x00900904 (vptr 0x0090090C), offset_to_top 0, 4 entries
//
// The answer to a message: the header has the value the answer is for (LocalSendMessageJob). The
// member names are ours.
class LocalAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    // the header in the buffer
    struct AckHeader
    {
        LocalMessage::Header m_Header; // 0x00
        u32 m_AckValue;                // 0x0C
        u32 m_Reserved;                // 0x10
    };

    static const u16 ACK_HEADER_SIZE = 20;

    LocalAckMessage(u8* pBuffer, u32 bufferSize, u8 type, u32 ackValue); // 0x004169F4
    virtual ~LocalAckMessage(); // 0x00416A28 slot 0x00
    // 0x00416A24 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x004169A0 slot 0x08 | fefates:bytes
    virtual bool ParseMessageHeader(); // 0x00416954 slot 0x0C | fefates:bytes

    u32 m_AckValue; // 0x14
};
ASSERT_SIZE(LocalAckMessage::AckHeader, 0x14);
ASSERT_SIZE(LocalAckMessage, 0x18);
} // namespace local
} // namespace pia
} // namespace nn
