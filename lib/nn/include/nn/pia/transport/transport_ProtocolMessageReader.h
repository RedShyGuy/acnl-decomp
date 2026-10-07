#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
}
namespace transport {
// Reads one message of a packet: a 20 byte header (flags, source station index, payload size,
// destination, source station key, protocol id, reserved data; all big endian) and the payload.
// Layout from Attach; the member names are ours.
class ProtocolMessageReader
{
public:
    // the header flags
    static const u8 FLAG_DESTINATION_KEY = 1; // the destination is a station key, not a bitmap
    static const u8 FLAG_RELAY_REQUEST = 2;   // to be relayed to other stations (PacketHandler::RelayMessage)
    static const u8 FLAG_RELAYED = 4;         // relayed: the packet does not come from its source (Attach
                                              // clears the source address)
    static const u8 FLAG_OWN_PACKET = 8;      // the message has a packet of its own
    static const u8 TERMINATOR = 0xFF;
    static const unsigned int HEADER_SIZE = 20;

    ProtocolMessageReader(); // 0x0045A298 | fefates:bytes [tier B]
    ~ProtocolMessageReader(); // 0x0045A2B8 | fefates:bytes [tier B]

    void Clear(); // 0x0045A1A8 | fefates:bytes [tier B]
    // the message at offset in the payload of the packet (none at its end)
    void Attach(const nn::pia::common::Packet& packet, unsigned int offset); // 0x0045A1CC | fefates:bytes [tier B]

    u32 GetDestination() const; // 0x007361B0 | fefates:bytes [tier B]
    u32 GetReservedData() const; // 0x007361C4 | fefates:bytes [tier B]
    u16 GetProtocolIdPort() const; // 0x007361D8 | fefates:bytes [tier B]
    u32 GetSourceStationKey() const; // 0x007361F0 | fefates:bytes [tier B]

    bool IsValid() const { return m_pHeader != nullptr; }
    StationIndex GetSourceStationIndex() const { return static_cast<StationIndex>(m_pHeader[1]); }
    const u8* GetPayload() const { return m_pHeader + HEADER_SIZE; }
    ProtocolId GetProtocolId() const
    {
        ProtocolId id;
        id.m_Id = __builtin_bswap32(*reinterpret_cast<const u32*>(m_pHeader + 12));
        return id;
    }
    // the size of the message in the packet: header and payload, 4 aligned (plus 1 to 4 bytes)
    u32 GetMessageSize() const { return ((m_PayloadSize + HEADER_SIZE - 1) & ~3) + 4; }

    u32 m_PayloadSize;                       // 0x00
    const u8* m_pHeader;                     // 0x04
    bool m_IsTerminated;                     // 0x08, the packet ends here (terminator byte)
    common::StationAddress m_SourceAddress;  // 0x0C
    u8 m_Unknown0x1C;                        // 0x1C, byte 5 of the packet
    u8 m_Ttl;                                // 0x1D
};
ASSERT_SIZE(ProtocolMessageReader, 0x20);
} // namespace transport
} // namespace pia
} // namespace nn
