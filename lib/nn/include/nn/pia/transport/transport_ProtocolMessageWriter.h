#pragma once

#include "decomp.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
}
namespace transport {
// Writes one message into the packets it is sent in (one per destination); Commit fills the
// headers (see ProtocolMessageReader) and copies the first message into the others. Layout from
// Reset / AddMessageBuffer / Commit; the member names are ours.
class ProtocolMessageWriter
{
public:
    static const int BUFFER_NUM = 11;
    static const unsigned int HEADER_SIZE = 20;

    // one packet the message goes into
    struct Buffer
    {
        common::Packet* m_pPacket; // 0x0
        u8* m_pHeader;             // 0x4, the message in the packet
        u32 m_Destination;         // 0x8
        bool m_IsDestinationKey;              // 0xC, header flag 1
        bool m_IsRelayRequest;              // 0xD, header flag 2
    };

    ProtocolMessageWriter(); // 0x0045A51C | fefates:bytes [tier B]

    void SetSource(nn::pia::StationIndex stationIndex, unsigned int stationKey); // 0x0045A510 | fefates:bytes [tier B]
    void Reset(const nn::pia::transport::ProtocolId& protocolId, unsigned int payloadSize, bool isRelayed, bool isOwnPacket); // 0x0045A330 | fefates:bytes [tier B]
    void AddMessageBuffer(nn::pia::common::Packet* pPacket, void* pHeader, unsigned int destination, bool isDestinationKey, bool isRelayRequest); // 0x0045A2FC | fefates:bytes [tier B]
    // the whole payload / a part of it
    void SetPayload(const void* pData); // 0x0045A2D8 | fefates:bytes [tier B]
    void SetPayload(const void* pData, unsigned int offset, unsigned int size); // 0x0045A2E8 | fefates:bytes [tier B]
    void Commit(); // 0x0045A35C | fefates:bytes [tier B]

    // the payload of the (first) message, to be filled in place (inline in the protocols)
    u8* GetPayload() const { return m_Buffers[0].m_pHeader + HEADER_SIZE; }

    u32 m_PayloadSize;            // 0x00
    StationIndex m_SourceStationIndex; // 0x04
    u32 m_SourceStationKey;       // 0x08
    ProtocolId m_ProtocolId;      // 0x0C
    u32 m_ReservedData;           // 0x10
    bool m_IsRelayed;                 // 0x14, header flag 4
    bool m_IsOwnPacket;                 // 0x15, header flag 8
    Buffer m_Buffers[BUFFER_NUM]; // 0x18
    u32 m_BufferNum;              // 0xC8
    u8 m_Ttl;                     // 0xCC, at least this TTL for the packets (0: as they are)
};
ASSERT_SIZE(ProtocolMessageWriter, 0xD0);
} // namespace transport
} // namespace pia
} // namespace nn
