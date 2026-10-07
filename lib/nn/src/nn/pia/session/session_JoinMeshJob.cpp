#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListener.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
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
// the join request is resent this long
const s32 JOIN_REQUEST_TIMEOUT_MSEC = 15000;
// the size of the buffer of the join request
const u32 JOIN_REQUEST_BUFFER_SIZE = 0x5AC;
// the join response: the header of the first part (and of a response in one part) and of the
// following parts, then per station its connection info and index, each aligned to 4 bytes
const u32 RESPONSE_HEADER_SIZE = 12;
const u32 RESPONSE_PART_HEADER_SIZE = 8;
// byte 4 of a response that is not rejected: one part, or split into 2 or 3 parts
const u8 RESPONSE_TYPE_SINGLE = 1;
// the entry of the host / receiver is not in the response
const u8 RESPONSE_ENTRY_NONE = 0xFF;

// the results of the connection to a station that tell the listener (name is ours)
inline bool IsStationConnectionFailure(nn::Result result)
{
    return result == common::RESULT_STATION_CONNECTION_FAILED_E7 || result == common::RESULT_STATION_CONNECTION_FAILED_E8 ||
           result == common::RESULT_STATION_CONNECTION_FAILED_E9 || result == common::RESULT_STATION_CONNECTION_FAILED_EB ||
           result == common::RESULT_STATION_CONNECTION_FAILED_EC || result == common::RESULT_STATION_CONNECTION_FAILED_ED ||
           result == common::RESULT_STATION_CONNECTION_FAILED_EA || result == common::RESULT_STATION_CONNECTION_FAILED_F2;
}

// an event without data to the listener of the mesh
inline void NotifyEvent(Mesh::EventType type)
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_pEventListener != nullptr) {
        Mesh::Event event;
        event.m_Type = type;
        event.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
        event.m_Unknown0x4 = 0;
        pMesh->m_pEventListener->OnEvent(event);
    }
}

// the join response arrived: bytes 8 to 10 of it to the listener
inline void NotifyJoinResponse(const u8* pData)
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_pEventListener != nullptr) {
        Mesh::Event event;
        event.m_Type = Mesh::EVENT_TYPE_JOIN_RESPONSE;
        event.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
        event.m_Unknown0x4 = __builtin_bswap16(*reinterpret_cast<const u16*>(pData + 8)) | (pData[10] << 16);
        pMesh->m_pEventListener->OnEvent(event);
    }
}
} // namespace

inline void nn::pia::session::JoinMeshJob::SignalFailureToCaller(nn::Result result)
{
    SetElapsedTimeToMonitoringData();
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalFailure(result);
        }
        common::g_SessionBeginMonitoringContent.m_JoinResult = result.GetPrintableBits();
        common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
        Mesh::s_pInstance->SetSessionBeginMonitoringData();
        m_pCallContext = nullptr;
    }
}

// 0x0042A198 slot 0x1C
void nn::pia::session::JoinMeshJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x0042A19C slot 0x18 | fefates:bytes-fuzzy
bool nn::pia::session::JoinMeshJob::StartupImpl()
{
    Reset(true);
    SetStep(&JoinMeshJob::StartConnectingToHost, "JoinMeshJob::StartConnectingToHost");
    return true;
}

// 0x0042A1FC
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::WaitLeaveMesh()
{
    if (!m_CallContext.IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&JoinMeshJob::CompleteCancelWithLeaveMesh, "JoinMeshJob::CompleteCancelWithLeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042A284
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::WaitRequestAck()
{
    if (!transport::ResendingMessageManager::s_pInstance->CheckNowResending(m_AckId)) {
        // acked
        m_AckId = 0;
        SetStep(&JoinMeshJob::WaitJoinResponse, "JoinMeshJob::WaitJoinResponse");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (CheckContextCallCanncelled()) {
        transport::ResendingMessageManager::s_pInstance->StopResending(m_AckId);
        m_AckId = 0;
        ClearWaitingFlags();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (CheckTransportConnectionStatus() || CheckConnectionStateWithHostStation()) {
        transport::ResendingMessageManager::s_pInstance->StopResending(m_AckId);
        m_AckId = 0;
        ClearWaitingFlags();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_IsWaitingResponse) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the response came before the ack
    transport::ResendingMessageManager::s_pInstance->StopResending(m_AckId);
    m_AckId = 0;
    SetStep(&JoinMeshJob::AnalyzeJoinResponse, "JoinMeshJob::AnalyzeJoinResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042A414
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::CompleteProcess()
{
    if (m_pCallContext != nullptr) {
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
    }
    m_Phase = PHASE_COMPLETED;
    Mesh* pMesh = Mesh::s_pInstance;
    pMesh->m_MonitoringStartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    pMesh->m_IsMonitoring = true;
    SetElapsedTimeToMonitoringData();
    CheckRelayConnectionForMonitoring();
    common::g_SessionBeginMonitoringContent.m_JoinResult = 0;
    common::g_SessionBeginMonitoringContent.m_HostPrincipalId = 0xFFFFFFFF;
    common::g_SessionBeginMonitoringContent.m_JoinPhase = 0xFF;
    transport::Transport::s_pInstance->SetMonitoringNetworkRtt(true);
    Mesh::s_pInstance->SetSessionBeginMonitoringData();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042A4D4
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::SendJoinRequest()
{
    if (CheckContextCallCanncelled()) {
        CancelConnectingToHost();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (transport::StationManager::s_pInstance->m_pLocalStation == nullptr) {
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    u8 buffer[JOIN_REQUEST_BUFFER_SIZE] = {};
    Mesh::s_pInstance->m_pMeshProtocol->MakeJoinRequestData(buffer);
    common::StationAddress address;
    if (transport::StationManager::s_pInstance->GetStationAddress(&address, Mesh::s_pInstance->m_HostStationIndex).IsFailure()) {
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    common::Time deadline = common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * JOIN_REQUEST_TIMEOUT_MSEC);
    MeshProtocol* pProtocol = Mesh::s_pInstance->m_pMeshProtocol;
    if (transport::ResendingMessageManager::s_pInstance
            ->SetSendMessage(&m_AckId, buffer, pProtocol->GetJoinRequestDataSize(), STATION_INDEX_UNIDENTIFIED, address,
                             Mesh::s_pInstance->m_pMeshProtocol->m_ProtocolId, deadline.m_Tick)
            .IsFailure()) {
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh::s_pInstance->m_pMeshProtocol->m_pJoinMeshJob = this;
    m_IsWaitingResponse = true;
    for (u32 i = 0; i < RESPONSE_PART_NUM_MAX; i++) {
        m_IsPartPending[i] = true;
    }
    SetStep(&JoinMeshJob::WaitRequestAck, "JoinMeshJob::WaitRequestAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042A7AC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::WaitJoinResponse()
{
    if (!m_IsWaitingResponse) {
        SetStep(&JoinMeshJob::AnalyzeJoinResponse, "JoinMeshJob::AnalyzeJoinResponse");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (CheckContextCallCanncelled()) {
        m_IsWaitingResponse = false;
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (CheckTransportConnectionStatus() || CheckConnectionStateWithHostStation()) {
        m_IsWaitingResponse = false;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042A88C
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::WaitAllConnection()
{
    if (CheckContextCallCanncelled()) {
        CancelConnectingToHost();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh* pMesh = Mesh::s_pInstance;
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        // the mesh update failed for us
        nn::Result result;
        if (m_Result == common::RESULT_STATION_CONNECTION_FAILED_E7 || m_Result == common::RESULT_STATION_CONNECTION_FAILED_EA ||
            m_Result == common::RESULT_STATION_CONNECTION_FAILED_EB || m_Result == common::RESULT_STATION_CONNECTION_FAILED_EC ||
            m_Result == common::RESULT_STATION_CONNECTION_FAILED_ED || m_Result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
            result = m_Result;
        } else {
            result = common::RESULT_JOIN_FAILED;
        }
        SignalFailureToCaller(result);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (pMesh->m_pLeaveMeshJob->IsRunning()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }

    // the stations of the response that are connected (with the local one)
    bool isMissing = false;
    u32 stationNum = 0;
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] != localStationIndex) {
            if (m_pIsSkipped[i]) {
                continue;
            }
            if (transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]) == nullptr ||
                !Mesh::s_pInstance->CheckStationIndexIsValid(m_pStationIndices[i])) {
                isMissing = true;
                break;
            }
        }
        stationNum++;
    }
    if (!isMissing && (stationNum != 2 || Mesh::s_pInstance->m_Unknown0x70 == 0 || !Mesh::s_pInstance->m_pProcessUpdateMeshJob->m_IsProcessing)) {
        // all there
        if (Mesh::s_pInstance->m_StationNum == 0) {
            Mesh::s_pInstance->m_StationNum = stationNum;
        }
        Mesh::s_pInstance->SetJoined(true);
        Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_NONE;
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->m_IsJoining = false;
        return ProceedToCompleteProcess();
    }

    // a newer mesh update: no more waiting for the stations that are not in it as they were
    u32 updateCount = Mesh::s_pInstance->m_Unknown0x70;
    if (updateCount != 0) {
        ProcessUpdateMeshJob* pUpdateJob = Mesh::s_pInstance->m_pProcessUpdateMeshJob;
        if (pUpdateJob->m_IsProcessing) {
            updateCount = pUpdateJob->m_Version;
        }
        if (m_UpdateCount < updateCount) {
            for (u32 i = 0; i < m_StationNum; i++) {
                if (m_pIsSkipped[i]) {
                    continue;
                }
                m_pIsSkipped[i] = true;
                for (u32 j = 0; j < pUpdateJob->m_StationNum; j++) {
                    if (pUpdateJob->m_pStationIndices[j] == m_pStationIndices[i]) {
                        if (pUpdateJob->m_pStationConnectionInfos[j] == m_pStationConnectionInfos[i]) {
                            m_pIsSkipped[i] = false;
                        }
                        break;
                    }
                }
            }
            m_UpdateCount = updateCount;
        }
    }
    if (CheckTransportConnectionStatus() || CheckConnectionStateWithHostStation()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042ACA4 (name is ours)
bool nn::pia::session::JoinMeshJob::ParseJoinResponse(const u8* pData, u32)
{
    u32 infoSize = m_pStationConnectionInfos[0].GetSerializedSize();
    u8 stationNum = pData[1];
    u8 localEntry = pData[2];
    u8 hostEntry = pData[3];
    u32 entrySize = infoSize + (3 - (infoSize & 3)) + 1;
    if (stationNum == 0 || localEntry == RESPONSE_ENTRY_NONE || hostEntry == RESPONSE_ENTRY_NONE) {
        // rejected
        m_StationNum = 0;
        m_ResponseCode = static_cast<ResponseCode>(pData[4]);
        Mesh::s_pInstance->m_pMeshProtocol->m_pJoinMeshJob = nullptr;
        m_IsWaitingResponse = false;
        return true;
    }
    if (stationNum > m_StationNumMax) {
        m_StationNum = 0;
        m_ResponseCode = RESPONSE_CODE_INVALID;
        Mesh::s_pInstance->m_pMeshProtocol->m_pJoinMeshJob = nullptr;
        m_IsWaitingResponse = false;
        return true;
    }

    u8 type = pData[4];
    if (type == RESPONSE_TYPE_SINGLE) {
        NotifyJoinResponse(pData);
        StationIndex localStationIndex = static_cast<StationIndex>(pData[localEntry * entrySize + infoSize + RESPONSE_HEADER_SIZE]);
        StationIndex hostStationIndex = static_cast<StationIndex>(pData[hostEntry * entrySize + infoSize + RESPONSE_HEADER_SIZE]);
        if (localStationIndex <= STATION_INDEX_MAX && hostStationIndex <= STATION_INDEX_MAX) {
            if (!UpdateLocalAndHostInformation(localStationIndex, hostStationIndex)) {
                return false;
            }
            m_StationNum = stationNum;
            for (u32 i = 0; i < m_StationNum; i++) {
                m_pStationConnectionInfos[i].Deserialize(pData + RESPONSE_HEADER_SIZE + i * entrySize);
                m_pStationIndices[i] = static_cast<StationIndex>(pData[i * entrySize + infoSize + RESPONSE_HEADER_SIZE]);
            }
        } else {
            m_StationNum = 0;
            m_ResponseCode = RESPONSE_CODE_INVALID;
        }
        Mesh::s_pInstance->m_pMeshProtocol->m_pJoinMeshJob = nullptr;
        m_IsWaitingResponse = false;
        return true;
    }
    if (type < 2 || type > RESPONSE_PART_NUM_MAX) {
        return false;
    }

    // split: byte 5 is the part, bytes 6 and 7 the number and the first of its entries
    u8 part = pData[5];
    if (!m_IsPartPending[part]) {
        return true;
    }
    u32 headerSize = RESPONSE_PART_HEADER_SIZE;
    if (part == 0) {
        headerSize = RESPONSE_HEADER_SIZE;
        NotifyJoinResponse(pData);
    }
    if (m_IsPartPending[0] & m_IsPartPending[1] & m_IsPartPending[2]) {
        // the first part to arrive
        m_PartStationNum = stationNum;
        m_PartLocalEntry = localEntry;
        m_PartHostEntry = hostEntry;
        m_PartNum = type;
    } else if (m_PartStationNum != stationNum || m_PartLocalEntry != localEntry || m_PartHostEntry != hostEntry || m_PartNum != type) {
        return false;
    }
    u8 entryNum = pData[6];
    u8 entryOffset = pData[7];
    for (int i = 0; i < entryNum; i++) {
        u32 index = entryOffset + i;
        m_pStationConnectionInfos[index].Deserialize(pData + headerSize + i * entrySize);
        m_pStationIndices[index] = static_cast<StationIndex>(pData[i * entrySize + infoSize + headerSize]);
    }
    m_IsPartPending[part] = false;
    bool isPending = false;
    for (u32 i = 0; i < m_PartNum; i++) {
        isPending |= m_IsPartPending[i];
    }
    if (isPending) {
        return true;
    }

    // all parts are there
    StationIndex localStationIndex = m_pStationIndices[m_PartLocalEntry];
    StationIndex hostStationIndex = m_pStationIndices[m_PartHostEntry];
    if (localStationIndex <= STATION_INDEX_MAX && hostStationIndex <= STATION_INDEX_MAX) {
        if (!UpdateLocalAndHostInformation(localStationIndex, hostStationIndex)) {
            return false;
        }
        m_StationNum = m_PartStationNum;
    } else {
        m_StationNum = 0;
        m_ResponseCode = RESPONSE_CODE_INVALID;
    }
    Mesh::s_pInstance->m_pMeshProtocol->m_pJoinMeshJob = nullptr;
    m_IsWaitingResponse = false;
    return true;
}

// 0x0042B08C
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::AnalyzeJoinResponse()
{
    if (CheckContextCallCanncelled()) {
        CancelConnectingToHost();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_StationNum != 0) {
        common::g_SessionBeginMonitoringContent.m_JoinStationNum = m_StationNum;
        m_HostPrincipalId = 0xFFFFFFFF;
        SetStep(&JoinMeshJob::WaitAllConnection, "JoinMeshJob::WaitAllConnection");
        m_Phase = PHASE_CONNECTING_TO_STATIONS;
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // rejected
    SetElapsedTimeToMonitoringData();
    if (m_pCallContext != nullptr) {
        nn::Result result;
        if (m_ResponseCode == RESPONSE_CODE_DENIED) {
            result = common::RESULT_JOIN_DENIED;
        } else if (m_ResponseCode == RESPONSE_CODE_REFUSED) {
            result = common::RESULT_JOIN_REFUSED;
        } else if (m_ResponseCode == RESPONSE_CODE_2) {
            NotifyEvent(Mesh::EVENT_TYPE_20);
            result = common::RESULT_JOIN_FAILED;
        } else {
            result = common::RESULT_INVALID_JOIN_RESPONSE;
        }
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalFailure(result);
        }
        common::g_SessionBeginMonitoringContent.m_JoinResult = result.GetPrintableBits();
        common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
        Mesh::s_pInstance->SetSessionBeginMonitoringData();
        m_pCallContext = nullptr;
    }
    CancelConnectingToHost();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042B28C (name is ours)
nn::pia::session::JoinMeshJob::Phase nn::pia::session::JoinMeshJob::GetPhase() const
{
    return m_Phase;
}

// 0x0042B294
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::StartConnectingToHost()
{
    if (CheckContextCallCanncelled()) {
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = PHASE_CONNECTING_TO_HOST;
    transport::Station* pStation = transport::StationManager::s_pInstance->CreateStation();
    if (!pStation->Startup(Mesh::s_pInstance->m_pStationProtocol)) {
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        pStation->Cleanup();
        pStation->CleanupJobs();
        transport::StationManager::s_pInstance->DestroyStation(pStation);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    pStation->m_StationIndex = Mesh::s_pInstance->m_HostStationIndex;
    pStation->m_StationAddress = m_pStationConnectionInfos->m_PublicLocation.m_StationAddress;
    transport::StationConnectionInfoTable::s_pInstance->AddToTable(pStation, *m_pStationConnectionInfos);
    common::Time now;
    now.SetNow();
    pStation->m_LocalConnectionId = static_cast<u8>(static_cast<u64>(now.m_Tick) % 254 + 2);
    transport::ConnectStationJob* pJob = pStation->m_pConnectStationJob;
    if (pJob->GetState() != common::Job::EXECUTE_STATE_IDLE && pJob->GetState() != common::Job::EXECUTE_STATE_FINISHED) {
        pJob->Cleanup();
        pJob->Reset(true);
    }
    if (pJob->Startup(m_pConnectCallContext, pStation, *m_pStationConnectionInfos, false, pStation->m_pStationProtocol->m_ProcessTimeoutMSec).IsFailure()) {
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
        pStation->Cleanup();
        pStation->CleanupJobs();
        transport::StationManager::s_pInstance->DestroyStation(pStation);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    pStation->m_State = transport::Station::STATION_STATE_2;
    pJob->Ready(false);
    SetStep(&JoinMeshJob::WaitUntilConnectToHost, "JoinMeshJob::WaitUntilConnectToHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042B580
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::WaitUntilConnectToHost()
{
    if (CheckContextCallCanncelled()) {
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    common::CallContext* pConnectCallContext = m_pConnectCallContext;
    if (!pConnectCallContext->IsFinished()) {
        if (CheckTransportConnectionStatus()) {
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nn::Result connectResult = pConnectCallContext->m_Result;
    if (connectResult.IsSuccess()) {
        if (transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex) != nullptr) {
            SetStep(&JoinMeshJob::SendJoinRequest, "JoinMeshJob::SendJoinRequest");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        SignalFailureToCaller(common::RESULT_JOIN_FAILED);
        CancelConnectingToHost();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result;
    if (connectResult == common::RESULT_CONNECTION_REFUSED) {
        result = common::RESULT_JOIN_REFUSED;
    } else if (connectResult == common::RESULT_INCOMPATIBLE_VERSION || connectResult == common::RESULT_STATION_CONNECTION_FAILED_E7 ||
               connectResult == common::RESULT_STATION_CONNECTION_FAILED_EA || connectResult == common::RESULT_STATION_CONNECTION_FAILED_EB ||
               connectResult == common::RESULT_STATION_CONNECTION_FAILED_EC || connectResult == common::RESULT_STATION_CONNECTION_FAILED_ED ||
               connectResult == common::RESULT_STATION_CONNECTION_FAILED_F2) {
        result = connectResult;
    } else {
        result = common::RESULT_JOIN_FAILED;
    }
    if (IsStationConnectionFailure(result)) {
        NotifyEvent(Mesh::EVENT_TYPE_CONNECTION_FAILED);
    }
    SignalFailureToCaller(result);
    CancelConnectingToHost();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042B884 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::JoinMeshJob::CheckContextCallCanncelled()
{
    if (m_pCallContext == nullptr || !m_pCallContext->IsCancelRequested()) {
        return false;
    }
    CancelConnectingToHost();
    if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pCallContext->SignalCancel();
    }
    SetElapsedTimeToMonitoringData();
    CheckRelayConnectionForMonitoring();
    common::g_SessionBeginMonitoringContent.m_JoinResult = common::RESULT_CANCELED;
    common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
    common::g_SessionBeginMonitoringContent.m_JoinPhase = m_Phase;
    Mesh::s_pInstance->SetSessionBeginMonitoringData();
    return true;
}

// 0x0042B910 (name is ours)
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::LeaveMeshWithHostMigration()
{
    if (Mesh::s_pInstance->m_StationNum > 1) {
        Mesh::s_pInstance->LeaveMeshWithHostMigration(&m_CallContext);
        SetStep(&JoinMeshJob::WaitLeaveMesh, "JoinMeshJob::WaitLeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&JoinMeshJob::CompleteCancelWithLeaveMesh, "JoinMeshJob::CompleteCancelWithLeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042B9E0
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::CompleteCancelWithLeaveMesh()
{
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_LEAVE;
    CancelConnectingToHost();
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    SetElapsedTimeToMonitoringData();
    CheckRelayConnectionForMonitoring();
    common::g_SessionBeginMonitoringContent.m_JoinResult = common::RESULT_CANCELED;
    common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
    common::g_SessionBeginMonitoringContent.m_JoinPhase = m_Phase;
    Mesh::s_pInstance->SetSessionBeginMonitoringData();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042BA68 slot 0x20
void nn::pia::session::JoinMeshJob::SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo&)
{
    // empty (in the original too)
}

// 0x0042BA6C | fefates:callgraph [tier C]
bool nn::pia::session::JoinMeshJob::UpdateLocalAndHostInformation(nn::pia::StationIndex localStationIndex, nn::pia::StationIndex hostStationIndex)
{
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pHostStation == nullptr || pLocalStation == nullptr) {
        return false;
    }
    pLocalStation->m_StationIndex = localStationIndex;
    Mesh::s_pInstance->m_LocalStationIndex = localStationIndex;
    Mesh::s_pInstance->StartUse(localStationIndex);
    if (transport::Transport::s_pInstance->m_ProtocolManager.StartupProtocols(localStationIndex).IsFailure()) {
        return false;
    }
    pLocalStation->m_State = transport::Station::STATION_STATE_CONNECTED;
    pHostStation->m_StationIndex = hostStationIndex;
    Mesh::s_pInstance->m_HostStationIndex = hostStationIndex;
    Mesh::s_pInstance->NoticeMeshEvent(Mesh::EVENT_TYPE_JOIN, localStationIndex);
    Mesh::s_pInstance->FixConnectedId(hostStationIndex);
    transport::StationManager::s_pInstance->m_Unknown0xA8 = hostStationIndex;
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (common::IsValidPointer(pRelayRouteManager)) {
        pRelayRouteManager->SetRelayRoute(localStationIndex, hostStationIndex, hostStationIndex);
    }
    return true;
}

// 0x0042BB60 | fefates:callgraph [tier C]
bool nn::pia::session::JoinMeshJob::CheckTransportConnectionStatus()
{
    if (transport::Transport::s_pInstance->m_StreamResult.IsSuccess()) {
        return false;
    }
    SetElapsedTimeToMonitoringData();
    CheckRelayConnectionForMonitoring();
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalFailure(common::RESULT_JOIN_FAILED);
        }
        common::g_SessionBeginMonitoringContent.m_JoinResult = common::RESULT_JOIN_FAILED;
        common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
        Mesh::s_pInstance->SetSessionBeginMonitoringData();
        m_pCallContext = nullptr;
    }
    CancelConnectingToHost();
    return true;
}

// 0x0042BC08 slot 0x24 (name is ours)
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::ProceedToCompleteProcess()
{
    SetStep(&JoinMeshJob::CompleteProcess, "JoinMeshJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042BC60 | fefates:callgraph [tier C]
bool nn::pia::session::JoinMeshJob::CheckConnectionStateWithHostStation()
{
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
    if (pHostStation != nullptr && pHostStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
        return false;
    }
    SetElapsedTimeToMonitoringData();
    CheckRelayConnectionForMonitoring();
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalFailure(common::RESULT_JOIN_FAILED);
        }
        common::g_SessionBeginMonitoringContent.m_JoinResult = common::RESULT_JOIN_FAILED;
        common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
        Mesh::s_pInstance->SetSessionBeginMonitoringData();
        m_pCallContext = nullptr;
    }
    CancelConnectingToHost();
    return true;
}

// 0x0042BD24 | fefates:callgraph [tier C]
void nn::pia::session::JoinMeshJob::Cleanup(nn::Result result)
{
    CleanupImpl();
    CancelConnectingToHost();
    ClearWaitingFlags();
    if (m_AckId != 0) {
        if (transport::ResendingMessageManager::s_pInstance != nullptr) {
            transport::ResendingMessageManager::s_pInstance->StopResending(m_AckId);
        }
        m_AckId = 0;
    }
    m_UpdateCount = 0;
    ClearSkipFlags();
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.Cancel();
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            SetElapsedTimeToMonitoringData();
            CheckRelayConnectionForMonitoring();
            nn::Result failure = result.IsFailure() ? result : nn::Result(common::RESULT_CANCELED);
            if (!m_pCallContext->IsFinished()) {
                m_pCallContext->SignalFailure(failure);
            }
            common::g_SessionBeginMonitoringContent.m_JoinResult = failure.GetPrintableBits();
            common::g_SessionBeginMonitoringContent.m_HostPrincipalId = m_HostPrincipalId;
            Mesh::s_pInstance->SetSessionBeginMonitoringData();
        }
        m_pCallContext = nullptr;
    }
    m_Phase = PHASE_NONE;
}

// 0x0042BE9C | fefates:callgraph
bool nn::pia::session::JoinMeshJob::Startup(const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext)
{
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    if (!StartupImpl()) {
        return false;
    }
    *m_pStationConnectionInfos = info;
    m_Result = nn::Result();
    ClearWaitingFlags();
    m_AckId = 0;
    m_UpdateCount = 0;
    ClearSkipFlags();
    Mesh::s_pInstance->ClearSessionBeginMonitoringData();
    SetHostInfoToMonitoringData(info);
    m_HostPrincipalId = info.m_PublicLocation.m_PrincipalId;
    Mesh::s_pInstance->m_pProcessUpdateMeshJob->m_IsJoining = true;
    common::Time now;
    now.SetNow();
    m_StartTime = now;
    return true;
}

// 0x0042BFC8 (name is ours)
nn::pia::common::ExecuteResult nn::pia::session::JoinMeshJob::LeaveMesh()
{
    Mesh::s_pInstance->LeaveMesh(&m_CallContext);
    SetStep(&JoinMeshJob::WaitLeaveMesh, "JoinMeshJob::WaitLeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042C030
nn::pia::session::JoinMeshJob::JoinMeshJob()
    : m_pCallContext(nullptr), m_AckId(0), m_IsWaitingResponse(false), m_ResponseCode(RESPONSE_CODE_INVALID), m_Phase(PHASE_NONE),
      m_Result(common::RESULT_NOT_SET), m_StartTime()
{
    for (u32 i = 0; i < RESPONSE_PART_NUM_MAX; i++) {
        m_IsPartPending[i] = false;
    }
    m_UpdateCount = 0;
    void* pBuffer = pead::AllocMemory(sizeof(common::CallContext), common::HeapManager::GetHeap());
    m_pConnectCallContext = ::new (pBuffer) common::CallContext();
    m_StationNumMax = Mesh::s_pInstance->m_StationNumMax;
    m_pStationConnectionInfos = common::NewArray<transport::StationConnectionInfo>(m_StationNumMax);
    m_pStationIndices = common::NewArray<StationIndex>(m_StationNumMax);
    m_pIsSkipped = common::NewArray<bool>(m_StationNumMax);
    ClearSkipFlags();
}

// 0x0042C304
// 0x0042C1FC (deleting dtor)
nn::pia::session::JoinMeshJob::~JoinMeshJob()
{
    CancelConnectingToHost();
    if (m_pConnectCallContext != nullptr) {
        m_pConnectCallContext->~CallContext();
        pead::FreeMemory(m_pConnectCallContext);
    }
    if (m_pStationConnectionInfos != nullptr) {
        common::DeleteArray(m_pStationConnectionInfos);
    }
    if (m_pStationIndices != nullptr) {
        common::DeleteArray(m_pStationIndices);
    }
    if (m_pIsSkipped != nullptr) {
        common::DeleteArray(m_pIsSkipped);
    }
}

// 0x00733580 (name is ours)
void nn::pia::session::JoinMeshJob::SetElapsedTimeToMonitoringData()
{
    s64 elapsed = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    if (elapsed >= 0) {
        common::g_SessionBeginMonitoringContent.m_JoinElapsedMSec = elapsed;
        common::g_SessionBeginMonitoringContent.m_JoinPhase = m_Phase;
    }
}

// 0x007335D8 | fefates:callgraph [tier C]
void nn::pia::session::JoinMeshJob::CheckRelayConnectionForMonitoring() const
{
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (!common::IsValidPointer(pRelayRouteManager)) {
        return;
    }
    transport::StationManager* pStationManager = transport::StationManager::s_pInstance;
    if (!common::IsValidPointer(pStationManager)) {
        return;
    }
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    if (!common::IsValidPointer(pTable)) {
        return;
    }
    Mesh* pMesh = Mesh::s_pInstance;
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    // the stations behind a relay and the relays
    u32 relayedPrincipalIds[23] = {};
    u32 relayPrincipalIds[transport::StationManager::STATION_NUM_MAX] = {};
    StationIndex relayStationIndices[transport::StationManager::STATION_NUM_MAX];
    u8 relayedNum = 0;
    u8 relayNum = 0;
    for (transport::Station** it = pStationManager->m_ActiveStations.Begin(); it != pStationManager->m_ActiveStations.End(); it++) {
        if ((*it)->m_State != transport::Station::STATION_STATE_CONNECTED) {
            continue;
        }
        if (!pMesh->CheckStationIndexIsValid((*it)->m_StationIndex)) {
            continue;
        }
        StationIndex relayStationIndex;
        if (pRelayRouteManager->GetRelayRoute(localStationIndex, (*it)->m_StationIndex, &relayStationIndex).IsFailure()) {
            continue;
        }
        if (relayStationIndex == STATION_INDEX_UNIDENTIFIED || (*it)->m_StationIndex == relayStationIndex) {
            continue;
        }
        relayedPrincipalIds[relayedNum++] = pTable->GetPrincipalIdByStation(*it);
        for (int i = 0; i <= relayNum; i++) {
            if (i == relayNum) {
                relayStationIndices[relayNum++] = relayStationIndex;
                break;
            }
            if (relayStationIndices[i] == relayStationIndex) {
                break;
            }
        }
    }
    for (int i = 0; i < relayNum; i++) {
        relayPrincipalIds[i] = pTable->GetPrincipalIdByStation(static_cast<const transport::StationManager*>(pStationManager)->GetStation(relayStationIndices[i]));
    }
    common::g_SessionBeginMonitoringContent.m_RelayedStationNum = relayedNum;
    // up to relayNum (not relayedNum) in the original
    for (int i = 0; i <= relayNum; i++) {
        common::g_SessionBeginMonitoringContent.m_RelayedStationPrincipalIdHashes[i] =
            relayedPrincipalIds[i] != 0 ? common::hashWithMd5(relayedPrincipalIds[i]) : 0xFFFFFFFF;
    }
    common::g_SessionBeginMonitoringContent.m_RelayStationNum = relayNum;
    for (int i = 0; i < relayNum; i++) {
        common::g_SessionBeginMonitoringContent.m_RelayStationPrincipalIdHashes[i] =
            relayPrincipalIds[i] != 0 ? common::hashWithMd5(relayPrincipalIds[i]) : 0xFFFFFFFF;
    }
}

// 0x00733834 slot 0x14
void nn::pia::session::JoinMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
