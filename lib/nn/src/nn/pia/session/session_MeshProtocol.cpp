#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/common/common_Watermark.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "nn/pia/session/session_DestroyMeshJob.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_KickoutManageJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListener.h"
#include "nn/pia/session/session_ProcessDestroyMeshJob.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_ProcessJoinRequestJob.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/session/session_RelayRouteManageJob.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_ReceivedMessageAccessor.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the flags of the traces (what the bits stand for is not known)
const u64 TRACE_FLAG_BIT30 = 0x40000000ULL;
const u64 TRACE_FLAG_BIT31 = 0x80000000ULL;
const u64 TRACE_FLAG_BIT32 = 0x100000000ULL;
const u64 TRACE_FLAG_BIT33 = 0x200000000ULL;
const u64 TRACE_FLAG_BIT34 = 0x400000000ULL;
// the port of the reliable messages
const u16 RELIABLE_PORT = 1;
// the header of the station data list (and of the first part of a join response)
const u32 STATION_DATA_LIST_HEADER_SIZE = 12;
// the header of a connection report, then one byte per station
const u32 CONNECTION_REPORT_HEADER_SIZE = 16;
// byte 12 of a connection report
const u8 CONNECTION_REPORT_VERSION = 50;
// the stations a host kicks out at once (one less than the indices)
const int KICKOUT_STATION_NUM = 11;
// the kickout reasons of RelayRouteManageJob start here
const u8 KICKOUT_REASON_RELAY_BASE = 3;

// the size of the entry of a station in the station data list: its connection info and its index,
// aligned to 4 bytes
inline u32 GetStationEntrySize(u32 infoSize)
{
    return infoSize + (3 - (infoSize & 3)) + 1;
}
} // namespace

inline nn::pia::transport::ReliableSlidingWindow* nn::pia::session::MeshProtocol::GetReliableSlidingWindow(StationIndex stationIndex, StationIndex localStationIndex) const
{
    if (stationIndex >= localStationIndex) {
        stationIndex = static_cast<StationIndex>(stationIndex - 1);
    }
    return &m_pReliableSlidingWindows[stationIndex];
}

inline nn::Result nn::pia::session::MeshProtocol::PushData(transport::ReliableSlidingWindow* pWindow, const void* pData, u32 size)
{
    nn::Result result = pWindow->PushData(pData, size);
    if (result.IsSuccess() && common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
    }
    return result;
}

// 0x0042CB00 (name is ours)
nn::Result nn::pia::session::MeshProtocol::Initialize()
{
    m_ReliableSlidingWindowNum = transport::Transport::s_pInstance->m_StationNum - 1;
    m_pReliableSlidingWindows = common::NewArray<transport::ReliableSlidingWindow>(m_ReliableSlidingWindowNum);
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        nn::Result result = m_pReliableSlidingWindows[i].Initialize(4, 4);
        if (result.IsFailure()) {
            Finalize();
            return result;
        }
    }
    m_NextSendTimeNum = transport::Transport::s_pInstance->m_StationNum - 1;
    m_pNextSendTimes = common::NewArray<common::Time>(m_NextSendTimeNum);
    for (u32 i = 0; i < m_NextSendTimeNum; i++) {
        common::Time now;
        now.SetNow();
        m_pNextSendTimes[i] = now;
    }
    u32 infoSize;
    {
        transport::StationConnectionInfo info;
        infoSize = info.GetSerializedSize();
    }
    u32 size = transport::Transport::s_pInstance->m_StationNum * GetStationEntrySize(infoSize) + STATION_DATA_LIST_HEADER_SIZE;
    m_StationDataListSize = size;
    m_BufferSize = size;
    m_pBuffer = common::NewArray<u8>(size, 4);
    m_pStationDataList = common::NewArray<u8>(m_StationDataListSize, 4);
    return nn::Result();
}

// 0x0042CE28 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::ParseHelper(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    u32 size = accessor.m_Size;
    if (size == 0) {
        return;
    }
    const u8* pData = accessor.m_pData;
    transport::StationManager* pStationManager = transport::StationManager::s_pInstance;
    Mesh* pMesh = Mesh::s_pInstance;
    switch (pData[0]) {
    case MESSAGE_TYPE_JOIN_REQUEST:
        ParseJoinRequest(accessor);
        return;
    case MESSAGE_TYPE_JOIN_RESPONSE: {
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        u32 ackId = transport::ResendingMessageManager::s_pInstance->ExtractAckIdFromMessage(pData, size);
        JoinMeshJob* pJob = m_pJoinMeshJob;
        if (pJob == nullptr) {
            // the response came again: only the ack
            Mesh::s_pInstance->m_pStationProtocol->SendAck(ackId, accessor.m_SourceAddress);
            return;
        }
        if (!pJob->m_IsWaitingResponse) {
            return;
        }
        transport::Station* pHostStation = pStationManager->GetStation(Mesh::s_pInstance->m_HostStationIndex);
        if (pHostStation == nullptr || !(pHostStation->m_StationAddress == accessor.m_SourceAddress)) {
            return;
        }
        if (!pJob->ParseJoinResponse(pData, accessor.m_Size)) {
            return;
        }
        Mesh::s_pInstance->m_pStationProtocol->SendAck(ackId, pHostStation->m_StationAddress);
        return;
    }
    case MESSAGE_TYPE_LEAVE_REQUEST:
        ParseLeaveRequest(accessor);
        return;
    case MESSAGE_TYPE_LEAVE_RESPONSE: {
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        LeaveMeshJob* pJob = m_pLeaveMeshJob;
        if (pJob == nullptr || !pJob->m_IsWaitingResponse) {
            return;
        }
        common::StationAddress address;
        address.Deserialize(pData + 4);
        StationIndex hostStationIndex = static_cast<StationIndex>(pData[1]);
        transport::Station* pHostStation = pStationManager->GetStation(Mesh::s_pInstance->m_HostStationIndex);
        if (common::IsValidPointer(pHostStation) && pHostStation->m_StationIndex == hostStationIndex) {
            pJob->m_IsWaitingResponse = false;
        }
        return;
    }
    case MESSAGE_TYPE_DESTROY_MESH: {
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        common::StationAddress address;
        address.Deserialize(pData + 4);
        StationIndex hostStationIndex = static_cast<StationIndex>(pData[1]);
        transport::Station* pHostStation = pStationManager->GetStation(Mesh::s_pInstance->m_HostStationIndex);
        if (!common::IsValidPointer(pHostStation)) {
            // the host may be gone in the migration
            if (!Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
                return;
            }
        } else if (pHostStation->m_StationIndex != hostStationIndex) {
            return;
        }
        ProcessDestroyMeshJob* pJob = Mesh::s_pInstance->m_pProcessDestroyMeshJob;
        if (pJob->Startup()) {
            pJob->Ready(false);
        }
        return;
    }
    case MESSAGE_TYPE_DESTROY_RESPONSE:
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        if (accessor.m_SourceStationIndex != pData[1]) {
            return;
        }
        if (!pMesh->m_pDestroyMeshJob->m_IsWaitingResponse) {
            return;
        }
        pMesh->m_pDestroyMeshJob->ReceiveDestroyResponse(static_cast<StationIndex>(pData[1]));
        return;
    case MESSAGE_TYPE_STATION_DATA_LIST:
        ParseStationDataList(accessor);
        return;
    case MESSAGE_TYPE_KICKOUT_NOTICE: {
        // from the host to the others
        StationIndex localStationIndex = pMesh->m_LocalStationIndex;
        if (localStationIndex <= STATION_INDEX_MAX && localStationIndex == pMesh->m_HostStationIndex) {
            return;
        }
        StationIndex sourceStationIndex = accessor.m_SourceStationIndex;
        if (sourceStationIndex > STATION_INDEX_MAX || sourceStationIndex != pMesh->m_HostStationIndex) {
            return;
        }
        if (size != 2) {
            return;
        }
        KickoutManageJob* pJob = pMesh->m_pKickoutManageJob;
        if (pJob->ReceiveKickoutNotice(static_cast<KickoutManageJob::KickoutReason>(pData[1]))) {
            pJob->Ready(false);
        }
        return;
    }
    case MESSAGE_TYPE_CONNECTION_CHECK: {
        StationIndex sourceStationIndex = accessor.m_SourceStationIndex;
        if (sourceStationIndex > STATION_INDEX_MAX || size != 2) {
            return;
        }
        if (sourceStationIndex != pData[1]) {
            return;
        }
        StationIndex localStationIndex = pMesh->m_LocalStationIndex;
        if (localStationIndex > STATION_INDEX_MAX) {
            return;
        }
        transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, static_cast<StationIndex>(pData[1]), 2, false);
        if (!common::IsValidPointer(pWriter)) {
            return;
        }
        pWriter->GetPayload()[0] = MESSAGE_TYPE_CONNECTION_CHECK_RESPONSE;
        pWriter->GetPayload()[1] = localStationIndex;
        m_pPacketHandler->Commit();
        return;
    }
    case MESSAGE_TYPE_CONNECTION_CHECK_RESPONSE:
        return;
    case MESSAGE_TYPE_CONNECTION_FAILURE_NOTICE:
        ParseConnectionFailureNotice(accessor);
        return;
    case MESSAGE_TYPE_GREETING: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        StationIndex stationIndex = static_cast<StationIndex>(accessor.m_pData[1]);
        if (accessor.m_SourceStationIndex != stationIndex) {
            return;
        }
        Mesh* pCurrentMesh = Mesh::s_pInstance;
        ProcessHostMigrationJob* pJob = pCurrentMesh->m_pProcessHostMigrationJob;
        if (pJob->m_IsWaitingGreeting) {
            pJob->ReceiveGreeting(stationIndex);
            return;
        }
        if (pJob->m_IsRunning) {
            return;
        }
        if (pCurrentMesh->m_pEventListener == nullptr) {
            return;
        }
        Mesh::Event event;
        event.m_Type = Mesh::EVENT_TYPE_GREETING;
        event.m_StationIndex = stationIndex;
        event.m_Unknown0x4 = 0;
        pCurrentMesh->m_pEventListener->OnEvent(event);
        return;
    }
    case MESSAGE_TYPE_MIGRATION_FINISH: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        bool value = accessor.m_pData[1] != 0;
        Mesh* pCurrentMesh = Mesh::s_pInstance;
        if (pCurrentMesh->m_HostStationIndex != accessor.m_SourceStationIndex) {
            return;
        }
        ProcessHostMigrationJob* pJob = pCurrentMesh->m_pProcessHostMigrationJob;
        if (!pJob->m_IsWaitingMigrationFinish) {
            return;
        }
        pJob->ReceiveMigrationFinish(value);
        return;
    }
    case MESSAGE_TYPE_GREETING_RESPONSE: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        StationIndex stationIndex = static_cast<StationIndex>(accessor.m_pData[1]);
        if (accessor.m_SourceStationIndex != stationIndex) {
            return;
        }
        ProcessHostMigrationJob* pJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
        if (!pJob->m_IsWaitingGreetingResponse) {
            return;
        }
        pJob->ReceiveGreetingResponse(stationIndex);
        return;
    }
    case MESSAGE_TYPE_MIGRATION_REQUEST: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        if (Mesh::s_pInstance->m_pLeaveMeshJob->IsRunning()) {
            return;
        }
        Mesh* pCurrentMesh = Mesh::s_pInstance;
        StationIndex hostStationIndex = pCurrentMesh->m_HostStationIndex;
        if (accessor.m_SourceStationIndex != hostStationIndex) {
            return;
        }
        StationIndex newHostStationIndex = static_cast<StationIndex>(accessor.m_pData[1]);
        if (newHostStationIndex > STATION_INDEX_MAX || hostStationIndex == newHostStationIndex) {
            return;
        }
        ProcessHostMigrationJob* pJob = pCurrentMesh->m_pProcessHostMigrationJob;
        if (!pJob->Startup(true, newHostStationIndex)) {
            return;
        }
        pJob->Ready(false);
        return;
    }
    case MESSAGE_TYPE_MIGRATION_RESPONSE: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        StationIndex stationIndex = static_cast<StationIndex>(accessor.m_pData[1]);
        if (accessor.m_SourceStationIndex != stationIndex) {
            return;
        }
        LeaveWithHostMigrationJob* pJob = Mesh::s_pInstance->m_pLeaveWithHostMigrationJob;
        if (!pJob->m_IsWaitingResponse) {
            return;
        }
        pJob->ReceiveMigrationResponse(stationIndex);
        return;
    }
    case MESSAGE_TYPE_MULTI_MIGRATION_RANKING: {
        if (!pMesh->m_IsHostMigrationEnabled) {
            return;
        }
        if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
            return;
        }
        Mesh* pCurrentMesh = Mesh::s_pInstance;
        StationIndex localStationIndex = pCurrentMesh->m_LocalStationIndex;
        if (localStationIndex <= STATION_INDEX_MAX && localStationIndex == pCurrentMesh->m_HostStationIndex) {
            return;
        }
        if (pCurrentMesh->m_pLeaveMeshJob->IsRunning()) {
            return;
        }
        StationIndex sourceStationIndex = accessor.m_SourceStationIndex;
        if (sourceStationIndex != Mesh::s_pInstance->m_HostStationIndex) {
            return;
        }
        const u8* pRanking = accessor.m_pData;
        if (accessor.m_Size != Mesh::s_pInstance->m_StationNumMax + STATION_DATA_LIST_HEADER_SIZE || pRanking[1] != sourceStationIndex) {
            return;
        }
        u32 value1 = common::deserializeU32(pRanking + 4);
        u32 value2 = common::deserializeU32(pRanking + 8);
        ProcessHostMigrationJob* pJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
        if (!pJob->StartupMulti(true, pRanking + STATION_DATA_LIST_HEADER_SIZE, value1, value2)) {
            return;
        }
        pJob->Ready(false);
        return;
    }
    case MESSAGE_TYPE_MULTI_MIGRATION_RANK_DECISION:
        ParseMultiMigrationRankDecision(accessor);
        return;
    case MESSAGE_TYPE_CONNECTION_REPORT: {
        // to the host
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        StationIndex localStationIndex = pMesh->m_LocalStationIndex;
        if (localStationIndex > STATION_INDEX_MAX || localStationIndex != pMesh->m_HostStationIndex) {
            return;
        }
        RelayRouteManageJob* pJob = pMesh->m_pRelayRouteManageJob;
        if (pJob == nullptr) {
            return;
        }
        if (pMesh->m_HostStationIndex != pData[1]) {
            return;
        }
        StationIndex stationIndex = static_cast<StationIndex>(pData[2]);
        if (accessor.m_SourceStationIndex != stationIndex || pData[13] != 0) {
            return;
        }
        if (pData[14] != 0 || pData[15] != 0) {
            return;
        }
        u32 value1 = common::deserializeU32(pData + 4);
        u32 value2 = common::deserializeU32(pData + 8);
        if (!pJob->IsRunning()) {
            if (!pJob->Startup(stationIndex, value1, value2)) {
                return;
            }
            pJob->Ready(false);
        }
        pJob->UpdateConnectionReport(stationIndex, pData, size);
        return;
    }
    case MESSAGE_TYPE_RELAY_ROUTE_DIRECTIONS: {
        // from the host to the others
        if (!common::IsValidPointer(pStationManager->m_pLocalStation)) {
            return;
        }
        StationIndex localStationIndex = pMesh->m_LocalStationIndex;
        if (localStationIndex <= STATION_INDEX_MAX && localStationIndex == pMesh->m_HostStationIndex) {
            return;
        }
        transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
        if (!common::IsValidPointer(pRelayRouteManager)) {
            return;
        }
        if (pMesh->m_HostStationIndex != pData[1]) {
            return;
        }
        if (pData[2] > pMesh->m_StationNumMax || pData[3] != 0) {
            return;
        }
        // only a newer version (high word first)
        u32 versionHigh = common::deserializeU32(pData + 4);
        u32 versionLow = common::deserializeU32(pData + 8);
        if (versionHigh < pRelayRouteManager->m_DirectionsVersionHigh ||
            (versionHigh == pRelayRouteManager->m_DirectionsVersionHigh && versionLow <= pRelayRouteManager->m_DirectionsVersionLow)) {
            return;
        }
        // (a trace call with 0x10000000 here was removed by the linker: the function is empty)
        pRelayRouteManager->SetRelayRouteDirections(pData + 12, size - 12);
        pRelayRouteManager->m_DirectionsVersionLow = versionLow;
        pRelayRouteManager->m_DirectionsVersionHigh = versionHigh;
        return;
    }
    default:
        accessor.m_SourceAddress.Trace(TRACE_FLAG_BIT30);
        return;
    }
}

// 0x0042D7C4 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendGreeting(nn::pia::StationIndex stationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    if (transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
        return false;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, 2, false);
    if (!common::IsValidPointer(pWriter)) {
        return false;
    }
    pWriter->GetPayload()[0] = MESSAGE_TYPE_GREETING;
    pWriter->GetPayload()[1] = localStationIndex;
    return m_pPacketHandler->Commit().IsSuccess();
}

// 0x0042D898 (name is ours)
void nn::pia::session::MeshProtocol::SendStationDataList(bool isNew)
{
    transport::StationConnectionInfo info;
    u32 infoSize = info.GetSerializedSize();
    u32 entrySize = GetStationEntrySize(infoSize);
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    Mesh* pMesh = Mesh::s_pInstance;
    StationIndex hostStationIndex = pMesh->m_HostStationIndex;
    u32 listSize = pMesh->m_StationNumMax * entrySize + STATION_DATA_LIST_HEADER_SIZE;
    if (isNew) {
        // a new version of the list
        pMesh->m_Unknown0x70++;
        memset(m_pStationDataList, 0, m_StationDataListSize);
        u8 hostEntry = 0xFF;
        transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
        u8* pList = m_pStationDataList;
        u8 stationNum = 0;
        u32 offset = STATION_DATA_LIST_HEADER_SIZE;
        for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
             it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
            StationIndex stationIndex = (*it)->m_StationIndex;
            if (stationIndex > STATION_INDEX_MAX || !Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
                (*it)->Trace(TRACE_FLAG_BIT33);
                continue;
            }
            u32 size = 0;
            if (stationIndex == hostStationIndex) {
                hostEntry = stationNum;
            }
            pTable->GetStationConnectionInfo(*it, &info);
            info.Serialize(pList + offset, &size, infoSize);
            offset += size;
            pList[offset] = stationIndex;
            offset += 2;
            stationNum++;
        }
        if (Mesh::s_pInstance->m_StationNumMax < stationNum) {
            transport::StationManager::s_pInstance->Trace(TRACE_FLAG_BIT30);
            pTable->Trace(TRACE_FLAG_BIT30);
        }
        pList[0] = MESSAGE_TYPE_STATION_DATA_LIST;
        pList[1] = stationNum;
        pList[2] = hostEntry;
        common::serializeU32(pList + 4, Mesh::s_pInstance->m_Unknown0x70);
        // one part
        pList[8] = 1;
        pList[9] = 0;
        pList[10] = stationNum;
        pList[11] = 0;
        m_IsStationDataListValid = true;
    } else if (!m_IsStationDataListValid) {
        return;
    }
    // to every station, the next resends spread over the interval
    u32 count = 0;
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        transport::ReliableSlidingWindow* pWindow = &m_pReliableSlidingWindows[i];
        if (!pWindow->IsInCommunication()) {
            continue;
        }
        if (!Mesh::s_pInstance->CheckStationIndexIsValid(pWindow->m_PeerStationIndex)) {
            continue;
        }
        if (isNew) {
            PushData(&m_pReliableSlidingWindows[i], m_pStationDataList, listSize);
            transport::Transport* pTransport = transport::Transport::s_pInstance;
            s32 delayMSec = m_KeepAliveTimeoutMSec / pTransport->m_StationNum * count + m_KeepAliveTimeoutMSec;
            m_pNextSendTimes[i] = pTransport->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * delayMSec);
        } else {
            transport::Transport* pTransport = transport::Transport::s_pInstance;
            s32 delayMSec = m_KeepAliveTimeoutMSec / pTransport->m_StationNum * count;
            m_pNextSendTimes[i] = pTransport->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * delayMSec);
        }
        count++;
    }
}

// 0x0042DC40 (name is ours)
void nn::pia::session::MeshProtocol::ParseStationDataList(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    if (Mesh::s_pInstance->m_pLeaveMeshJob->IsRunning()) {
        return;
    }
    if (Mesh::s_pInstance->m_pProcessDestroyMeshJob->m_IsRunning) {
        return;
    }
    const u8* pData = accessor.m_pData;
    u32 size = accessor.m_Size;
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
    if (pHostStation == nullptr || !(pHostStation->m_StationAddress == accessor.m_SourceAddress)) {
        // not from the host
        if (Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
            Mesh::s_pInstance->Disconnect(Mesh::DISCONNECT_REASON_9);
        } else {
            Mesh::s_pInstance->Disconnect(Mesh::DISCONNECT_REASON_6);
        }
        return;
    }
    u8 stationNum = pData[1];
    bool isSameVersion = false;
    u32 version = common::deserializeU32(pData + 4);
    if (Mesh::s_pInstance->m_Unknown0x70 > version) {
        return;
    }
    if (Mesh::s_pInstance->m_Unknown0x70 == version) {
        if (transport::StationManager::s_pInstance->m_ActiveStations.GetNum() == stationNum) {
            return;
        }
        isSameVersion = true;
    }
    ProcessUpdateMeshJob* pJob = Mesh::s_pInstance->m_pProcessUpdateMeshJob;
    if (pJob->m_IsProcessing) {
        if (version <= pJob->m_Version) {
            return;
        }
        if (pJob->UpdateStationDataList(pData, size) != ProcessUpdateMeshJob::UPDATE_STATE_ACCEPTED) {
            return;
        }
    } else {
        u32 state = pJob->Startup(pData, size, isSameVersion);
        if (state == ProcessUpdateMeshJob::UPDATE_STATE_PART_PENDING || state == ProcessUpdateMeshJob::UPDATE_STATE_OLD) {
            return;
        }
        if (state != ProcessUpdateMeshJob::UPDATE_STATE_ACCEPTED) {
            return;
        }
        pJob->Ready(false);
    }
    Mesh::s_pInstance->m_Unknown0x70 = version;
}

// 0x0042DDB8 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendDestroyMesh(nn::pia::StationIndex stationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    if (transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
        return false;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_DESTROY_MESH;
    m_pBuffer[1] = localStationIndex;
    m_pBuffer[3] = 0;
    m_pBuffer[2] = 0;
    u32 size = 0;
    pLocalStation->m_StationAddress.Serialize(m_pBuffer + 4, &size, m_BufferSize - 4);
    return PushData(pWindow, m_pBuffer, size + 4).IsSuccess();
}

// 0x0042DEFC | fefates:bytes [tier B]
void nn::pia::session::MeshProtocol::ParseJoinRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    const u8* pData = accessor.m_pData;
    u32 ackId = transport::ResendingMessageManager::s_pInstance->ExtractAckIdFromMessage(pData, accessor.m_Size);
    ProcessJoinRequestJob* pJob = m_pProcessJoinRequestJob;
    if (!common::IsValidPointer(pJob)) {
        return;
    }
    if (!pJob->m_IsProcessing && pJob->GetState() == common::Job::EXECUTE_STATE_SUSPENDED) {
        // a new request: the job goes on with it
        common::StationAddress address(accessor.m_SourceAddress);
        StationIndex sourceStationIndex = accessor.m_SourceStationIndex;
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(address);
        if (pStation != nullptr && pStation->m_StationIndex == STATION_INDEX_UNIDENTIFIED && pData[1] == STATION_INDEX_UNIDENTIFIED) {
            pJob->SetJoiningStationData(sourceStationIndex, address);
            pJob->m_AckId = ackId;
            Mesh::s_pInstance->m_pStationProtocol->SendAck(ackId, address);
            pJob->Resume(false);
        }
    } else if (pJob->m_IsProcessing && pJob->m_AckId == ackId) {
        // the request came again
        Mesh::s_pInstance->m_pStationProtocol->SendAck(ackId, accessor.m_SourceAddress);
    }
}

// 0x0042E050 (name is ours)
bool nn::pia::session::MeshProtocol::SendConnectionCheck(nn::pia::StationIndex stationIndex)
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, 2, false);
    if (!common::IsValidPointer(pWriter)) {
        return false;
    }
    pWriter->GetPayload()[0] = MESSAGE_TYPE_CONNECTION_CHECK;
    pWriter->GetPayload()[1] = localStationIndex;
    return m_pPacketHandler->Commit().IsSuccess();
}

// 0x0042E0E8
void nn::pia::session::MeshProtocol::SendLeaveRequest()
{
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
    if (!common::IsValidPointer(pHostStation)) {
        return;
    }
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return;
    }
    StationIndex hostStationIndex = pHostStation->m_StationIndex;
    StationIndex localStationIndex = pLocalStation->m_StationIndex;
    if (hostStationIndex > STATION_INDEX_MAX || localStationIndex > STATION_INDEX_MAX || hostStationIndex == localStationIndex) {
        return;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(hostStationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return;
    }
    m_pBuffer[0] = MESSAGE_TYPE_LEAVE_REQUEST;
    m_pBuffer[1] = localStationIndex;
    m_pBuffer[3] = 0;
    m_pBuffer[2] = 0;
    u32 size = 0;
    pLocalStation->m_StationAddress.Serialize(m_pBuffer + 4, &size, m_BufferSize - 4);
    PushData(pWindow, m_pBuffer, size + 4);
}

// 0x0042E214 (name is ours)
void nn::pia::session::MeshProtocol::ParseLeaveRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    const u8* pData = accessor.m_pData;
    common::StationAddress address;
    address.Deserialize(pData + 4);
    StationIndex stationIndex = static_cast<StationIndex>(pData[1]);
    if (accessor.m_SourceStationIndex != stationIndex) {
        return;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation == nullptr) {
        return;
    }
    SendLeaveResponse(pStation);
    if (Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
        Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
        if (Mesh::s_pInstance->m_StationNum != 0) {
            Mesh::s_pInstance->m_StationNum--;
        }
    }
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager != nullptr && stationIndex <= STATION_INDEX_MAX) {
        pRelayRouteManager->SearchRefugeRelayRoute(stationIndex);
    }
    pStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
    pStation->m_StationIndex = STATION_INDEX_UNIDENTIFIED;
    SendStationDataList(true);
}

// 0x0042E330
bool nn::pia::session::MeshProtocol::SendKickoutNotice(nn::pia::StationIndex stationIndex, unsigned char reason)
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_KICKOUT_NOTICE;
    m_pBuffer[1] = reason;
    return PushData(pWindow, m_pBuffer, 2).IsSuccess();
}

// 0x0042E3EC | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendLeaveResponse(nn::pia::transport::Station* pStation)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return;
    }
    u32 messageSize = pLocalStation->m_StationAddress.GetSerializedSize() + 4;
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, messageSize, false);
    if (!common::IsValidPointer(pWriter)) {
        pStation->Trace(TRACE_FLAG_BIT32);
        return;
    }
    m_pBuffer[0] = MESSAGE_TYPE_LEAVE_RESPONSE;
    m_pBuffer[1] = pLocalStation->m_StationIndex;
    m_pBuffer[3] = 0;
    m_pBuffer[2] = 0;
    u32 size;
    pLocalStation->m_StationAddress.Serialize(m_pBuffer + 4, &size, m_BufferSize - 4);
    pWriter->SetPayload(m_pBuffer);
    if (m_pPacketHandler->Commit().IsFailure()) {
        return;
    }
    // once more in a packet of its own
    pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, messageSize, true);
    if (!common::IsValidPointer(pWriter)) {
        return;
    }
    pWriter->SetPayload(m_pBuffer);
    m_pPacketHandler->Commit();
}

// 0x0042E544 (name is ours)
bool nn::pia::session::MeshProtocol::SendMigrationRequest(nn::pia::StationIndex stationIndex, nn::pia::StationIndex newHostStationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    if (transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
        return false;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_MIGRATION_REQUEST;
    m_pBuffer[1] = newHostStationIndex;
    return PushData(pWindow, m_pBuffer, 2).IsSuccess();
}

// 0x0042E648 | fefates:bytes [tier B]
void nn::pia::session::MeshProtocol::MakeJoinRequestData(unsigned char* pData)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    pData[0] = MESSAGE_TYPE_JOIN_REQUEST;
    pData[1] = pLocalStation->m_StationIndex;
    pData[3] = 0;
    pData[2] = 0;
    u32 size = 0;
    pLocalStation->m_StationAddress.Serialize(pData + 4, &size, GetJoinRequestDataSize() - 4);
}

// 0x0042E6AC (name is ours)
bool nn::pia::session::MeshProtocol::SendDestroyResponse(nn::pia::StationIndex stationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation == nullptr) {
        return false;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, 2, false);
    if (!common::IsValidPointer(pWriter)) {
        pStation->Trace(TRACE_FLAG_BIT31);
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_DESTROY_RESPONSE;
    m_pBuffer[1] = localStationIndex;
    pWriter->SetPayload(m_pBuffer);
    if (m_pPacketHandler->Commit().IsSuccess()) {
        // once more in a packet of its own
        pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, 2, true);
        if (common::IsValidPointer(pWriter)) {
            pWriter->SetPayload(m_pBuffer);
            m_pPacketHandler->Commit();
        }
    }
    return true;
}

// 0x0042E7FC | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendMigrationFinish(bool value)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_MIGRATION_FINISH;
    m_pBuffer[1] = value;
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        if (!m_pReliableSlidingWindows[i].IsInCommunication()) {
            continue;
        }
        PushData(&m_pReliableSlidingWindows[i], m_pBuffer, 2);
    }
    return true;
}

// 0x0042E8F8 slot 0x1C | fefates:callseq
nn::Result nn::pia::session::MeshProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_STATE;
    }
    StationIndex stationIndex = event.m_StationIndex;
    if (stationIndex >= static_cast<u8>(m_ReliableSlidingWindowNum + 1) || stationIndex == localStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    switch (event.m_Type) {
    case transport::ProtocolEvent::TYPE_JOIN:
        return pWindow->Startup(m_pPacketHandler, transport::ProtocolId(GetProtocolType(), RELIABLE_PORT).m_Id, localStationIndex, event.m_StationIndex);
    case transport::ProtocolEvent::TYPE_LEAVE:
        pWindow->Cleanup();
        return nn::Result();
    default:
        return common::RESULT_INVALID_ARGUMENT;
    }
}

// 0x0042E9C8 | fefates:callgraph
u32 nn::pia::session::MeshProtocol::MakeJoinResponseData(nn::pia::StationIndex stationIndex, unsigned char** ppData, unsigned int* pSizes)
{
    transport::StationConnectionInfo info;
    u32 infoSize = info.GetSerializedSize();
    // a station: its connection info (rounded up to words) and its index
    u32 entrySize = infoSize + (3 - (infoSize & 3)) + 1;
    StationIndex hostStationIndex = Mesh::s_pInstance->m_HostStationIndex;
    memset(m_pBuffer, 0, m_BufferSize);
    u32 hostPosition = 0xFF;
    u32 targetPosition = 0xFF;
    u8* pBuffer = m_pBuffer;
    u8 stationNum = 0;
    // bytes 8 to 10 of the header: the entry limits of the station id table and the station limit
    Session* pSession = Session::s_pInstance;
    if (pSession != nullptr && pSession->IsUsingStationIdTable()) {
        Session* pSession2 = Session::s_pInstance;
        if (pSession2->m_pMatchmakeSessions[pSession2->m_CurrentIndex] != nullptr) {
            pBuffer[8] = pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex];
        }
        if (pSession2->m_pMatchmakeSessions[pSession2->m_CurrentIndex == 0 ? 1 : 0] != nullptr) {
            pBuffer[9] = pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex == 0 ? 1 : 0];
        }
    }
    pBuffer[10] = static_cast<u8>(Mesh::s_pInstance->m_StationNumMax);
    u32 offset = STATION_DATA_LIST_HEADER_SIZE;
    transport::StationConnectionInfoTable* pInfoTable = transport::StationConnectionInfoTable::s_pInstance;
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        StationIndex index = (*it)->m_StationIndex;
        if (index > STATION_INDEX_MAX) {
            (*it)->Trace(TRACE_FLAG_BIT31);
            continue;
        }
        if (index == hostStationIndex) {
            hostPosition = stationNum;
        }
        u32 size = 0;
        if (index == stationIndex) {
            targetPosition = stationNum;
        }
        pInfoTable->GetStationConnectionInfo(*it, &info);
        info.Serialize(pBuffer + offset, &size, infoSize);
        stationNum++;
        pBuffer[offset + size] = index;
        offset += size + 2;
    }
    // the most stations a part can take
    u32 partEntrySize;
    {
        transport::StationConnectionInfo emptyInfo;
        u32 size = emptyInfo.GetSerializedSize();
        partEntrySize = size + (3 - (size & 3)) + 1;
    }
    u32 sizeLimit = m_pPacketHandler->GetPayloadSizeLimit();
    u32 partNum = 0;
    if (stationNum >= 1 && stationNum <= STATION_INDEX_MAX + 1) {
        for (u32 num = stationNum; num > 0; num--) {
            if (partEntrySize * num + STATION_DATA_LIST_HEADER_SIZE <= sizeLimit) {
                partNum = stationNum / num + (stationNum % num != 0 ? 1 : 0);
                break;
            }
        }
    }
    if (partNum == 1) {
        if (m_pPacketHandler->GetPayloadSizeLimit() < offset) {
            return 0;
        }
        u8* pData = ppData[0];
        pData[0] = MESSAGE_TYPE_JOIN_RESPONSE;
        pData[1] = stationNum;
        pData[2] = hostPosition;
        pData[3] = targetPosition;
        pData[4] = 1;
        pData[5] = 0;
        pData[6] = stationNum;
        pData[7] = 0;
        memcpy(pData + 8, pBuffer + 8, offset - 8);
        pSizes[0] = offset;
        return 1;
    }
    if (partNum != 2 && partNum != 3) {
        return 0;
    }
    // each part has an 8 byte header in front of its stations (the first one also bytes 8 to 11)
    u8 partStationNum = static_cast<u8>(stationNum / partNum + (stationNum % partNum != 0 ? 1 : 0));
    u32 position = 0;
    u8 restNum = stationNum;
    u8* pPart = pBuffer;
    for (u32 i = 0; i < partNum; i++) {
        pPart[0] = MESSAGE_TYPE_JOIN_RESPONSE;
        pPart[7] = stationNum - restNum;
        pPart[5] = i;
        pPart[1] = stationNum;
        pPart[2] = hostPosition;
        pPart[3] = targetPosition;
        pPart[4] = static_cast<u8>(partNum);
        pPart[6] = partStationNum;
        u32 size = partStationNum * entrySize + (i == 0 ? 12 : 8);
        memcpy(ppData[i], pPart, size);
        pSizes[i] = size;
        restNum = static_cast<u8>(restNum - partStationNum);
        position = position + size - 8;
        if (restNum < partStationNum) {
            partStationNum = restNum;
        }
        pPart = pBuffer + position;
    }
    return partNum;
}

// 0x0042ED90 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendConnectionReport(nn::pia::StationIndex stationIndex, unsigned int value1, unsigned int value2, unsigned int* pNoRouteBitmap)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    if (transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
        return false;
    }
    StationIndex hostStationIndex = Mesh::s_pInstance->m_HostStationIndex;
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return false;
    }
    u8* pReport = m_pBuffer;
    pReport[0] = MESSAGE_TYPE_CONNECTION_REPORT;
    pReport[1] = hostStationIndex;
    pReport[2] = localStationIndex;
    common::serializeU32(pReport + 4, value1);
    common::serializeU32(pReport + 8, value2);
    pReport[12] = CONNECTION_REPORT_VERSION;
    pReport[15] = 0;
    pReport[14] = 0;
    pReport[13] = 0;
    // the round trip time to every station in units of 4 ms (0: no direct connection)
    bool hasNoRoute = false;
    u32 noRouteBitmap = 0;
    u8 directNum = 0;
    u32 offset = CONNECTION_REPORT_HEADER_SIZE;
    for (int i = 0; i < Mesh::s_pInstance->m_StationNumMax; i++) {
        u16 rtt = 0;
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(static_cast<StationIndex>(i));
        if (pStation != nullptr && pStation != pLocalStation && pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
            rtt = transport::NetworkRttManager::s_pInstance->GetAverage(pStation->m_StationAddress);
            if (rtt != 0) {
                if (!pStation->IsConnectionRouteDirect()) {
                    rtt = 0;
                } else {
                    directNum++;
                    if ((rtt >> 2) == 0) {
                        rtt = 4;
                    }
                }
            } else if (!pStation->IsConnectionRouteRelay()) {
                hasNoRoute = true;
                noRouteBitmap |= 1 << i;
            }
        }
        u32 value = rtt >> 2;
        pReport[offset] = value >= 0xFF ? 0xFF : value;
        offset++;
    }
    pReport[3] = directNum;
    if (hasNoRoute) {
        *pNoRouteBitmap = noRouteBitmap;
        return false;
    }
    return PushData(pWindow, m_pBuffer, offset).IsSuccess();
}

// 0x0042F004 (name is ours)
bool nn::pia::session::MeshProtocol::SendGreetingResponse(nn::pia::StationIndex stationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    if (transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
        return false;
    }
    transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
    if (!pWindow->IsInCommunication()) {
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_GREETING_RESPONSE;
    m_pBuffer[1] = localStationIndex;
    return PushData(pWindow, m_pBuffer, 2).IsSuccess();
}

// 0x0042F104 (name is ours)
bool nn::pia::session::MeshProtocol::SendMigrationResponse(nn::pia::StationIndex stationIndex)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation == nullptr) {
        return false;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, 2, false);
    if (!common::IsValidPointer(pWriter)) {
        pStation->Trace(TRACE_FLAG_BIT34);
        return false;
    }
    m_pBuffer[0] = MESSAGE_TYPE_MIGRATION_RESPONSE;
    m_pBuffer[1] = localStationIndex;
    pWriter->SetPayload(m_pBuffer);
    if (m_pPacketHandler->Commit().IsSuccess()) {
        // once more in a packet of its own
        pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, pStation->m_StationAddress, 2, true);
        if (common::IsValidPointer(pWriter)) {
            pWriter->SetPayload(m_pBuffer);
            m_pPacketHandler->Commit();
        }
    }
    return true;
}

// 0x0042F254 (name is ours)
void nn::pia::session::MeshProtocol::SendJoinRejection(const nn::pia::common::StationAddress& address, unsigned char reason)
{
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, address, 5, false);
    if (!common::IsValidPointer(pWriter)) {
        address.Trace(TRACE_FLAG_BIT31);
        return;
    }
    // a join response without stations
    m_pBuffer[0] = MESSAGE_TYPE_JOIN_RESPONSE;
    m_pBuffer[1] = 0;
    m_pBuffer[2] = 0xFF;
    m_pBuffer[3] = 0xFF;
    m_pBuffer[4] = reason;
    pWriter->SetPayload(m_pBuffer);
    if (m_pPacketHandler->Commit().IsFailure()) {
        return;
    }
    // once more in a packet of its own
    pWriter = m_pPacketHandler->AssignByStationAddress(m_ProtocolId, address, 5, true);
    if (!common::IsValidPointer(pWriter)) {
        return;
    }
    pWriter->SetPayload(m_pBuffer);
    m_pPacketHandler->Commit();
}

// 0x0042F384 (name is ours)
bool nn::pia::session::MeshProtocol::SendMultiMigrationRanking(bool* pIsSent)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    u8 message[STATION_DATA_LIST_HEADER_SIZE + transport::StationManager::STATION_NUM_MAX] = {};
    u32 size = Mesh::s_pInstance->m_StationNumMax + STATION_DATA_LIST_HEADER_SIZE;
    u32 value1;
    u32 value2;
    if (Mesh::s_pInstance->m_pProcessHostMigrationJob
            ->MakeHostCandidateRanking(localStationIndex, message + STATION_DATA_LIST_HEADER_SIZE, &value1, &value2, true)
            .IsFailure()) {
        return false;
    }
    message[0] = MESSAGE_TYPE_MULTI_MIGRATION_RANKING;
    message[1] = localStationIndex;
    message[2] = Mesh::s_pInstance->m_StationNum;
    common::serializeU32(message + 4, value1);
    common::serializeU32(message + 8, value2);
    for (int i = 0; i <= STATION_INDEX_MAX; i++) {
        pIsSent[i] = false;
        StationIndex stationIndex = static_cast<StationIndex>(i);
        if (stationIndex == localStationIndex || !Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
            continue;
        }
        transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(stationIndex, localStationIndex);
        if (!pWindow->IsInCommunication()) {
            continue;
        }
        if (PushData(pWindow, message, size).IsFailure()) {
            continue;
        }
        pIsSent[i] = true;
    }
    return true;
}

// 0x0042F518 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendRelayRouteDirections()
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (!common::IsValidPointer(pRelayRouteManager)) {
        return false;
    }
    u32 directionsSize = pRelayRouteManager->GetRelayRouteDirectionsSize();
    u8* pMessage = m_pBuffer;
    pMessage[0] = MESSAGE_TYPE_RELAY_ROUTE_DIRECTIONS;
    pMessage[1] = localStationIndex;
    pMessage[2] = Mesh::s_pInstance->m_StationNum;
    pMessage[3] = 0;
    common::serializeU32(pMessage + 4, pRelayRouteManager->m_DirectionsVersionHigh);
    common::serializeU32(pMessage + 8, pRelayRouteManager->m_DirectionsVersionLow);
    u32 size = 0;
    if (pRelayRouteManager->GetRelayRouteDirections(pMessage + 12, m_BufferSize, &size).IsFailure()) {
        return false;
    }
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        if (!m_pReliableSlidingWindows[i].IsInCommunication()) {
            continue;
        }
        PushData(&m_pReliableSlidingWindows[i], m_pBuffer, directionsSize + 12);
    }
    return true;
}

// 0x0042F68C | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendConnectionFailureNotice(nn::pia::StationIndex destination, nn::pia::StationIndex stationIndex1, nn::pia::StationIndex stationIndex2, unsigned char reason, unsigned int version)
{
    if (Mesh::s_pInstance->m_LocalStationIndex > STATION_INDEX_MAX) {
        return;
    }
    transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, destination, 8, false);
    if (!common::IsValidPointer(pWriter)) {
        return;
    }
    u8 message[8] = {};
    message[0] = MESSAGE_TYPE_CONNECTION_FAILURE_NOTICE;
    message[1] = stationIndex1;
    message[2] = stationIndex2;
    message[3] = reason;
    common::serializeU32(message + 4, version);
    pWriter->SetPayload(message, 0, sizeof(message));
    m_pPacketHandler->Commit();
}

// 0x0042F750 (name is ours)
void nn::pia::session::MeshProtocol::ParseConnectionFailureNotice(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    StationIndex sourceStationIndex = accessor.m_SourceStationIndex;
    if (sourceStationIndex > STATION_INDEX_MAX || accessor.m_Size != 8) {
        return;
    }
    const u8* pData = accessor.m_pData;
    StationIndex stationIndex1 = static_cast<StationIndex>(pData[1]);
    StationIndex stationIndex2 = static_cast<StationIndex>(pData[2]);
    u8 reason = pData[3];
    u32 version = common::deserializeU32(pData + 4);
    Mesh* pMesh = Mesh::s_pInstance;
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    if (localStationIndex <= STATION_INDEX_MAX && localStationIndex == pMesh->m_HostStationIndex) {
        // the host passes the notice of station 2 on to station 1
        if (accessor.m_SourceStationIndex != stationIndex2) {
            return;
        }
        if (pMesh->m_Unknown0x70 > version) {
            return;
        }
        if (localStationIndex == stationIndex1) {
            return;
        }
        transport::ProtocolMessageWriter* pWriter = m_pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex1, 8, false);
        if (!common::IsValidPointer(pWriter)) {
            return;
        }
        u8 message[8] = {};
        message[0] = MESSAGE_TYPE_CONNECTION_FAILURE_NOTICE;
        message[1] = stationIndex1;
        message[2] = stationIndex2;
        message[3] = reason;
        common::serializeU32(message + 4, version);
        pWriter->SetPayload(message, 0, sizeof(message));
        m_pPacketHandler->Commit();
        return;
    }
    // to us from the host
    if (accessor.m_SourceStationIndex != pMesh->m_HostStationIndex || localStationIndex != stationIndex1) {
        return;
    }
    ProcessUpdateMeshJob* pJob = pMesh->m_pProcessUpdateMeshJob;
    if (!pJob->m_IsProcessing || pJob->m_Version != version) {
        return;
    }
    pJob->SetConnectionFailureNotice(stationIndex2, reason);
}

// 0x0042F89C | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::MeshProtocol::SendMultiMigrationRankDecision(nn::pia::StationIndex stationIndex, long long timeout, unsigned int* pAckId)
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return false;
    }
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || pLocalStation->m_StationIndex != localStationIndex) {
        return false;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation == nullptr) {
        return false;
    }
    bool isHigher;
    u32 value1;
    u32 value2;
    if (Mesh::s_pInstance->m_pProcessHostMigrationJob->CompareRank(localStationIndex, stationIndex, &isHigher, &value1, &value2).IsFailure()) {
        return false;
    }
    u8 message[12] = {};
    message[0] = MESSAGE_TYPE_MULTI_MIGRATION_RANK_DECISION;
    message[1] = Mesh::s_pInstance->m_HostStationIndex;
    message[2] = Mesh::s_pInstance->m_StationNum;
    message[3] = !isHigher;
    common::serializeU32(message + 4, value1);
    common::serializeU32(message + 8, value2);
    return transport::ResendingMessageManager::s_pInstance
        ->SetSendMessage(pAckId, message, sizeof(message), stationIndex, pStation->m_StationAddress, m_ProtocolId, timeout)
        .IsSuccess();
}

// 0x0042F9D4 (name is ours)
void nn::pia::session::MeshProtocol::ParseMultiMigrationRankDecision(const nn::pia::transport::ReceivedMessageAccessor& accessor)
{
    if (!Mesh::s_pInstance->m_IsHostMigrationEnabled) {
        return;
    }
    if (!common::IsValidPointer(transport::StationManager::s_pInstance->m_pLocalStation)) {
        return;
    }
    if (accessor.m_SourceStationIndex > STATION_INDEX_MAX) {
        return;
    }
    // the message and its ack id
    if (accessor.m_Size != 16) {
        return;
    }
    const u8* pData = accessor.m_pData;
    u32 ackId = transport::ResendingMessageManager::s_pInstance->ExtractAckIdFromMessage(pData, 16);
    bool isHigher;
    if (pData[3] == 0) {
        isHigher = false;
    } else if (pData[3] == 1) {
        isHigher = true;
    } else {
        return;
    }
    u32 value1 = common::deserializeU32(pData + 4);
    u32 value2 = common::deserializeU32(pData + 8);
    ProcessHostMigrationJob* pJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
    bool isLocalHigher;
    u32 localValue1;
    u32 localValue2;
    if (pJob->CompareRank(Mesh::s_pInstance->m_LocalStationIndex, accessor.m_SourceStationIndex, &isLocalHigher, &localValue1, &localValue2).IsFailure()) {
        return;
    }
    // the two disagree: the one with the larger values decides
    if (isLocalHigher != isHigher && (localValue1 > value1 || (localValue1 == value1 && localValue2 > value2))) {
        isHigher = isLocalHigher;
    }
    Mesh::s_pInstance->m_pStationProtocol->SendAck(ackId, accessor.m_SourceStationIndex);
    pJob->ReceiveRankDecision(accessor.m_SourceStationIndex, isHigher);
}

// 0x0042FB10 | fefates:callgraph [tier C]
nn::Result nn::pia::session::MeshProtocol::Startup()
{
    m_pProcessJoinRequestJob = nullptr;
    m_pJoinMeshJob = nullptr;
    m_Unknown0x1C = 0;
    m_pLeaveMeshJob = nullptr;
    m_IsStationDataListPending = false;
    return nn::Result();
}

// 0x0042FB30 slot 0x18
nn::Result nn::pia::session::MeshProtocol::Dispatch()
{
    transport::ReceivedMessageAccessor accessor;
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;

    // the unreliable messages
    transport::ProtocolId protocolId = m_ProtocolId;
    transport::PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    pIterator->m_pPacketHandler->BeginIteration();
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const transport::ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        accessor.m_pData = pReader->GetPayload();
        accessor.m_Size = pReader->m_PayloadSize;
        accessor.m_SourceStationIndex = pReader->GetSourceStationIndex();
        accessor.m_SourceAddress = pReader->m_SourceAddress;
        accessor.m_SourceStationKey = pReader->GetSourceStationKey();
        accessor.m_ConnectionId = pReader->m_Unknown0x1C;
        ParseHelper(accessor);
        pIterator->m_pPacketHandler->NextIteration();
    }

    // the packets of the reliable windows
    transport::ProtocolId reliableId(GetProtocolType(), RELIABLE_PORT);
    pIterator = m_pPacketHandler->GetIterator(reliableId);
    pIterator->m_pPacketHandler->BeginIteration();
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const transport::ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        StationIndex sourceStationIndex = pReader->GetSourceStationIndex();
        if (localStationIndex <= STATION_INDEX_MAX && sourceStationIndex <= STATION_INDEX_MAX) {
            transport::ReliableSlidingWindow* pWindow = GetReliableSlidingWindow(sourceStationIndex, localStationIndex);
            if (pWindow->IsInCommunication() && pWindow->AnalyzeProtocolMessage(*pReader).IsSuccess() &&
                common::WatermarkManager::s_pInstance != nullptr) {
                common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
                if (common::WatermarkManager::s_pInstance != nullptr) {
                    common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(pWindow->m_ReceiveCount);
                }
            }
        }
        pIterator->m_pPacketHandler->NextIteration();
    }

    // the reliable messages
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        if (!m_pReliableSlidingWindows[i].IsInCommunication()) {
            continue;
        }
        u32 size;
        while (m_pReliableSlidingWindows[i].PopData(m_pBuffer, &size, m_BufferSize).IsSuccess()) {
            if (common::WatermarkManager::s_pInstance != nullptr) {
                common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(m_pReliableSlidingWindows[i].m_ReceiveCount);
            }
            StationIndex peerStationIndex = m_pReliableSlidingWindows[i].m_PeerStationIndex;
            common::StationAddress address;
            if (transport::StationManager::s_pInstance->GetStationAddress(&address, peerStationIndex).IsSuccess()) {
                accessor.m_pData = m_pBuffer;
                accessor.m_Size = size;
                accessor.m_SourceStationIndex = peerStationIndex;
                accessor.m_SourceAddress = address;
                accessor.m_SourceStationKey = 0;
                accessor.m_ConnectionId = 0;
                ParseHelper(accessor);
            }
        }
    }
    for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
        if (m_pReliableSlidingWindows[i].IsInCommunication()) {
            m_pReliableSlidingWindows[i].Dispatch(m_pPacketHandler);
        }
    }

    // the stations that left or did not answer
    bool isStationLost = false;
    bool isHostLost = false;
    common::Time now = transport::Transport::s_pInstance->m_DispatchTime;
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return nn::Result();
    }
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        // the host kicks out the stations RelayRouteManageJob found without a route
        RelayRouteManageJob* pRelayJob = pMesh->m_pRelayRouteManageJob;
        if (pRelayJob != nullptr && pRelayJob->m_KickoutStationBitmap != 0) {
            u32 bitmap = pRelayJob->m_KickoutStationBitmap;
            u32 failedBitmap = 0;
            u32 bit = 1;
            for (int i = 0; i < KICKOUT_STATION_NUM; i++, bit <<= 1) {
                if (bitmap & 1) {
                    nn::Result result = Mesh::s_pInstance->m_pKickoutManageJob->StartKickout(
                        static_cast<StationIndex>(i),
                        static_cast<KickoutManageJob::KickoutReason>(Mesh::s_pInstance->m_pRelayRouteManageJob->m_pKickoutReasons[i] + KICKOUT_REASON_RELAY_BASE));
                    if (result.IsFailure() && result != common::RESULT_ALREADY_EXISTS) {
                        failedBitmap |= bit;
                    }
                }
                bitmap >>= 1;
                if (bitmap == 0) {
                    break;
                }
            }
            Mesh::s_pInstance->m_pRelayRouteManageJob->m_KickoutStationBitmap = failedBitmap;
        }
    }
    s32 index = 0;
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End();) {
        transport::Station* pStation = *it++;
        index++;
        if (pStation == pLocalStation) {
            continue;
        }
        bool isTimeout = false;
        if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
            // a station behind a relay gets two keep alive intervals more
            s32 timeoutMSec = m_KeepAliveTimeoutMSec;
            transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
            if (pRelayRouteManager != nullptr && pStation->m_StationIndex <= STATION_INDEX_MAX && pLocalStation->m_StationIndex <= STATION_INDEX_MAX) {
                StationIndex relayStationIndex;
                if (pRelayRouteManager->GetRelayRoute(pLocalStation->m_StationIndex, pStation->m_StationIndex, &relayStationIndex).IsSuccess() &&
                    pStation->m_StationIndex != relayStationIndex) {
                    timeoutMSec += transport::Transport::s_pInstance->GetKeepAliveIntervalMSec() * 2;
                }
            }
            isTimeout = pStation->m_LastReceiveTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * timeoutMSec) < now;
        }
        if (pStation->m_Unknown0x68) {
            isTimeout = true;
        }
        if (!isTimeout && pStation->m_State != transport::Station::STATION_STATE_DISCONNECTED) {
            continue;
        }
        StationIndex stationIndex = pStation->m_StationIndex;
        if (Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
            if (Mesh::s_pInstance->m_HostStationIndex == stationIndex && isTimeout) {
                isHostLost = true;
            }
            Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
            ProcessJoinRequestJob* pJoinJob = Mesh::s_pInstance->m_pProcessJoinRequestJob;
            if (pJoinJob->m_JoiningStationIndex == stationIndex) {
                pJoinJob->CancellationNotice(stationIndex);
            } else {
                isStationLost = true;
                if (Mesh::s_pInstance->m_StationNum != 0) {
                    Mesh::s_pInstance->m_StationNum--;
                }
            }
        }
        transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
        if (pRelayRouteManager != nullptr && stationIndex <= STATION_INDEX_MAX && !isHostLost) {
            pRelayRouteManager->SearchRefugeRelayRoute(stationIndex);
        }
        pStation->m_pDisconnectStationJob->OnDisconnected(pStation);
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
        transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pStation);
        pStation->CleanupJobs();
        pStation->Cleanup();
        transport::StationManager::s_pInstance->DestroyStation(pStation);
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->ClearStationIndex(stationIndex);
        transport::Transport::s_pInstance->OutputStreamUpdateEvent();
        // the list changed: on after the stations looked at
        it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
        for (s32 i = 0; i < index; i++) {
            if (++it == transport::StationManager::s_pInstance->m_ActiveStations.End()) {
                break;
            }
        }
    }

    pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        // the host: the stations of the mesh that have no station any more
        u32 bitmap = pMesh->m_StationBitmap;
        for (u32 i = 0; i < KICKOUT_STATION_NUM; i++) {
            if ((bitmap & 1) && transport::StationManager::s_pInstance->GetStation(static_cast<StationIndex>(i)) == nullptr) {
                Mesh::s_pInstance->UnfixDisconnectedId(static_cast<StationIndex>(i));
                Mesh* pCurrentMesh = Mesh::s_pInstance;
                ProcessJoinRequestJob* pJoinJob = pCurrentMesh->m_pProcessJoinRequestJob;
                if (pJoinJob->m_JoiningStationIndex == static_cast<StationIndex>(i)) {
                    pJoinJob->CancellationNotice(static_cast<StationIndex>(i));
                } else {
                    isStationLost = true;
                    if (pCurrentMesh->m_StationNum != 0) {
                        pCurrentMesh->m_StationNum--;
                    }
                }
                if (transport::Transport::s_pInstance->m_pRelayRouteManager != nullptr) {
                    transport::Transport::s_pInstance->m_pRelayRouteManager->SearchRefugeRelayRoute(static_cast<StationIndex>(i));
                }
            }
            bitmap >>= 1;
            if (bitmap == 0) {
                break;
            }
        }
    }

    if (isStationLost) {
        pMesh = Mesh::s_pInstance;
        if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
            // the host tells the others (after the migration if one runs)
            if (pMesh->m_IsHostMigrationEnabled && Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
                m_IsStationDataListPending = true;
                return nn::Result();
            }
            SendStationDataList(true);
            return nn::Result();
        }
        if (!isHostLost) {
            return nn::Result();
        }
        // the host is gone
        u32 bitmap = pMesh->m_StationBitmap;
        for (u16 i = 0; i < KICKOUT_STATION_NUM; i++) {
            if ((bitmap & (1 << i)) && transport::StationManager::s_pInstance->GetStation(static_cast<StationIndex>(i)) == nullptr) {
                Mesh::s_pInstance->UnfixDisconnectedId(static_cast<StationIndex>(i));
                if (Mesh::s_pInstance->m_StationNum != 0) {
                    Mesh::s_pInstance->m_StationNum--;
                }
                transport::Transport::s_pInstance->OutputStreamUpdateEvent();
            }
        }
        if (Mesh::s_pInstance->m_IsHostMigrationEnabled) {
            ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
            bool isStarted;
            if (Mesh::s_pInstance->m_HostMigrationMode == 2) {
                isStarted = pMigrationJob->StartupMulti(false, nullptr, 0, 0);
            } else {
                isStarted = pMigrationJob->Startup(false, STATION_INDEX_UNIDENTIFIED);
            }
            if (isStarted) {
                Mesh::s_pInstance->m_pProcessHostMigrationJob->Ready(false);
            }
            return nn::Result();
        }
        Mesh::s_pInstance->CleanupStationsJobs();
        if (Mesh::s_pInstance->m_pJoinMeshJob->IsRunning()) {
            Mesh::s_pInstance->m_pJoinMeshJob->Cleanup(common::RESULT_JOIN_FAILED);
            Mesh::s_pInstance->m_pJoinMeshJob->Reset(true);
        } else {
            Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_8;
            Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_8);
        }
        Mesh::s_pInstance->CleanupStatus();
        return nn::Result();
    }

    pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        // the host resends the list
        if (pMesh->m_StationNum > 1 && !(pMesh->m_IsHostMigrationEnabled && Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning)) {
            if (m_IsStationDataListPending) {
                m_IsStationDataListPending = false;
                SendStationDataList(true);
            }
            for (u32 i = 0; i < m_ReliableSlidingWindowNum; i++) {
                if (!m_pReliableSlidingWindows[i].IsInCommunication()) {
                    continue;
                }
                if (now < m_pNextSendTimes[i]) {
                    continue;
                }
                transport::StationConnectionInfo info;
                u32 infoSize = info.GetSerializedSize();
                u32 size = Mesh::s_pInstance->m_StationNumMax * (infoSize + (3 - (info.GetSerializedSize() & 3)) + 1) + STATION_DATA_LIST_HEADER_SIZE;
                PushData(&m_pReliableSlidingWindows[i], m_pStationDataList, size);
                m_pNextSendTimes[i] = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_StationDataListIntervalMSec);
            }
        }
        // a fatal error in the migration destroys the mesh
        if (!Mesh::s_pInstance->m_IsHostMigrationEnabled) {
            return nn::Result();
        }
        ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
        if (pMigrationJob->m_IsRunning || !pMigrationJob->IsFatalErrorOccur()) {
            return nn::Result();
        }
        if (Mesh::s_pInstance->m_pDestroyMeshJob->Startup(nullptr, true)) {
            Mesh::s_pInstance->m_pDestroyMeshJob->Ready(false);
            Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_9;
        }
        return nn::Result();
    }

    // a client: the migration starts when the host is gone
    if (!pMesh->m_IsHostMigrationEnabled) {
        return nn::Result();
    }
    if (!Mesh::s_pInstance->m_HostMigrationStartFlag) {
        return nn::Result();
    }
    if (!Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
        if (pHostStation != nullptr) {
            if (Mesh::s_pInstance->CheckStationIndexIsValid(pHostStation->m_StationIndex)) {
                Mesh::s_pInstance->UnfixDisconnectedId(pHostStation->m_StationIndex);
                if (Mesh::s_pInstance->m_StationNum != 0) {
                    Mesh::s_pInstance->m_StationNum--;
                }
            }
            pHostStation->m_pDisconnectStationJob->OnDisconnected(pHostStation);
            transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pHostStation);
            transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pHostStation);
            pHostStation->CleanupJobs();
            pHostStation->Cleanup();
            transport::StationManager::s_pInstance->DestroyStation(pHostStation);
            transport::Transport::s_pInstance->OutputStreamUpdateEvent();
        }
        if (Mesh::s_pInstance->m_pProcessHostMigrationJob->Startup(false, STATION_INDEX_UNIDENTIFIED)) {
            Mesh::s_pInstance->m_pProcessHostMigrationJob->Ready(false);
        }
    }
    Mesh::s_pInstance->m_HostMigrationStartFlag = false;
    return nn::Result();
}

// 0x00430830 (name is ours)
void nn::pia::session::MeshProtocol::Finalize()
{
    if (m_pBuffer != nullptr) {
        common::DeleteArray(m_pBuffer);
        m_pBuffer = nullptr;
    }
    m_BufferSize = 0;
    if (m_pStationDataList != nullptr) {
        common::DeleteArray(m_pStationDataList);
        m_pStationDataList = nullptr;
    }
    m_StationDataListSize = 0;
    if (m_pReliableSlidingWindows != nullptr) {
        common::DeleteArray(m_pReliableSlidingWindows);
        m_pReliableSlidingWindows = nullptr;
    }
    m_ReliableSlidingWindowNum = 0;
    if (m_pNextSendTimes != nullptr) {
        common::DeleteArray(m_pNextSendTimes);
        m_pNextSendTimes = nullptr;
    }
    m_NextSendTimeNum = 0;
}

// 0x00430918
nn::pia::session::MeshProtocol::MeshProtocol()
    : m_pProcessJoinRequestJob(nullptr), m_pJoinMeshJob(nullptr), m_Unknown0x1C(0), m_pLeaveMeshJob(nullptr), m_KeepAliveTimeoutMSec(10000),
      m_StationDataListIntervalMSec(10000), m_Unknown0x30(), m_ReliableSlidingWindowNum(0), m_pReliableSlidingWindows(nullptr), m_Unknown0x44(0),
      m_BufferSize(0), m_pBuffer(nullptr), m_StationDataListSize(0), m_pStationDataList(nullptr), m_IsStationDataListValid(false), m_NextSendTimeNum(0),
      m_pNextSendTimes(nullptr)
{
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->SetName("MeshProtocolReliable send buffer num");
    }
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->SetName("MeshProtocolReliable receive buffer num");
    }
}

// 0x0045FA54
// 0x00430A24 (deleting dtor)
nn::pia::session::MeshProtocol::~MeshProtocol()
{
    // empty (in the original too)
}

// 0x0045FA14 slot 0x10
nn::Result nn::pia::session::MeshProtocol::Startup(nn::pia::StationIndex localStationIndex)
{
    return transport::Protocol::Startup(localStationIndex);
}

// 0x00733844 | fefates:bytes [tier B]
u32 nn::pia::session::MeshProtocol::GetJoinRequestDataSize() const
{
    return transport::StationManager::s_pInstance->m_pLocalStation->m_StationAddress.GetSerializedSize() + 4;
}

// 0x0073383C slot 0x0C
u16 nn::pia::session::MeshProtocol::GetProtocolType() const
{
    return transport::PROTOCOL_TYPE_MESH;
}

// 0x00733868 slot 0x08
void nn::pia::session::MeshProtocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
