#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
}
namespace transport {
struct ReceivedMessageAccessor;

// RTTI N2nn3pia9transport15StationProtocolE @ 0x008D01B8
// vtable 0x00901E0C (vptr 0x00901E14), offset_to_top 0, 9 entries
//
// The connection of the stations: connection request / response, disconnection request / response
// and the acks of the resent messages (ResendingMessageManager). The messages start with their type
// byte; requests and responses carry the protocol version 5 in byte 2. The member names, the
// message type names and the names marked so are ours.
class StationProtocol : public ::nn::pia::transport::Protocol
{
public:
    // byte 0 of the messages
    enum MessageType
    {
        MESSAGE_TYPE_CONNECTION_REQUEST = 1,
        MESSAGE_TYPE_CONNECTION_RESPONSE = 2,
        MESSAGE_TYPE_DISCONNECTION_REQUEST = 3,
        MESSAGE_TYPE_DISCONNECTION_RESPONSE = 4,
        MESSAGE_TYPE_ACK = 5,
        MESSAGE_TYPE_RELAY_CONNECTION_REQUEST = 6,
        MESSAGE_TYPE_RELAY_CONNECTION_RESPONSE = 7,
    };
    // byte 2 of the connection request / response
    static const u8 VERSION = 5;
    // byte 1 of a connection response that denies the connection (ConnectStationJob turns it
    // into its result)
    static const u8 DENY_REASON_REFUSED = 1;
    static const u8 DENY_REASON_INCOMPATIBLE_VERSION = 2;

    StationProtocol(); // 0x0045262C | fefates:bytes [tier B]
    virtual ~StationProtocol(); // 0x00452660 slot 0x00
    // 0x00452650 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00735664 slot 0x08
    virtual u16 GetProtocolType() const; // 0x0073551C slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x00452440 slot 0x10
    virtual void Cleanup(); // 0x00452324 slot 0x14
    virtual nn::Result Dispatch(); // 0x00452450 slot 0x18 | fefates:bytes
    virtual bool IsEnableProtocolFiltering() const; // 0x00735524 slot 0x20

    nn::Result ParseHelper(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x00451458 | fefates:bytes-fuzzy [tier B]
    // (name is ours)
    void ParseConnectionRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor, bool isRelay); // 0x00451860
    void ParseConnectionResponseCommon(const nn::pia::transport::ReceivedMessageAccessor& accessor, bool isRelay); // 0x0045205C | fefates:callseq [tier C]
    void ParseDisconnectionRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x004516B0 | fefates:bytes [tier B]

    // isOwnPacket: the request gets a packet of its own
    void SendDisconnectionRequest(nn::pia::StationIndex stationIndex, bool isOwnPacket); // 0x004515E4 | fefates:bytes [tier B]
    void SendDisconnectionRequest(const nn::pia::common::StationAddress& address); // 0x0045164C | fefates:bytes [tier B]
    void SendDenyingConnectionResponse(const nn::pia::common::StationAddress& address, unsigned char reason); // 0x0045229C | fefates:bytes [tier B]
    void SendAck(unsigned int ackId, nn::pia::StationIndex stationIndex); // 0x00452330 | fefates:bytes [tier B]
    void SendAck(unsigned int ackId, const nn::pia::common::StationAddress& address); // 0x004523B8 | fefates:bytes [tier B]

    // the connection request: the type, the connection id, the version, isInverseConnection and the
    // local StationConnectionInfo
    void MakeConnectionRequestData(unsigned char* pData, unsigned char connectionId, bool isRelay, bool isInverseConnection) const; // 0x0073552C | fefates:bytes [tier B]
    // the connection response: the local identification info
    void MakeConnectionResponseData(unsigned char* pData, bool isRelay) const; // 0x00735598 | fefates:bytes [tier B]
    u32 GetConnectionRequestDataSize() const; // 0x00735640 | fefates:bytes [tier B]

    // the ack message
    struct AckMessage
    {
        u8 m_Type;
        u8 m_Padding[3];
        u8 m_AckId[4]; // big endian
    };

    s32 m_ProcessTimeoutMSec;  // 0x14, the timeout of ProcessConnectionRequestJob (15000)
    u32 m_AddressChangedNum;   // 0x18, the stations that came back with another address
};
ASSERT_SIZE(StationProtocol, 0x1C);
} // namespace transport
} // namespace pia
} // namespace nn
