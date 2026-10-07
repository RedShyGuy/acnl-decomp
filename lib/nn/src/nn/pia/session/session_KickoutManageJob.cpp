#include "nn/pia/session/session_KickoutManageJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the kickout reasons the join result tells (the others give RESULT_JOIN_KICKED_OUT)
const u8 KICKOUT_REASON_FIRST = 3;
const u8 KICKOUT_REASON_LAST = 23;
} // namespace

// 0x004382C4 | fefates:bytes [tier B]
nn::Result nn::pia::session::KickoutManageJob::StartKickout(nn::pia::StationIndex stationIndex, nn::pia::session::KickoutManageJob::KickoutReason reason)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 freeIndex = STATION_INDEX_MAX + 1;
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        StationIndex entryStationIndex = m_Entries[i].m_StationIndex;
        if (entryStationIndex == stationIndex) {
            return common::RESULT_ALREADY_KICKED_OUT;
        }
        if (entryStationIndex > STATION_INDEX_MAX && freeIndex == STATION_INDEX_MAX + 1) {
            freeIndex = i;
        }
    }
    if (freeIndex < STATION_INDEX_MAX + 1 && Mesh::s_pInstance->m_pMeshProtocol->SendKickoutNotice(stationIndex, reason)) {
        m_Entries[freeIndex].m_StationIndex = stationIndex;
        m_Entries[freeIndex].m_Reason = reason;
        return nn::Result();
    }
    return common::RESULT_BUFFER_IS_FULL;
}

// 0x00438374 (name is ours)
bool nn::pia::session::KickoutManageJob::ReceiveKickoutNotice(KickoutReason reason)
{
    StartupImpl();
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS || IsRunning()) {
        return false;
    }
    Reset(true);
    m_Reason = reason;
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_pJoinMeshJob->IsRunning()) {
        // the join fails with the reason
        u8 index = m_Reason - KICKOUT_REASON_FIRST;
        if (index > KICKOUT_REASON_LAST - KICKOUT_REASON_FIRST) {
            m_Result = common::RESULT_JOIN_DENIED;
        } else if (index == 1) {
            m_Result = common::RESULT_JOIN_KICKED_OUT_4;
        } else if (index == 2) {
            m_Result = common::RESULT_JOIN_KICKED_OUT_5;
        } else if (index == 3) {
            m_Result = common::RESULT_JOIN_KICKED_OUT_6;
        } else {
            m_Result = common::RESULT_JOIN_KICKED_OUT;
        }
        m_IsJoinCanceled = true;
        if (!pMesh->m_pProcessUpdateMeshJob->IsRunning()) {
            if (pMesh->GetJoinMeshJobPhase() == 3) {
                SetStep(&KickoutManageJob::ClientStartLeaveMesh, "KickoutManageJob::ClientStartLeaveMesh");
            } else {
                SetStep(&KickoutManageJob::ClientFinalize, "KickoutManageJob::ClientFinalize");
            }
            return true;
        }
    }
    SetStep(&KickoutManageJob::ClientStartLeaveMesh, "KickoutManageJob::ClientStartLeaveMesh");
    if (pMesh->m_pLeaveMeshJob->IsRunning()) {
        m_Reason = static_cast<KickoutReason>(0);
    }
    return true;
}

// 0x00438528
nn::pia::common::ExecuteResult nn::pia::session::KickoutManageJob::ClientFinalize()
{
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_BY_HOST;
    Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_BY_HOST);
    if (m_IsJoinCanceled && Mesh::s_pInstance->m_pJoinMeshJob->IsRunning()) {
        Mesh::s_pInstance->m_pJoinMeshJob->Cleanup(m_Result);
        Mesh::s_pInstance->m_pJoinMeshJob->Reset(true);
    }
    ClearStationIndices();
    if (m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalSuccess(nn::Result());
        }
        m_pCallContext = nullptr;
    }
    m_IsJoinCanceled = false;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00438608 slot 0x18 (name is ours)
void nn::pia::session::KickoutManageJob::StartupImpl()
{
    // empty (in the original too)
}

// 0x0043860C slot 0x1C (name is ours)
void nn::pia::session::KickoutManageJob::OnKickout(const nn::pia::common::StationAddress&)
{
    // empty (in the original too)
}

// 0x00438610 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::KickoutManageJob::ClientWaitLeaveMesh()
{
    if (!m_CallContext.IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&KickoutManageJob::ClientFinalize, "KickoutManageJob::ClientFinalize");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00438690 | fefates:bytes [tier B]
bool nn::pia::session::KickoutManageJob::AssociateKickoutWith(nn::pia::common::CallContext* pCallContext)
{
    if (m_pCallContext != nullptr) {
        return false;
    }
    m_Reason = static_cast<KickoutReason>(0);
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    return true;
}

// 0x004386C8
nn::pia::common::ExecuteResult nn::pia::session::KickoutManageJob::ClientStartLeaveMesh()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && (pMesh->CheckJoined() != common::RESULT_NOT_JOINED || m_IsJoinCanceled)) {
        LeaveMeshJob* pJob = pMesh->m_pLeaveMeshJob;
        if (pJob->Startup(&m_CallContext)) {
            pMesh->m_pLeaveMeshJob->Ready(false);
        } else if (!pMesh->m_pLeaveMeshJob->RegisterExtraCallback(&m_CallContext)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        SetStep(&KickoutManageJob::ClientWaitLeaveMesh, "KickoutManageJob::ClientWaitLeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&KickoutManageJob::ClientFinalize, "KickoutManageJob::ClientFinalize");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004387F0 | fefates:bytes [tier B]
void nn::pia::session::KickoutManageJob::SetLeaveEventStationIndex(nn::pia::StationIndex stationIndex)
{
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_Entries[i].m_StationIndex == stationIndex) {
            m_Entries[i].m_StationIndex = STATION_INDEX_UNIDENTIFIED;
            common::StationAddress address;
            if (transport::StationManager::s_pInstance->GetStationAddress(&address, stationIndex).IsSuccess()) {
                OnKickout(address);
            }
            return;
        }
    }
}

// 0x00438890 (name is ours)
void nn::pia::session::KickoutManageJob::Cleanup()
{
    ClearStationIndices();
    if (m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalSuccess(nn::Result());
        }
        m_pCallContext = nullptr;
    }
    m_IsJoinCanceled = false;
}

// 0x00438904 (name is ours)
bool nn::pia::session::KickoutManageJob::ClearEntries()
{
    ClearStationIndices();
    m_Reason = static_cast<KickoutReason>(0);
    m_IsJoinCanceled = false;
    return true;
}

// 0x00438950
nn::pia::session::KickoutManageJob::KickoutManageJob()
    : m_Reason(static_cast<KickoutReason>(0)), m_pCallContext(nullptr), m_IsJoinCanceled(false), m_Result(common::RESULT_NOT_SET)
{
}

// 0x004389B4
// 0x0043898C (deleting dtor)
nn::pia::session::KickoutManageJob::~KickoutManageJob()
{
    // empty (in the original too)
}

// 0x007338F8 slot 0x14
void nn::pia::session::KickoutManageJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
