#include "nn/pia/transport/transport_StationPacketHandler.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "pead/RuntimeTypeInfo/peadDerive.h"
#include "pead/peadBitUtil.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the stations whose messages go through a relay station (name is ours)
struct RelayDestination
{
    RelayDestination(); // 0x0077A504 (name is ours; AssignAll has its own copy at 0x0077A510)

    StationIndex m_StationIndex; // 0x0
    u32 m_StationBitmap;         // 0x4
};

// 0x0077A504 (name is ours; AssignAll has its own copy at 0x0077A510)
RelayDestination::RelayDestination() : m_StationBitmap(0)
{
}

// the size of a message with the payload size (header and payload, 4 aligned)
u32 GetMessageSize(u32 size)
{
    return ((size + ProtocolMessageWriter::HEADER_SIZE - 1) & ~3) + 4;
}

s32 GetNextIndex(const PacketStream::Accessor* pAccessor, s32 index)
{
    index++;
    if (index >= static_cast<s32>(pAccessor->m_pStream->m_PacketNum)) {
        index = 0;
    }
    return index;
}

// the index of the lowest set bit
StationIndex GetLowestStationIndex(u32 bitmap)
{
    return static_cast<StationIndex>(pead::CountOnes((bitmap & -bitmap) - 1));
}
} // namespace

// 0x0044DE9C | fefates:callgraph [tier C]
void nn::pia::transport::StationPacketHandler::Cleanup()
{
    m_pRelayRouteManager = nullptr;
    m_RelayNodeAddress.Clear();
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_LocalStationBitmap = 0;
    m_LocalStationKey = 0;
    CleanupCore();
}

// 0x0044E1E8 | fefates:callgraph [tier C]
void nn::pia::transport::StationPacketHandler::Finalize()
{
    m_pRelayRouteManager = nullptr;
    m_RelayNodeAddress.Clear();
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_LocalStationBitmap = 0;
    m_LocalStationKey = 0;
    FinalizeCore();
}

// 0x0044E60C | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::EndDispatch(const nn::pia::common::Time& now)
{
    // keep alive messages to the stations nothing was sent to for a while
    u32 keepAliveBitmap = m_KeepAliveSender.Update(m_SentStationBitmap, now);
    m_SentStationBitmap = 0;
    if (keepAliveBitmap != 0) {
        ProtocolId protocolId;
        protocolId.SetPort(0);
        protocolId.SetType(PROTOCOL_TYPE_KEEP_ALIVE);
        if (AssignByStationBitmap(protocolId, keepAliveBitmap, 0, false) != nullptr) {
            Commit();
            if (m_SentStationBitmap != 0) {
                m_KeepAliveSender.Update(m_SentStationBitmap, now);
                m_SentStationBitmap = 0;
            }
        }
    }
    EndDispatchCore();
}

// 0x00458A60 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationPacketHandler::Initialize(unsigned int destinationNumMax, bool isBroadcast, unsigned int headerSize)
{
    nn::Result result = InitializeCore(destinationNumMax, isBroadcast, headerSize);
    if (result.IsFailure()) {
        return result;
    }
    m_pRelayRouteManager = nullptr;
    m_RelayNodeAddress.Clear();
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_LocalStationBitmap = 0;
    m_LocalStationKey = 0;
    return nn::Result();
}

// 0x00458A9C | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignMulti(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationBitmap, unsigned int size, bool isOwnPacket, nn::pia::StationIndex sourceStationIndex, unsigned int sourceStationKey, bool isRelayed)
{
    if (m_pWriter == nullptr || m_pReservedWriter != nullptr) {
        return nullptr;
    }
    u32 messageSize = GetMessageSize(size);
    if (messageSize > m_PayloadSizeLimit) {
        return nullptr;
    }
    u32 sizeLimit = m_PacketSizeLimit - messageSize;
    UpdateMyStation();
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nullptr;
    }

    // the stations directly, through relay stations and through the relay node
    RelayDestination relayDestinations[STATION_INDEX_MAX + 1];
    u32 relayNodeBitmap = 0;
    u32 directBitmap = 0;
    s32 relayDestinationNum = 0;
    if (!common::IsValidPointer(m_pRelayRouteManager)) {
        directBitmap = stationBitmap;
        relayNodeBitmap = 0;
    } else {
        for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
            if (!(stationBitmap & (1 << i))) {
                continue;
            }
            StationIndex stationIndex = static_cast<StationIndex>(i);
            StationIndex route;
            if (m_pRelayRouteManager->GetRelayRoute(m_LocalStationIndex, stationIndex, &route).IsFailure() || route == m_LocalStationIndex) {
                stationBitmap &= ~(1 << stationIndex);
                continue;
            }
            u32 bitmap;
            if (route == STATION_INDEX_UNIDENTIFIED) {
                bitmap = 1 << stationIndex;
                if (m_RelayNodeAddress.IsValid()) {
                    relayNodeBitmap |= bitmap;
                }
            } else {
                u32 destinationList;
                m_pRelayRouteManager->GetDestStationList(m_LocalStationIndex, route, &destinationList);
                u32 routeBit = 1 << route;
                bitmap = destinationList & stationBitmap;
                if (routeBit == bitmap) {
                    directBitmap |= routeBit;
                } else {
                    relayDestinations[relayDestinationNum].m_StationIndex = route;
                    relayDestinations[relayDestinationNum].m_StationBitmap = bitmap;
                    relayDestinationNum++;
                }
            }
            stationBitmap &= ~bitmap;
        }
    }
    m_MessageWriter.SetSource(sourceStationIndex, sourceStationKey);
    m_MessageWriter.Reset(protocolId, size, isRelayed, isOwnPacket);
    AssignDirectMessage(directBitmap, messageSize, sizeLimit, isOwnPacket);
    for (s32 i = 0; i < relayDestinationNum; i++) {
        AssignRelayStationMessage(relayDestinations[i].m_StationIndex, relayDestinations[i].m_StationBitmap, messageSize, sizeLimit, isOwnPacket);
    }
    AssignRelayNodeMessage(relayNodeBitmap, messageSize, sizeLimit, isOwnPacket);
    return ReserveMessageWriter();
}

// 0x00458CCC | fefates:callseq
u32 nn::pia::transport::StationPacketHandler::CheckPacket(const nn::pia::common::Packet& packet)
{
    StationManager* pManager = StationManager::s_pInstance;
    Station* pStation = pManager->GetStation(packet.m_SourceStationAddress);
    if (!common::IsValidPointer(pStation)) {
        return packet.m_Size;
    }
    u8 value = packet.m_Unknown0x5;
    if (value != 0 && value != 1 && pStation->m_RemoteConnectionId != 0 && value != pStation->m_RemoteConnectionId) {
        return 0;
    }
    if (m_LocalStationIndex <= STATION_INDEX_MAX) {
        if (!pStation->m_SequenceIdController.CheckReceivedSequenceId(__builtin_bswap16(packet.m_SequenceId))) {
            return 0;
        }
    }
    if (m_LocalStationIndex != pManager->m_Unknown0xA8 && pStation->m_State == Station::STATION_STATE_CONNECTED &&
        pStation->m_StationIndex == STATION_INDEX_UNIDENTIFIED) {
        return 0;
    }
    return packet.m_Size;
}

// 0x00458D94 | fefates:bytes
nn::pia::common::Packet* nn::pia::transport::StationPacketHandler::AssignPacket(nn::pia::StationIndex stationIndex, unsigned int stationBitmap, const nn::pia::common::StationAddress& address, bool isOwnPacket)
{
    common::Packet* pPacket = PacketHandler::AssignPacket(stationIndex, stationBitmap, address, isOwnPacket);
    if (pPacket == nullptr) {
        return nullptr;
    }
    u16 sequenceId = 0;
    u8 value = 0;
    if (m_LocalStationIndex > STATION_INDEX_MAX) {
        value = 1;
    } else if (stationIndex <= STATION_INDEX_MAX) {
        Station* pStation = StationManager::s_pInstance->GetStation(stationIndex);
        if (common::IsValidPointer(pStation)) {
            sequenceId = pStation->m_SequenceIdController.GetNextSendSequenceId();
            value = pStation->m_LocalConnectionId;
        }
    }
    pPacket->m_SequenceId = __builtin_bswap16(sequenceId);
    pPacket->m_Unknown0x5 = value;
    m_SentStationBitmap |= stationBitmap;
    return pPacket;
}

// 0x00458E3C | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignSingle(const nn::pia::transport::ProtocolId& protocolId, nn::pia::StationIndex stationIndex, unsigned int size, bool isOwnPacket)
{
    if (m_pWriter == nullptr || m_pReservedWriter != nullptr) {
        return nullptr;
    }
    u32 messageSize = GetMessageSize(size);
    if (messageSize > m_PayloadSizeLimit) {
        return nullptr;
    }
    u32 sizeLimit = m_PacketSizeLimit - messageSize;
    UpdateMyStation();
    m_MessageWriter.SetSource(m_LocalStationIndex, m_LocalStationKey);
    m_MessageWriter.Reset(protocolId, size, false, isOwnPacket);
    if (stationIndex == 254) {
        AssignHostMessage(messageSize, sizeLimit, isOwnPacket);
        return ReserveMessageWriter();
    }
    if (stationIndex > STATION_INDEX_MAX || m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nullptr;
    }
    u32 stationBit = 1 << stationIndex;
    if (common::IsValidPointer(m_pRelayRouteManager)) {
        StationIndex route;
        if (m_pRelayRouteManager->GetRelayRoute(m_LocalStationIndex, stationIndex, &route).IsFailure() || route == m_LocalStationIndex) {
            return nullptr;
        }
        if (route == STATION_INDEX_UNIDENTIFIED) {
            if (!m_RelayNodeAddress.IsValid()) {
                return nullptr;
            }
            AssignRelayNodeMessage(stationBit, messageSize, sizeLimit, isOwnPacket);
            return ReserveMessageWriter();
        }
        if (stationIndex != route) {
            AssignRelayStationMessage(route, stationBit, messageSize, sizeLimit, isOwnPacket);
            return ReserveMessageWriter();
        }
    }
    AssignDirectMessage(stationBit, messageSize, sizeLimit, isOwnPacket);
    return ReserveMessageWriter();
}

// 0x00458FE0 | fefates:bytes
bool nn::pia::transport::StationPacketHandler::CheckMessage(const nn::pia::transport::ProtocolMessageReader& reader)
{
    u8 flags = reader.m_pHeader[0];
    if (flags & ProtocolMessageReader::FLAG_RELAY_REQUEST) {
        if (flags & ProtocolMessageReader::FLAG_DESTINATION_KEY) {
            return false;
        }
        u32 destination = reader.GetDestination();
        if (m_LocalStationBitmap == 0) {
            return true;
        }
        return (destination & ~m_LocalStationBitmap) != 0;
    }
    if (!(flags & ProtocolMessageReader::FLAG_DESTINATION_KEY)) {
        return true;
    }
    return reader.GetDestination() == m_LocalStationKey;
}

// 0x00459050 | fefates:bytes
bool nn::pia::transport::StationPacketHandler::CheckReceive(const nn::pia::transport::ProtocolMessageReader& reader)
{
    u8 flags = reader.m_pHeader[0];
    if (flags & ProtocolMessageReader::FLAG_DESTINATION_KEY) {
        if (flags & ProtocolMessageReader::FLAG_RELAY_REQUEST) {
            return false;
        }
        return reader.GetDestination() == m_LocalStationKey;
    }
    u32 destination = reader.GetDestination();
    if (destination == 0) {
        return !(reader.m_pHeader[0] & ProtocolMessageReader::FLAG_RELAY_REQUEST);
    }
    return (destination & m_LocalStationBitmap) != 0;
}

// 0x004590CC | fefates:bytes
void nn::pia::transport::StationPacketHandler::RelayMessage(const nn::pia::transport::ProtocolMessageReader& reader)
{
    StationManager* pManager = StationManager::s_pInstance;
    u32 stationBitmap = reader.GetDestination() & pManager->GetParticipatingStationBitmap(false);
    if (stationBitmap == 0) {
        return;
    }
    ProtocolId protocolId = reader.GetProtocolId();
    StationIndex sourceStationIndex = reader.GetSourceStationIndex();
    u32 size = reader.m_PayloadSize;
    u32 sourceStationKey = reader.GetSourceStationKey();
    bool isOwnPacket = (reader.m_pHeader[0] & ProtocolMessageReader::FLAG_OWN_PACKET) >> 3;
    ProtocolMessageWriter* pWriter = AssignMulti(protocolId, stationBitmap, size, isOwnPacket, sourceStationIndex, sourceStationKey, true);
    if (pWriter == nullptr) {
        return;
    }
    pWriter->SetPayload(reader.GetPayload());
    pWriter->m_ReservedData = reader.GetReservedData();
    Commit();
}

// 0x00459194 | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::BeginDispatch(const nn::pia::common::Time& now)
{
    UpdateMyStation();
    BeginDispatchCore();

    // the stations something was received from
    StationManager* pManager = StationManager::s_pInstance;
    PacketStream::Reader* pReader = m_pReader;
    u32 receivedBitmap = 0;
    for (s32 i = pReader->m_Head; i != pReader->m_Position; i = GetNextIndex(pReader, i)) {
        common::Packet* pPacket = pReader->m_pStream->GetPacket(i);
        if (!pPacket->m_HasMessages) {
            continue;
        }
        if (pPacket->m_Unknown0x5 != 1) {
            Station* pStation = pManager->GetStation(pPacket->m_SourceStationAddress);
            if (common::IsValidPointer(pStation) && pStation->m_StationIndex <= STATION_INDEX_MAX) {
                receivedBitmap |= 1 << pStation->m_StationIndex;
            }
        }
        for (u32 offset = 0; offset < pPacket->m_Size - common::Packet::HEADER_SIZE; offset += m_MessageReader.GetMessageSize()) {
            m_MessageReader.Attach(*pPacket, offset);
            if (!m_MessageReader.IsValid()) {
                break;
            }
            StationIndex sourceStationIndex = m_MessageReader.GetSourceStationIndex();
            if (sourceStationIndex <= STATION_INDEX_MAX) {
                receivedBitmap |= 1 << sourceStationIndex;
            }
        }
    }
    m_KeepAliveReceiver.Update(receivedBitmap, now);
}

// 0x004592CC | fefates:bytes [tier B]
void nn::pia::transport::StationPacketHandler::UpdateMyStation()
{
    if (m_LocalStationIndex <= STATION_INDEX_MAX) {
        return;
    }
    Station* pLocalStation = StationManager::s_pInstance->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return;
    }
    m_LocalStationIndex = pLocalStation->m_StationIndex;
    if (m_LocalStationIndex > STATION_INDEX_MAX) {
        return;
    }
    m_LocalStationBitmap = 1 << m_LocalStationIndex;
    m_MessageWriter.SetSource(m_LocalStationIndex, m_LocalStationKey);
}

// 0x00459328 | fefates:bytes [tier B]
bool nn::pia::transport::StationPacketHandler::AssignAllMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket)
{
    if (stationBitmap == 0) {
        return true;
    }
    common::Packet* pPacket = nullptr;
    if (!isOwnPacket) {
        PacketStream::Writer* pWriter = m_pWriter;
        for (s32 i = pWriter->m_Head; i != pWriter->m_Position; i = GetNextIndex(pWriter, i)) {
            common::Packet* p = pWriter->m_pStream->GetPacket(i);
            if (!p->m_Unknown0x5D5 && p->m_Size <= sizeLimit && p->m_DestinationStationIndex == 255) {
                pPacket = p;
                break;
            }
        }
    }
    if (pPacket == nullptr) {
        common::StationAddress address;
        pPacket = AssignPacket(static_cast<StationIndex>(255), stationBitmap, address, isOwnPacket);
        if (pPacket == nullptr) {
            return false;
        }
    }
    u8* pPayload = AssignPacketPayload(pPacket, messageSize);
    m_MessageWriter.AddMessageBuffer(pPacket, pPayload, stationBitmap, false, false);
    return true;
}

// 0x0045946C | fefates:bytes [tier B]
bool nn::pia::transport::StationPacketHandler::AssignHostMessage(unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket)
{
    common::Packet* pPacket = nullptr;
    if (!isOwnPacket) {
        PacketStream::Writer* pWriter = m_pWriter;
        for (s32 i = pWriter->m_Head; i != pWriter->m_Position; i = GetNextIndex(pWriter, i)) {
            common::Packet* p = pWriter->m_pStream->GetPacket(i);
            if (!p->m_Unknown0x5D5 && p->m_Size <= sizeLimit && p->m_DestinationStationIndex == 254) {
                pPacket = p;
                break;
            }
        }
    }
    if (pPacket == nullptr) {
        common::StationAddress address(StationConnectionInfoTable::s_pInstance->m_HostAddress);
        pPacket = AssignPacket(static_cast<StationIndex>(254), 0, address, isOwnPacket);
        if (pPacket == nullptr) {
            return false;
        }
    }
    u8* pPayload = AssignPacketPayload(pPacket, messageSize);
    m_MessageWriter.AddMessageBuffer(pPacket, pPayload, 0, false, false);
    return true;
}

// 0x004595B4 (name is ours)
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignByStationKey(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationKey, unsigned int size, bool isOwnPacket)
{
    ProtocolMessageWriter* pResult = nullptr;
    if (m_pWriter == nullptr || m_pReservedWriter != nullptr || !m_RelayNodeAddress.IsValid()) {
        return pResult;
    }
    u32 messageSize = GetMessageSize(size);
    if (messageSize > m_PayloadSizeLimit) {
        return pResult;
    }
    u32 sizeLimit = m_PacketSizeLimit - messageSize;
    UpdateMyStation();

    // a packet to the relay node that still has room, else a new one
    common::Packet* pPacket = nullptr;
    if (!isOwnPacket) {
        PacketStream::Writer* pWriter = m_pWriter;
        for (s32 i = pWriter->m_Head; i != pWriter->m_Position; i = GetNextIndex(pWriter, i)) {
            common::Packet* p = pWriter->m_pStream->GetPacket(i);
            if (!p->m_Unknown0x5D5 && p->m_Size <= sizeLimit && p->m_DestinationStationIndex == STATION_INDEX_UNIDENTIFIED &&
                p->m_DestinationBitmap == 0 && p->m_DestinationStationAddress == m_RelayNodeAddress) {
                pPacket = p;
                break;
            }
        }
    }
    if (pPacket == nullptr) {
        pPacket = AssignPacket(STATION_INDEX_UNIDENTIFIED, 0, m_RelayNodeAddress, isOwnPacket);
        if (pPacket == nullptr) {
            return pResult;
        }
    }
    u8* pPayload = AssignPacketPayload(pPacket, messageSize);
    m_MessageWriter.Reset(protocolId, size, false, isOwnPacket);
    m_MessageWriter.AddMessageBuffer(pPacket, pPayload, stationKey, true, true);
    pResult = ReserveMessageWriter();
    return pResult;
}

// 0x0045976C | fefates:bytes [tier B]
bool nn::pia::transport::StationPacketHandler::AssignDirectMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket)
{
    if (stationBitmap == 0) {
        return true;
    }
    StationManager* pManager = StationManager::s_pInstance;
    PacketStream::Writer* pWriter = m_pWriter;
    s32 i = isOwnPacket ? pWriter->m_Position : pWriter->m_Head;
    do {
        common::Packet* pPacket;
        u32 bitmap;
        if (i != pWriter->m_Position) {
            // a packet to some of the stations that still has room
            pPacket = pWriter->m_pStream->GetPacket(i);
            i = GetNextIndex(pWriter, i);
            if (pPacket->m_Unknown0x5D5 || pPacket->m_Size > sizeLimit) {
                continue;
            }
            bitmap = pPacket->m_DestinationBitmap;
            if (bitmap == 0 || (bitmap & ~stationBitmap) != 0) {
                continue;
            }
            stationBitmap &= ~bitmap;
        } else {
            // a new packet to the next (at most m_DestinationNumMax) stations
            s32 bitIndex = pead::BitFlagUtil::findOnBitFromRight(stationBitmap, m_DestinationNumMax);
            if (bitIndex < 0) {
                bitmap = stationBitmap;
            } else {
                bitmap = ((1 << (bitIndex + 1)) - 1) & stationBitmap;
            }
            stationBitmap &= ~bitmap;
            s32 stationNum = pead::CountOnes(bitmap);
            common::StationAddress address;
            StationIndex stationIndex;
            if (stationNum == 1) {
                stationIndex = GetLowestStationIndex(bitmap);
                if (pManager->GetStationAddress(&address, stationIndex).IsFailure()) {
                    continue;
                }
            } else {
                stationIndex = STATION_INDEX_UNIDENTIFIED;
                address.Clear();
            }
            pPacket = AssignPacket(stationIndex, bitmap, address, isOwnPacket);
            if (pPacket == nullptr) {
                return false;
            }
            i = GetNextIndex(pWriter, i);
        }
        u8* pPayload = AssignPacketPayload(pPacket, messageSize);
        m_MessageWriter.AddMessageBuffer(pPacket, pPayload, bitmap, false, false);
    } while (stationBitmap != 0);
    return true;
}

// 0x00459974 | fefates:bytes
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignByStationIndex(const nn::pia::transport::ProtocolId& protocolId, nn::pia::StationIndex stationIndex, unsigned int size, bool isOwnPacket)
{
    if (stationIndex != 255) {
        return AssignSingle(protocolId, stationIndex, size, isOwnPacket);
    }
    if (m_IsBroadcast) {
        return AssignAll(protocolId, size, isOwnPacket);
    }
    u32 stationBitmap = StationManager::s_pInstance->GetParticipatingStationBitmap(false);
    return AssignMulti(protocolId, stationBitmap, size, isOwnPacket, m_LocalStationIndex, m_LocalStationKey, false);
}

// 0x00459A10 | fefates:bytes
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignByStationBitmap(const nn::pia::transport::ProtocolId& protocolId, unsigned int stationBitmap, unsigned int size, bool isOwnPacket)
{
    stationBitmap &= StationManager::s_pInstance->GetParticipatingStationBitmap(false);
    return AssignMulti(protocolId, stationBitmap, size, isOwnPacket, m_LocalStationIndex, m_LocalStationKey, false);
}

// 0x00459A7C | fefates:bytes [tier B]
bool nn::pia::transport::StationPacketHandler::AssignRelayNodeMessage(unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket)
{
    if (stationBitmap == 0) {
        return true;
    }
    StationConnectionInfoTable* pTable = StationConnectionInfoTable::s_pInstance;
    PacketStream::Writer* pWriter = m_pWriter;
    s32 i = isOwnPacket ? pWriter->m_Position : pWriter->m_Head;
    do {
        // a packet to the relay node that still has room, else a new one
        common::Packet* pPacket;
        if (i != pWriter->m_Position) {
            pPacket = pWriter->m_pStream->GetPacket(i);
            i = GetNextIndex(pWriter, i);
            if (pPacket->m_Unknown0x5D5 || pPacket->m_Size > sizeLimit || pPacket->m_DestinationStationIndex != STATION_INDEX_UNIDENTIFIED ||
                pPacket->m_DestinationBitmap != 0 || !(pPacket->m_DestinationStationAddress == m_RelayNodeAddress)) {
                continue;
            }
        } else {
            pPacket = AssignPacket(STATION_INDEX_UNIDENTIFIED, 0, m_RelayNodeAddress, isOwnPacket);
            if (pPacket == nullptr) {
                return false;
            }
            i = GetNextIndex(pWriter, i);
        }
        // a message to each station by its key while the packet has room
        while (stationBitmap != 0 && pPacket->m_Size <= sizeLimit) {
            StationIndex stationIndex = GetLowestStationIndex(stationBitmap);
            u32 stationKey;
            if (pTable->GetStationKey(stationIndex, &stationKey).IsSuccess()) {
                u8* pPayload = AssignPacketPayload(pPacket, messageSize);
                m_MessageWriter.AddMessageBuffer(pPacket, pPayload, stationKey, true, true);
                m_SentStationBitmap |= 1 << stationIndex;
            }
            stationBitmap &= ~(1 << stationIndex);
        }
    } while (stationBitmap != 0);
    return true;
}

// 0x00459C48 | fefates:bytes [tier B]
bool nn::pia::transport::StationPacketHandler::AssignRelayStationMessage(nn::pia::StationIndex relayStationIndex, unsigned int stationBitmap, unsigned int messageSize, unsigned int sizeLimit, bool isOwnPacket)
{
    if (stationBitmap == 0) {
        return true;
    }
    common::Packet* pPacket = nullptr;
    if (!isOwnPacket) {
        PacketStream::Writer* pWriter = m_pWriter;
        for (s32 i = pWriter->m_Head; i != pWriter->m_Position; i = GetNextIndex(pWriter, i)) {
            common::Packet* p = pWriter->m_pStream->GetPacket(i);
            if (!p->m_Unknown0x5D5 && p->m_Size <= sizeLimit && p->m_DestinationStationIndex == relayStationIndex) {
                pPacket = p;
                break;
            }
        }
    }
    if (pPacket == nullptr) {
        u32 relayStationBit = 1 << relayStationIndex;
        common::StationAddress address;
        if (StationManager::s_pInstance->GetStationAddress(&address, relayStationIndex).IsFailure()) {
            return false;
        }
        pPacket = AssignPacket(relayStationIndex, relayStationBit, address, isOwnPacket);
        if (pPacket == nullptr) {
            return false;
        }
    }
    u8* pPayload = AssignPacketPayload(pPacket, messageSize);
    m_MessageWriter.AddMessageBuffer(pPacket, pPayload, stationBitmap, false, true);
    m_SentStationBitmap |= stationBitmap;
    return true;
}

// 0x00459DD4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationPacketHandler::Startup(nn::pia::transport::PacketStream::Writer* pWriter, nn::pia::transport::PacketStream::Reader* pReader, const nn::pia::transport::RelayRouteManager* pRelayRouteManager, const nn::pia::common::StationAddress* pRelayNodeAddress, const nn::pia::common::CryptoSetting* pCryptoSetting)
{
    // the packets without the signature
    common::PayloadSizeManager* pSizeManager = common::PayloadSizeManager::GetInstance();
    u32 packetSize = (pSizeManager->m_MtuSize & ~3) - pSizeManager->m_SignatureSize;
    nn::Result result = StartupCore(pWriter, pReader, packetSize, pCryptoSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pRelayRouteManager = pRelayRouteManager;
    if (common::IsValidPointer(pRelayNodeAddress)) {
        m_RelayNodeAddress = *pRelayNodeAddress;
    } else {
        m_RelayNodeAddress.Clear();
    }
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_LocalStationBitmap = 0;
    if (StationConnectionInfoTable::s_pInstance->GetLocalStationKey(&m_LocalStationKey).IsFailure()) {
        return common::RESULT_INVALID_STATE;
    }
    m_MessageWriter.SetSource(m_LocalStationIndex, m_LocalStationKey);
    m_SentStationBitmap = 0;
    return nn::Result();
}

// 0x00459EA0 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::StationPacketHandler::AssignAll(const nn::pia::transport::ProtocolId& protocolId, unsigned int size, bool isOwnPacket)
{
    if (m_pWriter == nullptr || m_pReservedWriter != nullptr) {
        return nullptr;
    }
    u32 messageSize = GetMessageSize(size);
    if (messageSize > m_PayloadSizeLimit) {
        return nullptr;
    }
    u32 sizeLimit = m_PacketSizeLimit - messageSize;
    UpdateMyStation();
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nullptr;
    }
    u32 stationBitmap = StationManager::s_pInstance->GetParticipatingStationBitmap(false);
    if (stationBitmap == 0) {
        return nullptr;
    }

    // the stations in one packet to all, through relay stations and through the relay node
    RelayDestination relayDestinations[STATION_INDEX_MAX + 1];
    u32 relayNodeBitmap = 0;
    u32 directBitmap;
    s32 relayDestinationNum = 0;
    if (!common::IsValidPointer(m_pRelayRouteManager)) {
        directBitmap = stationBitmap;
        relayNodeBitmap = 0;
    } else {
        u32 directStationList;
        if (m_pRelayRouteManager->GetDirectStationList(m_LocalStationIndex, &directStationList).IsFailure()) {
            return nullptr;
        }
        directBitmap = directStationList & stationBitmap;
        u32 restBitmap = stationBitmap & ~directBitmap;
        if (restBitmap != 0) {
            for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
                if (!(restBitmap & (1 << i))) {
                    continue;
                }
                StationIndex stationIndex = static_cast<StationIndex>(i);
                StationIndex route;
                if (m_pRelayRouteManager->GetRelayRoute(m_LocalStationIndex, stationIndex, &route).IsFailure() || route == m_LocalStationIndex) {
                    restBitmap &= ~(1 << stationIndex);
                    continue;
                }
                if (route == STATION_INDEX_UNIDENTIFIED) {
                    u32 bit = 1 << stationIndex;
                    if (m_RelayNodeAddress.IsValid()) {
                        relayNodeBitmap |= bit;
                    }
                    restBitmap &= ~bit;
                    continue;
                }
                u32 destinationList;
                m_pRelayRouteManager->GetDestStationList(m_LocalStationIndex, route, &destinationList);
                u32 bitmap = destinationList & restBitmap;
                relayDestinations[relayDestinationNum].m_StationIndex = route;
                relayDestinations[relayDestinationNum].m_StationBitmap = bitmap;
                relayDestinationNum++;
                restBitmap &= ~bitmap;
            }
        }
    }
    m_MessageWriter.SetSource(m_LocalStationIndex, m_LocalStationKey);
    m_MessageWriter.Reset(protocolId, size, false, isOwnPacket);
    AssignAllMessage(directBitmap, messageSize, sizeLimit, isOwnPacket);
    for (s32 i = 0; i < relayDestinationNum; i++) {
        AssignRelayStationMessage(relayDestinations[i].m_StationIndex, relayDestinations[i].m_StationBitmap, messageSize, sizeLimit, isOwnPacket);
    }
    AssignRelayNodeMessage(relayNodeBitmap, messageSize, sizeLimit, isOwnPacket);
    return ReserveMessageWriter();
}

// 0x0045A0FC | fefates:bytes [tier B]
nn::pia::transport::StationPacketHandler::StationPacketHandler()
{
    // only the members with constructors
}

// 0x0045A16C
// 0x0045A12C (deleting dtor)
nn::pia::transport::StationPacketHandler::~StationPacketHandler()
{
    // empty (in the original too)
}

// 0x007360B0
const pead::RuntimeTypeInfo::Interface* nn::pia::transport::StationPacketHandler::getRuntimeTypeInfo() const
{
    return getRuntimeTypeInfoStatic();
}

// 0x007360FC
bool nn::pia::transport::StationPacketHandler::checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface* typeInfo) const
{
    if (typeInfo == getRuntimeTypeInfoStatic()) {
        return true;
    }
    return PacketHandler::checkDerivedRuntimeTypeInfo(typeInfo);
}

} // namespace transport
} // namespace pia
} // namespace nn

// 0x00828240
template bool pead::RuntimeTypeInfo::Derive<nn::pia::transport::PacketHandler>::isDerived(const pead::RuntimeTypeInfo::Interface*) const;
