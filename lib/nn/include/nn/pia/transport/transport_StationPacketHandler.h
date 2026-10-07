#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_KeepAliveReceiver.h"
#include "nn/pia/transport/transport_KeepAliveSender.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "pead/RuntimeTypeInfo/peadDerive.h"
#include "nn/pia/transport/transport_PacketStream.h"

namespace nn {
namespace pia {
namespace common {
class Time;
}
namespace transport {
class RelayRouteManager;

// RTTI N2nn3pia9transport20StationPacketHandlerE @ 0x008D0224
// vtable 0x00901F00 (vptr 0x00901F08), offset_to_top 0, 16 entries
//
// The packet handler of the stations: the messages go directly to the stations, through other
// stations (relay routes) or through the relay node, with sequence ids and keep alive messages;
// the received messages are checked against the local station. Its members start in the tail
// padding of PacketHandler. Layout from the constructor and Startup; the member names, the names
// of the unnamed slots and getRuntimeTypeInfoStatic are ours.
class StationPacketHandler : public ::nn::pia::transport::PacketHandler
{
public:
    StationPacketHandler(); // 0x0045A0FC | fefates:bytes [tier B]
    virtual bool checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface* typeInfo) const; // 0x007360FC slot 0x00
    virtual const pead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const; // 0x007360B0 slot 0x04
    virtual ~StationPacketHandler(); // 0x0045A16C slot 0x08
    // 0x0045A12C slot 0x0C | fefates:bytes (deleting dtor)
    virtual ProtocolMessageWriter* AssignByStationIndex(const nn::pia::transport::ProtocolId& protocolId, nn::pia::StationIndex stationIndex, unsigned int size, bool isOwnPacket); // 0x00459974 slot 0x10 | fefates:bytes
    virtual ProtocolMessageWriter* AssignByStationBitmap(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationBitmap, unsigned int size, bool isOwnPacket); // 0x00459A10 slot 0x14 | fefates:bytes
    virtual ProtocolMessageWriter* AssignByStationKey(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationKey, unsigned int size, bool isOwnPacket); // 0x004595B4 slot 0x1C (name is ours)
    virtual common::Packet* AssignPacket(nn::pia::StationIndex stationIndex, unsigned int stationBitmap, const nn::pia::common::StationAddress& address, bool isOwnPacket); // 0x00458D94 slot 0x20 | fefates:bytes
    virtual void RelayMessage(const nn::pia::transport::ProtocolMessageReader& reader); // 0x004590CC slot 0x24 | fefates:bytes
    virtual u32 CheckPacket(const nn::pia::common::Packet& packet); // 0x00458CCC slot 0x34 | fefates:callseq
    virtual bool CheckMessage(const nn::pia::transport::ProtocolMessageReader& reader); // 0x00458FE0 slot 0x38 | fefates:bytes
    virtual bool CheckReceive(const nn::pia::transport::ProtocolMessageReader& reader); // 0x00459050 slot 0x3C | fefates:bytes

    nn::Result Initialize(unsigned int destinationNumMax, bool isBroadcast, unsigned int headerSize); // 0x00458A60 | fefates:bytes [tier B]
    void Finalize(); // 0x0044E1E8 | fefates:callgraph [tier C]
    nn::Result Startup(nn::pia::transport::PacketStream::Writer* pWriter, nn::pia::transport::PacketStream::Reader* pReader, const nn::pia::transport::RelayRouteManager* pRelayRouteManager, const nn::pia::common::StationAddress* pRelayNodeAddress, const nn::pia::common::CryptoSetting* pCryptoSetting); // 0x00459DD4 | fefates:bytes [tier B]
    void Cleanup(); // 0x0044DE9C | fefates:callgraph [tier C]
    void BeginDispatch(const nn::pia::common::Time& now); // 0x00459194 | fefates:bytes [tier B]
    void EndDispatch(const nn::pia::common::Time& now); // 0x0044E60C | fefates:bytes [tier B]
    // the local station, once it has an index
    void UpdateMyStation(); // 0x004592CC | fefates:bytes [tier B]

    ProtocolMessageWriter* AssignSingle(const nn::pia::transport::ProtocolId& protocolId, nn::pia::StationIndex stationIndex, unsigned int size, bool isOwnPacket); // 0x00458E3C | fefates:bytes [tier B]
    ProtocolMessageWriter* AssignMulti(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationBitmap, unsigned int size, bool isOwnPacket, nn::pia::StationIndex sourceStationIndex, unsigned int sourceStationKey, bool isRelayed); // 0x00458A9C | fefates:bytes [tier B]
    ProtocolMessageWriter* AssignAll(const nn::pia::transport::ProtocolId& protocolId, unsigned int size, bool isOwnPacket); // 0x00459EA0 | fefates:bytes [tier B]
    // the buffers of the message in the packets (false if a packet could not be assigned)
    bool AssignDirectMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket); // 0x0045976C | fefates:bytes [tier B]
    bool AssignAllMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket); // 0x00459328 | fefates:bytes [tier B]
    bool AssignHostMessage(unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket); // 0x0045946C | fefates:bytes [tier B]
    bool AssignRelayNodeMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket); // 0x00459A7C | fefates:bytes [tier B]
    bool AssignRelayStationMessage(nn::pia::StationIndex relayStationIndex, unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket); // 0x00459C48 | fefates:bytes [tier B]

    // (inline everywhere)
    static const pead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()
    {
        // 0x009832A8 (guard 0x009832A4)
        static const pead::RuntimeTypeInfo::Derive<PacketHandler> s_TypeInfo;
        return &s_TypeInfo;
    }

    StationIndex m_LocalStationIndex;               // 0x13D
    u32 m_LocalStationBitmap;                       // 0x140
    u32 m_LocalStationKey;                          // 0x144
    const RelayRouteManager* m_pRelayRouteManager;  // 0x148
    common::StationAddress m_RelayNodeAddress;      // 0x14C
    KeepAliveSender m_KeepAliveSender;              // 0x15C
    KeepAliveReceiver m_KeepAliveReceiver;          // 0x164
    u32 m_SentStationBitmap;                        // 0x168, the stations packets went to in this dispatch
};
ASSERT_OFFSET(StationPacketHandler, m_LocalStationIndex, 0x13D);
ASSERT_OFFSET(StationPacketHandler, m_RelayNodeAddress, 0x14C);
ASSERT_OFFSET(StationPacketHandler, m_SentStationBitmap, 0x168);
ASSERT_SIZE(StationPacketHandler, 0x16C);
} // namespace transport
} // namespace pia
} // namespace nn
