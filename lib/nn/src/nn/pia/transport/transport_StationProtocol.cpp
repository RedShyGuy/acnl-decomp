#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_ReceivedMessageAccessor.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocolManager.h"
#include "nn/pia/transport/transport_StationProtocolReliable.h"
#include "nn/pia/transport/transport_Transport.h"
#include <string.h>

namespace nn {
namespace pia {
namespace transport {
namespace {
// the flags of the Trace calls
const u64 TRACE_FLAG = 0x80000;
const u64 TRACE_FLAG_STATION = 0x80000000;
const u64 TRACE_FLAG_STATION_CONNECTION = 0x80080000;
// the size of a received reliable message at most
const u32 RELIABLE_MESSAGE_SIZE_MAX = 0x596;
} // namespace

// 0x00451458 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::transport::StationProtocol::ParseHelper(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    if (accessor.m_Size == 0) {
        return common::RESULT_INVALID_FORMAT;
    }
    const u8* pData = accessor.m_pData;
    StationManager* pManager = StationManager::s_pInstance;
    switch (pData[0]) {
    case MESSAGE_TYPE_CONNECTION_REQUEST:
        ParseConnectionRequest(accessor, false);
        break;
    case MESSAGE_TYPE_CONNECTION_RESPONSE:
        ParseConnectionResponseCommon(accessor, false);
        break;
    case MESSAGE_TYPE_DISCONNECTION_REQUEST:
        ParseDisconnectionRequest(accessor);
        break;
    case MESSAGE_TYPE_DISCONNECTION_RESPONSE: {
        if (!common::IsValidPointer(pManager->m_pLocalStation)) {
            break;
        }
        Station* pStation = pManager->GetStation(accessor.m_SourceStationIndex);
        if (pStation == nullptr) {
            pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceAddress);
            if (pStation == nullptr) {
                break;
            }
        }
        DisconnectStationJob* pJob = pStation->m_pDisconnectStationJob;
        if (pJob->m_IsWaitingResponse) {
            pJob->m_IsWaitingResponse = false;
        }
        break;
    }
    case MESSAGE_TYPE_ACK: {
        if (!common::IsValidPointer(pManager->m_pLocalStation)) {
            break;
        }
        u32 ackId = common::deserializeU32(pData + 4);
        Station* pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceAddress);
        if (!common::IsValidPointer(pStation)) {
            ResendingMessageManager::s_pInstance->StopResending(ackId);
            break;
        }
        ResendingMessageManager::s_pInstance->StopResending(ackId);
        // the keep alive of the stations without an index
        if (pStation->m_StationIndex > STATION_INDEX_MAX) {
            pStation->m_LastReceiveTime = Transport::s_pInstance->m_DispatchTime;
        }
        break;
    }
    case MESSAGE_TYPE_RELAY_CONNECTION_REQUEST:
        ParseConnectionRequest(accessor, true);
        break;
    case MESSAGE_TYPE_RELAY_CONNECTION_RESPONSE:
        ParseConnectionResponseCommon(accessor, true);
        break;
    default:
        return common::RESULT_INVALID_FORMAT;
    }
    return nn::Result();
}

// 0x004515E4 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDisconnectionRequest(nn::pia::StationIndex stationIndex, bool isOwnPacket)
{
    ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, 1, isOwnPacket);
    if (common::IsValidPointer(pWriter)) {
        pWriter->GetPayload()[0] = MESSAGE_TYPE_DISCONNECTION_REQUEST;
        m_pPacketHandler->Commit();
    }
}

// 0x0045164C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDisconnectionRequest(const nn::pia::common::StationAddress& address)
{
    ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, address, 1, false);
    if (common::IsValidPointer(pWriter)) {
        pWriter->GetPayload()[0] = MESSAGE_TYPE_DISCONNECTION_REQUEST;
        m_pPacketHandler->Commit();
    }
}

// 0x004516B0 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::ParseDisconnectionRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager->m_pLocalStation)) {
        return;
    }
    Station* pStation = pManager->GetStation(accessor.m_SourceStationIndex);
    if (pStation == nullptr) {
        pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceAddress);
        if (pStation == nullptr) {
            // an unknown station: answer anyway
            if (accessor.m_SourceAddress.IsValid()) {
                ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, accessor.m_SourceAddress, 1, false);
                if (common::IsValidPointer(pWriter)) {
                    pWriter->GetPayload()[0] = MESSAGE_TYPE_DISCONNECTION_RESPONSE;
                    m_pPacketHandler->Commit();
                }
            }
            return;
        }
    }
    // the request must come from the station itself
    if (accessor.m_SourceStationKey != 0) {
        u32 stationKey = 0;
        if (StationConnectionInfoTable::s_pInstance->GetStationKey(pStation, &stationKey).IsFailure()) {
            return;
        }
        if (accessor.m_SourceStationKey != stationKey) {
            return;
        }
    } else {
        if (accessor.m_ConnectionId == 0 || accessor.m_ConnectionId != pStation->m_RemoteConnectionId) {
            return;
        }
    }
    if (pStation->m_StationAddress.IsValid()) {
        ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, 1, false);
        if (common::IsValidPointer(pWriter)) {
            pWriter->GetPayload()[0] = MESSAGE_TYPE_DISCONNECTION_RESPONSE;
            m_pPacketHandler->Commit();
        }
    }
    RelayRouteManager* pRelayRouteManager = Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager != nullptr && pStation->m_StationIndex <= STATION_INDEX_MAX) {
        pRelayRouteManager->SearchRefugeRelayRoute(pStation->m_StationIndex);
    }
    pStation->m_State = Station::STATION_STATE_DISCONNECTED;
}

// 0x00451860 (name is ours)
void nn::pia::transport::StationProtocol::ParseConnectionRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor, bool isRelay)
{
    if (!common::IsValidPointer(StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    const u8* pData = accessor.m_pData;
    if (pData[2] != VERSION) {
        SendDenyingConnectionResponse(accessor.m_SourceAddress, DENY_REASON_INCOMPATIBLE_VERSION);
        return;
    }
    // 1: the connection back to a station that asked for one (ProcessConnectionRequestJob)
    u8 isInverseConnection = pData[3];
    if (isInverseConnection != 0 && isInverseConnection != 1) {
        return;
    }
    u32 ackId = ResendingMessageManager::s_pInstance->ExtractAckIdFromMessage(pData, accessor.m_Size);
    u8 connectionId = pData[1];
    StationConnectionInfo info;
    info.Deserialize(pData + 4);

    Station* pStation = StationConnectionInfoTable::s_pInstance->GetStation(info);
    if (pStation == nullptr && !isRelay) {
        pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceAddress);
        // a station behind the same NAT with only a public address is known by its address alone
        if (info.m_PublicLocation.m_StationAddress.GetInetAddress().IsValid() &&
            !info.m_PrivateLocation.m_StationAddress.GetInetAddress().IsValid() &&
            StationConnectionInfoTable::s_pInstance->GetStationPartialMatch(info.m_PublicLocation, StationConnectionInfoTable::PartialMatchMode(1)) != nullptr &&
            StationConnectionInfoTable::s_pInstance->GetStationPartialMatch(info.m_PublicLocation, StationConnectionInfoTable::PartialMatchMode(0)) == nullptr) {
            info.m_PublicLocation.Trace(TRACE_FLAG);
            SendDenyingConnectionResponse(accessor.m_SourceAddress, DENY_REASON_REFUSED);
            return;
        }
        if (info.m_PublicLocation.m_StationAddress.IsValid() && pStation == nullptr) {
            // the station may come back with another address
            pStation = StationConnectionInfoTable::s_pInstance->GetStationPartialMatch(info.m_PublicLocation, StationConnectionInfoTable::PartialMatchMode(0));
            if (pStation != nullptr) {
                pStation->Trace(TRACE_FLAG_STATION_CONNECTION);
                if (!isInverseConnection) {
                    pStation->m_State = Station::STATION_STATE_DISCONNECTED;
                    return;
                }
                pStation->m_StationAddress.Trace(TRACE_FLAG);
                accessor.m_SourceAddress.Trace(TRACE_FLAG);
                m_AddressChangedNum++;
                pStation->m_StationAddress = accessor.m_SourceAddress;
            }
        }
    }

    if (pStation != nullptr) {
        // a known station: a new connection id starts a new connection
        if (!isInverseConnection) {
            pStation->Trace(TRACE_FLAG_STATION_CONNECTION);
            if (pStation->m_RemoteConnectionId == connectionId) {
                return;
            }
            pStation->m_RemoteConnectionId = connectionId;
        }
        if (pStation->m_RemoteConnectionId == 0) {
            pStation->m_RemoteConnectionId = connectionId;
            pStation->Trace(TRACE_FLAG_STATION);
        } else if (pStation->m_RemoteConnectionId != connectionId) {
            pStation->Trace(TRACE_FLAG_STATION);
            return;
        }
        if (pStation->m_State == Station::STATION_STATE_2) {
            ConnectStationJob* pJob = pStation->m_pConnectStationJob;
            if (pJob->GetState() == common::Job::EXECUTE_STATE_FINISHED) {
                return;
            }
            if (pJob->GetState() != common::Job::EXECUTE_STATE_WAITING && pJob->GetState() != common::Job::EXECUTE_STATE_RUNNING) {
                return;
            }
        } else {
            // the request is known: ack it again
            if (isRelay) {
                if (pStation->m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
                    SendAck(ackId, pStation->m_StationIndex);
                }
            } else {
                SendAck(ackId, pStation->m_StationAddress);
            }
            if (pStation->m_StationIndex > STATION_INDEX_MAX) {
                pStation->m_LastReceiveTime = Transport::s_pInstance->m_DispatchTime;
            }
            return;
        }
    } else {
        // a new station
        if (isInverseConnection == 1) {
            return;
        }
        Transport* pTransport = Transport::s_pInstance;
        if (pTransport->m_StationNum <= StationManager::s_pInstance->m_ActiveStations.GetNum()) {
            if (!isRelay) {
                SendDenyingConnectionResponse(accessor.m_SourceAddress, DENY_REASON_REFUSED);
            }
            return;
        }
        u32 principalId = 0;
        if (info.m_PublicLocation.m_PrincipalId != 0) {
            principalId = info.m_PublicLocation.m_PrincipalId;
        } else if (info.m_PrivateLocation.m_PrincipalId != 0) {
            principalId = info.m_PrivateLocation.m_PrincipalId;
        }
        if (pTransport->m_IsUsingStationIdTable) {
            StationIdTable::Entry* pEntry = nullptr;
            if (principalId != 0) {
                pEntry = pTransport->m_pStationIdTable->FindCore(principalId);
            }
            if (!(static_cast<u32>(pTransport->m_pStationIdTable->GetEntryNum()) < pTransport->m_StationNum &&
                  static_cast<u32>(pTransport->m_pStationIdTable->GetEntryNum()) < pTransport->m_pStationIdTable->m_EntryNumMax) &&
                pEntry == nullptr) {
                if (!isRelay) {
                    SendDenyingConnectionResponse(accessor.m_SourceAddress, DENY_REASON_REFUSED);
                }
                return;
            }
        }
        // the old station of the same player goes
        if (principalId != 0) {
            StationManager* pManager = StationManager::s_pInstance;
            for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
                Station* pOld = *it;
                if (pOld->m_StationId == StationId(principalId, 0)) {
                    if (pOld->m_State == Station::STATION_STATE_CONNECTED) {
                        pOld->m_Unknown0x68 = true;
                    } else {
                        pOld->m_State = Station::STATION_STATE_DISCONNECTED;
                    }
                }
            }
        }
        pStation = StationManager::s_pInstance->CreateStation();
        if (isRelay) {
            pStation->Startup(this, info.m_PublicLocation.m_StationAddress);
        } else {
            pStation->Startup(this, accessor.m_SourceAddress);
        }
        pStation->m_State = Station::STATION_STATE_REQUESTED;
        if (principalId != 0) {
            pStation->SetStationId(StationId(principalId, 0));
        }
        pStation->m_RemoteConnectionId = connectionId;
        common::Time now;
        now.SetNow();
        pStation->m_LocalConnectionId = static_cast<u8>(static_cast<u64>(now.m_Tick) % 254 + 2);
        ConnectStationJob* pJob = pStation->m_pConnectStationJob;
        if (pJob->GetState() != common::Job::EXECUTE_STATE_IDLE && pJob->GetState() != common::Job::EXECUTE_STATE_FINISHED) {
            pJob->Cleanup();
            pJob->Reset(true);
        }
    }

    // start the job that answers
    if (StationConnectionInfoTable::s_pInstance->AddToTable(pStation, info).IsFailure()) {
        StationConnectionInfoTable::s_pInstance->Trace(TRACE_FLAG);
        return;
    }
    ProcessConnectionRequestJob* pJob = pStation->m_pProcessConnectionRequestJob;
    bool isStarted;
    if (isRelay) {
        isStarted = pJob->StartupRelayConnection(pStation, m_ProcessTimeoutMSec, isInverseConnection == 1);
    } else {
        isStarted = pJob->Startup(pStation, m_ProcessTimeoutMSec, isInverseConnection == 1);
    }
    if (!isStarted) {
        StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
        return;
    }
    pJob->Ready(false);
    if (isRelay) {
        if (pStation->m_StationIndex <= STATION_INDEX_MAX) {
            SendAck(ackId, pStation->m_StationIndex);
        }
    } else {
        SendAck(ackId, pStation->m_StationAddress);
    }
    if (pStation->m_StationIndex > STATION_INDEX_MAX) {
        pStation->m_LastReceiveTime = Transport::s_pInstance->m_DispatchTime;
        pStation->m_LastSendTime = Transport::s_pInstance->m_DispatchTime;
    }
}

// 0x0045205C | fefates:callseq [tier C]
void nn::pia::transport::StationProtocol::ParseConnectionResponseCommon(const nn::pia::transport::ReceivedMessageAccessor& accessor, bool isRelay)
{
    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager->m_pLocalStation)) {
        return;
    }
    u32 ackId = ResendingMessageManager::s_pInstance->ExtractAckIdFromMessage(accessor.m_pData, accessor.m_Size);
    Station* pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceAddress);
    if (pStation == nullptr) {
        if (!isRelay) {
            return;
        }
        pStation = StationManager::s_pInstance->GetStation(accessor.m_SourceStationIndex);
        if (pStation == nullptr) {
            return;
        }
    }
    const u8* pData = accessor.m_pData;
    if (pData[1] != 0) {
        // denied
        pStation->m_pConnectStationJob->m_DenyReason = pData[1];
        return;
    }
    pStation->m_LastReceiveTime = Transport::s_pInstance->m_DispatchTime;
    pStation->m_LastSendTime = Transport::s_pInstance->m_DispatchTime;
    if (isRelay) {
        if (pStation->m_StationIndex != STATION_INDEX_UNIDENTIFIED) {
            SendAck(ackId, pStation->m_StationIndex);
        }
    } else {
        SendAck(ackId, pStation->m_StationAddress);
    }
    if (pStation->m_State != Station::STATION_STATE_CONNECTING) {
        pStation->Trace(TRACE_FLAG_STATION);
        return;
    }
    Station::IdentificationInfo info;
    memcpy(&info.m_PlayerName, pData + 4, sizeof(info.m_PlayerName));
    for (u32 i = 0; i < 16; i++) {
        info.m_Unknown0x20[i] = common::deserializeU16(pData + 36 + i * 2);
    }
    info.m_Unknown0x40 = 0;
    info.m_Unknown0x42 = pData[68];
    info.m_Unknown0x43 = pData[69];
    info.m_Unknown0x44 = pData[3];
    info.m_Unknown0x48 = common::deserializeU32(pData + 70);
    if (IdentificationInfoTable::s_pInstance->AddToTable(pStation, &info).IsSuccess()) {
        pStation->m_State = Station::STATION_STATE_CONNECTED;
    } else {
        IdentificationInfoTable::s_pInstance->Trace(TRACE_FLAG);
    }
}

// 0x0045229C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDenyingConnectionResponse(const nn::pia::common::StationAddress& address, unsigned char reason)
{
    ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, address, 4, false);
    if (common::IsValidPointer(pWriter)) {
        pWriter->GetPayload()[0] = MESSAGE_TYPE_CONNECTION_RESPONSE;
        pWriter->GetPayload()[1] = reason;
        pWriter->GetPayload()[2] = VERSION;
        pWriter->GetPayload()[3] = 0;
        m_pPacketHandler->Commit();
    }
}

// 0x00452324
void nn::pia::transport::StationProtocol::Cleanup()
{
    m_AddressChangedNum = 0;
}

// 0x00452330 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendAck(unsigned int ackId, nn::pia::StationIndex stationIndex)
{
    ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, sizeof(AckMessage), false);
    if (common::IsValidPointer(pWriter)) {
        AckMessage message = {};
        message.m_Type = MESSAGE_TYPE_ACK;
        common::serializeU32(message.m_AckId, ackId);
        pWriter->SetPayload(&message);
        m_pPacketHandler->Commit();
    }
}

// 0x004523B8 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendAck(unsigned int ackId, const nn::pia::common::StationAddress& address)
{
    ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, address, sizeof(AckMessage), false);
    if (common::IsValidPointer(pWriter)) {
        AckMessage message = {};
        message.m_Type = MESSAGE_TYPE_ACK;
        common::serializeU32(message.m_AckId, ackId);
        pWriter->SetPayload(&message);
        m_pPacketHandler->Commit();
    }
}

// 0x00452440
nn::Result nn::pia::transport::StationProtocol::Startup(nn::pia::StationIndex)
{
    m_AddressChangedNum = 0;
    return nn::Result();
}

// 0x00452450 | fefates:bytes
nn::Result nn::pia::transport::StationProtocol::Dispatch()
{
    ReceivedMessageAccessor accessor;
    ProtocolId protocolId = m_ProtocolId;
    PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        accessor.m_pData = pReader->GetPayload();
        accessor.m_Size = pReader->m_PayloadSize;
        accessor.m_SourceStationIndex = pReader->GetSourceStationIndex();
        accessor.m_SourceAddress = pReader->m_SourceAddress;
        accessor.m_SourceStationKey = pReader->GetSourceStationKey();
        accessor.m_ConnectionId = pReader->m_Unknown0x1C;
        nn::Result result = ParseHelper(accessor);
        if (result.IsFailure()) {
            return result;
        }
        pIterator->m_pPacketHandler->NextIteration();
    }

    // the messages that came through the reliable stream
    u8 buffer[RELIABLE_MESSAGE_SIZE_MAX];
    ReceivedMessageAccessor reliableAccessor;
    reliableAccessor.m_pData = buffer;
    for (u32 i = 0; i <= STATION_INDEX_MAX; i++) {
        for (;;) {
            StationProtocolReliable* pReliable = nullptr;
            if (common::IsValidPointer(StationProtocolManager::s_pInstance)) {
                pReliable = StationProtocolManager::s_pInstance->GetStationProtocolReliable();
                if (!common::IsValidPointer(pReliable)) {
                    pReliable = nullptr;
                }
            }
            if (pReliable->Receive(static_cast<StationIndex>(i), RELIABLE_MESSAGE_SIZE_MAX, buffer, &reliableAccessor.m_Size,
                                   &reliableAccessor.m_SourceAddress)
                    .IsFailure()) {
                break;
            }
            reliableAccessor.m_SourceStationIndex = static_cast<StationIndex>(i);
            if (ParseHelper(reliableAccessor).IsFailure()) {
                break;
            }
        }
    }
    return nn::Result();
}

// 0x0045262C | fefates:bytes [tier B]
nn::pia::transport::StationProtocol::StationProtocol() : m_ProcessTimeoutMSec(15000)
{
}

// 0x00452660
// 0x00452650 (deleting dtor)
nn::pia::transport::StationProtocol::~StationProtocol()
{
    // empty (in the original too)
}

// 0x0073551C
u16 nn::pia::transport::StationProtocol::GetProtocolType() const
{
    return PROTOCOL_TYPE_STATION;
}

// 0x00735524
bool nn::pia::transport::StationProtocol::IsEnableProtocolFiltering() const
{
    return false;
}

// 0x0073552C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::MakeConnectionRequestData(unsigned char* pData, unsigned char connectionId, bool isRelay, bool isInverseConnection) const
{
    pData[0] = isRelay ? MESSAGE_TYPE_RELAY_CONNECTION_REQUEST : MESSAGE_TYPE_CONNECTION_REQUEST;
    pData[1] = connectionId;
    pData[2] = VERSION;
    pData[3] = isInverseConnection;
    const StationConnectionInfo& info = StationConnectionInfoTable::s_pInstance->m_LocalInfo;
    unsigned int size;
    info.Serialize(pData + 4, &size, info.GetSerializedSize());
}

// 0x00735598 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::MakeConnectionResponseData(unsigned char* pData, bool isRelay) const
{
    Station::IdentificationInfo info;
    IdentificationInfoTable::s_pInstance->GetLocalIdentificationInfo(&info);
    pData[1] = 0;
    pData[0] = isRelay ? MESSAGE_TYPE_RELAY_CONNECTION_RESPONSE : MESSAGE_TYPE_CONNECTION_RESPONSE;
    pData[2] = VERSION;
    pData[3] = info.m_Unknown0x44;
    memcpy(pData + 4, &info.m_PlayerName, sizeof(info.m_PlayerName));
    for (u32 i = 0; i < 16; i++) {
        common::serializeU16(pData + 36 + i * 2, info.m_Unknown0x20[i]);
    }
    pData[68] = info.m_Unknown0x42;
    pData[69] = info.m_Unknown0x43;
    common::serializeU32(pData + 70, info.m_Unknown0x48);
}

// 0x00735640 | fefates:bytes [tier B]
u32 nn::pia::transport::StationProtocol::GetConnectionRequestDataSize() const
{
    return StationConnectionInfoTable::s_pInstance->m_LocalInfo.GetSerializedSize() + 4;
}

// 0x00735664
void nn::pia::transport::StationProtocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
