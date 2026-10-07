#include "nn/pia/session/session_ProcessJoinRequestJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
const u64 TRACE_FLAG = 0x80000000ULL;
const s32 TIMEOUT_MSEC = 15000;
// Mesh::CheckApprovalJoin: no objection
const u8 REJECT_REASON_NONE = 0xFF;
// the reason when the application rejects it, and the one of a cleared job
const u8 REJECT_REASON_APPLICATION = 1;
const u8 REJECT_REASON_DEFAULT = 3;
// a part of the join response: its size and the stations in it
const u32 RESPONSE_BUFFER_SIZE = 1450;
const int RESPONSE_PART_STATION_NUM = 9;
} // namespace

// 0x0043F848 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::InitialStep()
{
    m_AckId = 0;
    m_ResponseAckIds[0] = 0;
    m_ResponseAckIds[1] = 0;
    m_ResponseAckIds[2] = 0;
    m_IsProcessing = false;
    m_RejectReason = REJECT_REASON_DEFAULT;
    m_JoiningStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_IsCanceled = false;
    // MeshProtocol resumes it with the next request
    SetStep(&ProcessJoinRequestJob::CheckApprovalJoin, "ProcessJoinRequestJob::CheckApprovalJoin");
    return common::ExecuteResult(common::ExecuteResult::STATE_SUSPEND);
}

// 0x0043F8DC
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::JoinSucceeded()
{
    if (!m_IsCanceled) {
        Mesh::s_pInstance->m_StationNum++;
        Mesh::s_pInstance->m_pMeshProtocol->SendStationDataList(true);
    }
    SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043F96C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::WaitResponseAck()
{
    bool isResending = false;
    for (u32 i = 0; i < RESPONSE_PART_NUM; i++) {
        isResending |= transport::ResendingMessageManager::s_pInstance->CheckNowResending(m_ResponseAckIds[i]);
    }
    if (!isResending) {
        // all parts arrived
        SetStep(&ProcessJoinRequestJob::JoinSucceeded, "ProcessJoinRequestJob::JoinSucceeded");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline || m_IsCanceled) {
        for (u32 i = 0; i < RESPONSE_PART_NUM; i++) {
            transport::ResendingMessageManager::s_pInstance->StopResending(m_ResponseAckIds[i]);
            m_ResponseAckIds[i] = 0;
        }
        m_Deadline = common::Time();
        SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043FAC4
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::SendJoinResponse()
{
    if (transport::StationManager::s_pInstance->m_pLocalStation == nullptr) {
        SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_JoiningStationAddress);
    if (!common::IsValidPointer(pStation)) {
        SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_JoiningStationIndex = Mesh::s_pInstance->GetFreeStationIndex();
    if (m_JoiningStationIndex == STATION_INDEX_UNIDENTIFIED) {
        // the mesh is full
        SetStep(&ProcessJoinRequestJob::SendDenyingJoinResponse, "ProcessJoinRequestJob::SendDenyingJoinResponse");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }

    // the old connections of the same station go
    {
        transport::Station* pOldStations[STATION_INDEX_MAX + 1] = {};
        u32 oldStationNum = 0;
        transport::StationConnectionInfo info;
        transport::StationConnectionInfo otherInfo;
        if (transport::StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(pStation, &info).IsSuccess() &&
            info.m_PublicLocation.m_StationAddress.IsValid()) {
            for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
                 it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
                if (*it == pStation) {
                    continue;
                }
                if (transport::StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(*it, &otherInfo).IsFailure()) {
                    continue;
                }
                if (info.m_PublicLocation.m_StationKey == otherInfo.m_PublicLocation.m_StationKey &&
                    info.m_PublicLocation.m_StationAddress.GetExtensionId() == otherInfo.m_PublicLocation.m_StationAddress.GetExtensionId()) {
                    pOldStations[oldStationNum++] = *it;
                }
            }
            if (oldStationNum != 0) {
                bool isMeshChanged = false;
                for (u32 i = 0; i < oldStationNum; i++) {
                    pOldStations[i]->Trace(TRACE_FLAG);
                    StationIndex stationIndex = pOldStations[i]->m_StationIndex;
                    if (Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
                        Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
                        isMeshChanged = true;
                        if (Mesh::s_pInstance->m_StationNum != 0) {
                            Mesh::s_pInstance->m_StationNum--;
                        }
                    }
                    pOldStations[i]->m_pDisconnectStationJob->OnDisconnected(pOldStations[i]);
                    transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pOldStations[i]);
                    transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pOldStations[i]);
                    pOldStations[i]->Cleanup();
                    pOldStations[i]->CleanupJobs();
                    transport::StationManager::s_pInstance->DestroyStation(pOldStations[i]);
                }
                transport::Transport::s_pInstance->OutputStreamUpdateEvent();
                if (isMeshChanged) {
                    Mesh::s_pInstance->m_pMeshProtocol->SendStationDataList(true);
                }
            }
        }
    }

    pStation->m_StationIndex = m_JoiningStationIndex;
    Mesh::s_pInstance->FixConnectedId(m_JoiningStationIndex);
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (common::IsValidPointer(pRelayRouteManager)) {
        pRelayRouteManager->SetRelayRoute(Mesh::s_pInstance->m_LocalStationIndex, m_JoiningStationIndex, m_JoiningStationIndex);
    }
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);

    // the response, in up to three parts
    u8* pBuffers[RESPONSE_PART_NUM] = {m_pResponseBuffers[0], m_pResponseBuffers[1], m_pResponseBuffers[2]};
    u32 sizes[RESPONSE_PART_NUM] = {0, 0, 0};
    u32 partNum = Mesh::s_pInstance->m_pMeshProtocol->MakeJoinResponseData(m_JoiningStationIndex, pBuffers, sizes);
    if (partNum - 1 > RESPONSE_PART_NUM - 1) {
        SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u32 i;
    for (i = 0; i < partNum; i++) {
        if (transport::ResendingMessageManager::s_pInstance
                ->SetSendMessage(&m_ResponseAckIds[i], pBuffers[i], sizes[i], STATION_INDEX_UNIDENTIFIED, pStation->m_StationAddress,
                                 Mesh::s_pInstance->m_pMeshProtocol->m_ProtocolId, m_Deadline.m_Tick)
                .IsFailure()) {
            for (int j = static_cast<int>(i) - 1; j >= 0; j--) {
                transport::ResendingMessageManager::s_pInstance->StopResending(m_ResponseAckIds[j]);
            }
            SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    for (; static_cast<int>(i) < static_cast<int>(RESPONSE_PART_NUM); i++) {
        m_ResponseAckIds[i] = 0;
    }
    SetStep(&ProcessJoinRequestJob::WaitResponseAck, "ProcessJoinRequestJob::WaitResponseAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004400B8
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::CheckApprovalJoin()
{
    // (a trace call of the address was removed by the linker here)
    m_IsProcessing = true;
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_JoiningStationAddress);
    if (pStation == nullptr || pStation->m_State != transport::Station::STATION_STATE_CONNECTED) {
        SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u8 reason = Mesh::s_pInstance->CheckApprovalJoin(pStation);
    if (reason != REJECT_REASON_NONE) {
        m_RejectReason = reason;
        SetStep(&ProcessJoinRequestJob::SendDenyingJoinResponse, "ProcessJoinRequestJob::SendDenyingJoinResponse");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the application decides after the identification info
    if (Mesh::s_pInstance->m_JoinApprovalCallback != nullptr) {
        transport::Station::IdentificationInfo identificationInfo;
        if (transport::IdentificationInfoTable::s_pInstance->GetIdentificationInfo(pStation, &identificationInfo).IsFailure()) {
            SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (!Mesh::s_pInstance->m_JoinApprovalCallback(&identificationInfo) ||
            (Session::s_pInstance != nullptr && !Session::s_pInstance->IsJoinable(0))) {
            m_RejectReason = REJECT_REASON_APPLICATION;
            // the number of rejected joins (0 stays for none)
            u8 rejectedNum = common::g_SessionStateMonitoringContent.m_Unknown0x3C8;
            common::g_SessionStateMonitoringContent.m_Unknown0x3C8 = rejectedNum == 0xFF ? 1 : rejectedNum + 1;
            SetStep(&ProcessJoinRequestJob::SendDenyingJoinResponse, "ProcessJoinRequestJob::SendDenyingJoinResponse");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    SetStep(&ProcessJoinRequestJob::SendJoinResponse, "ProcessJoinRequestJob::SendJoinResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00440320 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::CancellationNotice(nn::pia::StationIndex stationIndex)
{
    if (m_JoiningStationIndex == stationIndex) {
        m_IsCanceled = true;
    }
}

// 0x00440334 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::ProcessJoinRequestJob::SendDenyingJoinResponse()
{
    if (transport::StationManager::s_pInstance->m_pLocalStation != nullptr) {
        Mesh::s_pInstance->m_pMeshProtocol->SendJoinRejection(m_JoiningStationAddress, m_RejectReason);
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_JoiningStationAddress);
        if (pStation != nullptr) {
            pStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
        }
    }
    SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00440408 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::Cleanup()
{
    transport::ResendingMessageManager* pManager = transport::ResendingMessageManager::s_pInstance;
    if (pManager != nullptr) {
        if (m_AckId != 0) {
            pManager->StopResending(m_AckId);
        }
        for (u32 i = 0; i < RESPONSE_PART_NUM; i++) {
            transport::ResendingMessageManager::s_pInstance->StopResending(m_ResponseAckIds[i]);
        }
    }
    m_AckId = 0;
    m_ResponseAckIds[0] = 0;
    m_ResponseAckIds[1] = 0;
    m_ResponseAckIds[2] = 0;
    m_IsProcessing = false;
    m_RejectReason = REJECT_REASON_DEFAULT;
    m_JoiningStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_IsCanceled = false;
    m_Deadline = common::Time();
}

// 0x00440488 | fefates:bytes [tier B]
bool nn::pia::session::ProcessJoinRequestJob::Startup()
{
    Mesh::s_pInstance->m_pMeshProtocol->m_pProcessJoinRequestJob = this;
    m_AckId = 0;
    m_ResponseAckIds[0] = 0;
    m_ResponseAckIds[1] = 0;
    m_ResponseAckIds[2] = 0;
    m_JoiningStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_IsCanceled = false;
    Reset(true);
    SetStep(&ProcessJoinRequestJob::InitialStep, "ProcessJoinRequestJob::InitialStep");
    return true;
}

// 0x00440520 | fefates:bytes-fuzzy [tier B]
nn::pia::session::ProcessJoinRequestJob::ProcessJoinRequestJob()
    : m_Deadline(), m_TimeoutMSec(TIMEOUT_MSEC), m_AckId(0), m_JoiningStationAddress()
{
    m_JoiningStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_IsProcessing = false;
    m_RejectReason = REJECT_REASON_DEFAULT;
    m_IsCanceled = false;
    // a buffer per part the largest mesh needs
    int stationNumMax = Mesh::s_pInstance->m_StationNumMax;
    u32 partNum = stationNumMax / RESPONSE_PART_STATION_NUM + (stationNumMax % RESPONSE_PART_STATION_NUM > 0 ? 1 : 0);
    for (u32 i = 0; i < RESPONSE_PART_NUM; i++) {
        if (i < partNum) {
            m_pResponseBuffers[i] = common::NewArray<u8>(RESPONSE_BUFFER_SIZE, 4);
        } else {
            m_pResponseBuffers[i] = nullptr;
        }
    }
    m_ResponseAckIds[0] = 0;
    m_ResponseAckIds[1] = 0;
    m_ResponseAckIds[2] = 0;
}

// 0x004406B8
// 0x00440644 (deleting dtor)
nn::pia::session::ProcessJoinRequestJob::~ProcessJoinRequestJob()
{
    for (u32 i = 0; i < RESPONSE_PART_NUM; i++) {
        if (m_pResponseBuffers[i] != nullptr) {
            common::DeleteArray(m_pResponseBuffers[i]);
        }
    }
}

// 0x00426F30
void nn::pia::session::ProcessJoinRequestJob::SetJoiningStationData(nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address)
{
    m_JoiningStationIndex = stationIndex;
    m_JoiningStationAddress = address;
}

// 0x0073405C slot 0x14
void nn::pia::session::ProcessJoinRequestJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
