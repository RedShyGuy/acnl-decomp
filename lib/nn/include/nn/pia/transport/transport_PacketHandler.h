#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CryptoSetting.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_PacketStream.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "pead/RuntimeTypeInfo/peadInterface.h"
#include "pead/RuntimeTypeInfo/peadRoot.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
class StationAddress;
}
namespace transport {
class PacketAnalysisData;
class PacketAnalyzer;

// RTTI N2nn3pia9transport13PacketHandlerE @ 0x008D0164
// vtable 0x00901D74 (vptr 0x00901D7C), offset_to_top 0, 16 entries
//
// Puts the messages of the protocols into the packets of the send stream and reads the messages
// of the received packets (iteration over the messages of one protocol). The checks of the
// packets and messages are for the derived classes (StationPacketHandler). The base of a pead
// runtime type hierarchy. Layout from the constructor, InitializeCore and StartupCore; the member
// names, the names of the unnamed slots and getRuntimeTypeInfoStatic are ours.
class PacketHandler : public ::nn::pia::common::RootObject
{
public:
    // the iterator over the messages that GetIterator selects; its functions are the ones of the
    // packet handler (IsEndIteration, NextIteration)
    class Iterator;

    PacketHandler(); // 0x0044ED18 | fefates:bytes [tier B]
    virtual bool checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface* typeInfo) const; // 0x00734CE0 slot 0x00
    virtual const pead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const; // 0x00734BEC slot 0x04
    virtual ~PacketHandler(); // 0x0044ED8C slot 0x08 | fefates:bytes
    // 0x0044ED64 slot 0x0C (deleting dtor)
    // a writer for a message of size bytes of payload; null if there is no room. isOwnPacket: the
    // message gets a packet of its own (header flag 8)
    virtual ProtocolMessageWriter* AssignByStationIndex(const nn::pia::transport::ProtocolId& protocolId, nn::pia::StationIndex stationIndex, unsigned int size, bool isOwnPacket); // 0x0044EAEC slot 0x10
    virtual ProtocolMessageWriter* AssignByStationBitmap(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationBitmap, unsigned int size, bool isOwnPacket); // 0x0044EB0C slot 0x14
    virtual ProtocolMessageWriter* AssignByStationAddress(const nn::pia::transport::ProtocolId& protocolId, const nn::pia::common::StationAddress& address, unsigned int size, bool isOwnPacket); // 0x0044EB14 slot 0x18 | fefates:bytes
    // a writer for a message to the station with the key (through the relay node)
    virtual ProtocolMessageWriter* AssignByStationKey(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationKey, unsigned int size, bool isOwnPacket); // 0x0044EABC slot 0x1C (name is ours)
    // a new packet of the send stream for the destination
    virtual common::Packet* AssignPacket(nn::pia::StationIndex stationIndex, unsigned int stationBitmap, const nn::pia::common::StationAddress& address, bool isOwnPacket); // 0x0044E16C slot 0x20 | fefates:bytes
    virtual void RelayMessage(const nn::pia::transport::ProtocolMessageReader& reader); // 0x0044E2AC slot 0x24
    virtual void BeginIteration(); // 0x0044E4E4 slot 0x28 | fefates:bytes
    virtual bool IsEndIteration() const; // 0x00734BD0 slot 0x2C | fefates:bytes
    virtual void NextIteration(); // 0x0044E2B0 slot 0x30 | fefates:bytes-fuzzy
    // the size of the received packet; less than its header if the packet is invalid
    virtual u32 CheckPacket(const nn::pia::common::Packet& packet); // 0x0044DE94 slot 0x34
    virtual bool CheckMessage(const nn::pia::transport::ProtocolMessageReader& reader); // 0x0044E1D8 slot 0x38
    // whether the iteration stops at the message
    virtual bool CheckReceive(const nn::pia::transport::ProtocolMessageReader& reader); // 0x0044E1E0 slot 0x3C

    nn::Result InitializeCore(unsigned int destinationNumMax, bool isBroadcast, unsigned int headerSize); // 0x0044E518 | fefates:bytes [tier B]
    void FinalizeCore(); // 0x0044E21C | fefates:bytes [tier B]
    nn::Result StartupCore(nn::pia::transport::PacketStream::Writer* pWriter, nn::pia::transport::PacketStream::Reader* pReader, unsigned int packetSize, const nn::pia::common::CryptoSetting* pCryptoSetting); // 0x0044DFD8 | fefates:bytes [tier B]
    void CleanupCore(); // 0x0044DED0 | fefates:bytes [tier B]
    // decrypts and checks the received packets, relays the messages for other stations
    void BeginDispatchCore(); // 0x0044E814 | fefates:bytes [tier B]
    // encrypts the packets to send and passes them on
    void EndDispatchCore(); // 0x0044E6A0 | fefates:bytes [tier B]

    Iterator* GetIterator(const nn::pia::transport::ProtocolId& protocolId); // 0x0044DF50 | fefates:bytes [tier B]
    Iterator* GetIterator(unsigned short protocolType); // 0x0044DF88 | fefates:bytes [tier B]
    u8* AssignPacketPayload(nn::pia::common::Packet* pPacket, unsigned int size); // 0x0044EAC4 | fefates:bytes [tier B]
    ProtocolMessageWriter* ReserveMessageWriter(); // 0x0044EAF4 | fefates:bytes [tier B]
    nn::Result Commit(); // 0x0044ECE8 | fefates:bytes [tier B]
    u32 GetPayloadSizeLimit() const; // 0x00734C38 | fefates:bytes [tier B]
    void ClearPacketAnalysisData(); // 0x0044ECAC | fefates:bytes [tier B]
    nn::Result GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData* pSendData, nn::pia::transport::PacketAnalysisData* pReceiveData) const; // 0x00734C44 | fefates:bytes [tier B]

    // (inline everywhere)
    static const pead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()
    {
        // 0x009832A0 (guard 0x0098329C)
        static const pead::RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }

    // the iterator (only the handler)
    class Iterator
    {
    public:
        explicit Iterator(PacketHandler* pPacketHandler) : m_pPacketHandler(pPacketHandler) {}
        // the reader of the current message, null at the end
        const ProtocolMessageReader* GetMessageReader() const; // 0x00734D3C | fefates:bytes [tier B]

        PacketHandler* m_pPacketHandler; // 0x0
    };

    PacketStream::Writer* m_pWriter;          // 0x004
    PacketStream::Reader* m_pReader;          // 0x008
    ProtocolMessageWriter m_MessageWriter;    // 0x00C
    ProtocolMessageReader m_MessageReader;    // 0x0DC
    // the most stations a packet goes to (StationPacketHandler::AssignDirectMessage)
    u32 m_DestinationNumMax;                  // 0x0FC
    // messages to all stations go into one packet (StationPacketHandler::AssignAll)
    bool m_IsBroadcast;                       // 0x100
    u32 m_PayloadSizeLimit;                   // 0x104, the payload of a packet
    u32 m_PacketSizeLimit;                    // 0x108
    ProtocolMessageWriter* m_pReservedWriter; // 0x10C, the writer between Assign* and Commit
    // the selection of the messages of the iteration: (protocol id & mask) == id, big endian
    u32 m_IterationProtocolId;                // 0x110
    u32 m_IterationMask;                      // 0x114
    s32 m_IterationIndex;                     // 0x118, the packet in the received ones
    u32 m_IterationOffset;                    // 0x11C, the message in the packet
    Iterator m_Iterator;                      // 0x120
    PacketAnalyzer* m_pSendAnalyzer;          // 0x124
    PacketAnalyzer* m_pReceiveAnalyzer;       // 0x128
    common::CryptoSetting m_CryptoSetting;    // 0x12C
};
ASSERT_OFFSET(PacketHandler, m_MessageReader, 0xDC);
ASSERT_OFFSET(PacketHandler, m_Iterator, 0x120);
ASSERT_OFFSET(PacketHandler, m_CryptoSetting, 0x12C);
ASSERT_SIZE(PacketHandler, 0x140);
} // namespace transport
} // namespace pia
} // namespace nn
