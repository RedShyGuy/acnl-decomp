#include "nn/pia/inet/inet_NexJointSessionJob.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexJoinSessionSetting.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteria.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_SessionProtocol.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
typedef common::ObjList<StationId> StationIdList;

// a counter of the monitoring data (0xFF means "not set", so it wraps to 1)
inline void Increment(u8& count)
{
    count = count == 0xFF ? 1 : count + 1;
}

inline common::Time GetTimeAfter(s64 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

inline common::Time GetTimeBefore(s64 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime - common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

inline NexMatchMeshLayerController* GetController()
{
    return static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController);
}

// the matchmake session of the current session / of the other one
inline session::CommonMatchmakeSession* GetCurrentMatchmakeSession()
{
    session::Session* pSession = session::Session::s_pInstance;
    return pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
}

inline session::CommonMatchmakeSession* GetOtherMatchmakeSession()
{
    session::Session* pSession = session::Session::s_pInstance;
    return pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
}

inline bool IsSessionDisconnected()
{
    return session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5;
}

// the phases of the joint sessions in which the local station is the leader of the new session
inline bool IsLeaderPhase(u8 phase)
{
    return phase == 8 || phase == 6 || phase == 7;
}

const common::ExecuteResult::State CONTINUE = common::ExecuteResult::STATE_CONTINUE;
const common::ExecuteResult::State NEXT_DISPATCH = common::ExecuteResult::STATE_NEXT_DISPATCH;
const common::ExecuteResult::State SUCCESS = common::ExecuteResult::STATE_SUCCESS;

// the timeouts (ms)
const s64 RETRY_TIMEOUT_MSEC = 12000;
const s64 SHORT_RETRY_TIMEOUT_MSEC = 3000;
const s64 PREPARED_TIMEOUT_MSEC = 15000;
const s64 NOTIFICATION_TIMEOUT_MSEC = 10000;
const s64 COMPANION_TIMEOUT_MSEC = 40000;
const s64 MESH_RESTART_DELAY_MSEC = 5000;
// the time of the restart is set back by that much, so that the wait for the companion ends sooner
const s64 COMPANION_RESTART_OFFSET_MSEC = 36000;
const s64 INVITATION_TIMEOUT_MSEC = 20000;
const s64 LEAVE_MESH_COMPANION_TIMEOUT_MSEC = 30000;
// the invitation is sent again after that many ms without all answers
const s64 RESEND_INTERVAL_MSEC = 2000;
// the wait before the previous mesh is left
const u16 LEAVE_MESH_WAIT_MSEC = 1000;
// the connection info of the next host is traced with it (the call was removed)
const u64 CONNECTION_INFO_TRACE_FLAG = 0x2000000000ULL;

// the values of the monitoring data that are not set
const u8 INVALID_U8 = 0xFF;
const u16 INVALID_U16 = 0xFFFF;
const u32 INVALID_U32 = 0xFFFFFFFF;
// the session host is set again at most every that many ms
const s64 UPDATE_SESSION_HOST_INTERVAL_MSEC = 10000;
const u8 RETRY_COUNT_MAX = 2;
} // namespace

// 0x003E8910
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::MeshRestart()
{
    session::Mesh::s_pInstance->Cleanup();
    session::Session::s_pInstance->CleanupConfigParticipationJob();
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    RemoveUnknownStations();
    UpdateUnknown0x1AD();
    pSession->m_pStationIdStatusTable->ClearValid();
    pSession->m_pStationIdStatusTable->ClearUnknown0x13();
    session::Session::s_pInstance->m_DisconnectState = 0;
    m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    session::CommonMatchmakeSession* pMatchmakeSession;
    if (IsLeaderPhase(m_Phase)) {
        u8 otherIndex = pSession->m_CurrentIndex == 0 ? 1 : 0;
        NexFacade::s_pInstance->m_Unknown0x10 = pSession->m_SessionIds[otherIndex];
        NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
        if (!pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex]) ||
            !pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0])) {
            m_Unknown0xC0 = 24;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        m_Unknown0x99 = pMatchmakeSession->vf_0x88() && pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88();
        bool isRetry = false;
        if (pMatchmakeSession->vf_0x88()) {
            m_Unknown0x190 = 0;
            if (m_Unknown0x1AD && pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                pMatchmakeSession->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]);
            } else {
                isRetry = true;
            }
        } else {
            StationId ownerStationId;
            ownerStationId.m_Low = pMatchmakeSession->vf_0x90();
            m_Unknown0x190 = 0;
            u32 sessionId;
            if (pSession->m_pStationIdStatusTable->GetSessionId(ownerStationId, &sessionId) &&
                pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88() && pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                if (!pController->HasParticipant(pSession->m_SessionIds[pSession->m_CurrentIndex], ownerStationId.m_Low)) {
                    m_Result = common::RESULT_SESSION_OWNER_LEFT;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
                if (m_Unknown0x1AD) {
                    pMatchmakeSession->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]);
                    m_Unknown0x99 = true;
                } else {
                    isRetry = true;
                }
            }
        }
        if (isRetry) {
            m_RetryDeadline = GetTimeAfter(RETRY_TIMEOUT_MSEC);
            m_RetryCount++;
            if (m_RetryCount > RETRY_COUNT_MAX) {
                m_Result = common::RESULT_SESSION_OWNER_LEFT;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            SetStep(&NexJointSessionJob::WaitStartRetryJoinMesh, "NexJointSessionJob::WaitStartRetryJoinMesh");
            return common::ExecuteResult(CONTINUE);
        }
    } else {
        NexFacade::s_pInstance->m_Unknown0x10 = pSession->m_SessionIds[pSession->m_CurrentIndex];
        m_Unknown0x188 = 0;
        if (!static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController)->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex])) {
            m_Unknown0xC0 = 24;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        m_Unknown0x99 = pMatchmakeSession->vf_0x88();
        if (!m_Unknown0x99) {
            m_Unknown0x98 = 0;
            m_StationId = GetStationIdOfIndex253();
        } else if (!m_Unknown0x98) {
            bool isFound = false;
            for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
                if (m_StationId == pNode->m_Value) {
                    isFound = true;
                    break;
                }
            }
            if (isFound) {
                m_Unknown0x99 = 0;
            } else {
                m_Unknown0x98 = 1;
                m_StationId = pSession->m_LocalStationId;
                if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]).IsFailure()) {
                    m_Unknown0xC0 = 23;
                    m_Result = common::RESULT_UNREGISTER_FAILED;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            }
        }
    }
    nn::Result result = pSession->m_pMeshLayerController->StartupMesh(pMatchmakeSession, true);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 26;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_Unknown0x1AA = 0;
    m_IsMeshEvent19 = false;
    m_IsMeshEvent20 = false;
    if (m_Unknown0x99) {
        m_JobPhase = 15;
        SetStep(&NexJointSessionJob::StartCreateNextMesh, "NexJointSessionJob::StartCreateNextMesh");
    } else {
        SetStep(&NexJointSessionJob::StartGetNextMeshHostStationConnectionInfo, "NexJointSessionJob::StartGetNextMeshHostStationConnectionInfo");
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003E9248
void nn::pia::inet::NexJointSessionJob::vf_0x60()
{
    m_JobPhase = 0;
    m_Unknown0x17D = 0;
    m_StationId0x180 = GetStationIdOfIndex253();
    m_Unknown0x188 = 0;
    m_Unknown0x18C = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x194 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    m_Unknown0x1AA = 0;
    m_IsUnregistering = 0;
    m_Unknown0x1AC = 0;
    m_Result = nn::Result();
    m_ConnectionInfo.SetStationConnectionInfo(transport::StationConnectionInfo());
}

// 0x003E92C4
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitLeaveMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    switch (m_Unknown0x59) {
    case 1:
        if (!pMesh->IsLeaveMeshAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetLeaveMeshAsyncResult();
        break;
    case 2:
        if (!pMesh->IsLeaveMeshWithHostMigrationAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetLeaveMeshWithHostMigrationAsyncResult();
        break;
    case 3:
        if (!pMesh->IsDestroyMeshAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetDestroyMeshAsyncResult();
        break;
    }
    SetStep(&NexJointSessionJob::StartLeaveCurrentMatchmakeSession, "NexJointSessionJob::StartLeaveCurrentMatchmakeSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003E9468
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::ProcessFailure()
{
    if (m_Unknown0xC0 == 0) {
        m_Unknown0xC0 = 57;
    }
    session::Session::s_pInstance->m_Unknown0xB1 = 0;
    session::Session::s_pInstance->m_Unknown0xB2 = false;
    FailJointSession();
    if (common::IsValidPointer(m_pCallContext)) {
        if (IsSessionDisconnected()) {
            m_Result = common::RESULT_NOT_IN_SESSION;
        }
        m_pCallContext->SignalFailure(m_Result);
        m_pCallContext = nullptr;
    }
    m_Result = nn::Result();
    m_JobPhase = 22;
    session::Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    session::Mesh::s_pInstance->SendMonitoringData(true);
    return common::ExecuteResult(SUCCESS);
}

// 0x003E9520
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartLeaveMesh()
{
    m_Unknown0x9D = 1;
    session::Session::s_pInstance->m_Unknown0xB1 = 1;
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->CheckJoined() == common::RESULT_NOT_JOINED) {
        SetStep(&NexJointSessionJob::StartLeaveCurrentMatchmakeSession, "NexJointSessionJob::StartLeaveCurrentMatchmakeSession");
        return common::ExecuteResult(CONTINUE);
    }
    nn::Result result;
    if (pMesh->m_LocalStationIndex <= 11 && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        if (session::Session::s_pInstance->m_IsHostMigrationEnabled) {
            result = pMesh->LeaveMeshWithHostMigrationAsync();
            m_Unknown0x59 = 2;
        } else {
            result = pMesh->DestroyMeshAsync();
            m_Unknown0x59 = 3;
        }
    } else {
        result = pMesh->LeaveMeshAsync();
        m_Unknown0x59 = 1;
    }
    if (result.IsFailure()) {
        SetStep(&NexJointSessionJob::StartLeaveCurrentMatchmakeSession, "NexJointSessionJob::StartLeaveCurrentMatchmakeSession");
        return common::ExecuteResult(CONTINUE);
    }
    SetStep(&NexJointSessionJob::WaitLeaveMesh, "NexJointSessionJob::WaitLeaveMesh");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003E9728
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::ProcessComplete()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session::s_pInstance->m_Unknown0xB2 = false;
    if (!CompleteJointSession()) {
        session::Session::s_pInstance->m_Unknown0xB2 = true;
        m_Unknown0xC0 = 37;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext)) {
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
    }
    m_JobPhase = 22;
    session::Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    session::Mesh::s_pInstance->SendMonitoringData(true);
    return common::ExecuteResult(SUCCESS);
}

// 0x003E9970
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitMeshRestart()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_MeshRestartDeadline < common::Scheduler::s_pInstance->m_DispatchTime) {
        SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
    }
    if (m_Unknown0x98) {
        SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
    }
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003E9B0C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::CallSessionEvent()
{
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure()) {
        if (common::IsValidPointer(m_pCallContext)) {
            m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            m_pCallContext = nullptr;
        }
    } else if (IsSessionDisconnected()) {
        if (common::IsValidPointer(m_pCallContext)) {
            m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
            m_pCallContext = nullptr;
        }
    } else {
        BeginJointSession();
        session::Session::s_pInstance->m_Unknown0xB2 = true;
        session::Mesh::s_pInstance->m_IsMonitoringDataSent = true;
        m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
        m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
        if (IsSessionDisconnected()) {
            m_Unknown0x9D = 1;
            m_Unknown0xC0 = 3;
            m_Result = common::RESULT_NOT_IN_SESSION;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
            return common::ExecuteResult(CONTINUE);
        }
        if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            m_Unknown0xC0 = 4;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_IsFailed) {
            m_Result = m_FailureResult;
            m_Unknown0xC0 = 6;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_Unknown0x1AC) {
            m_Unknown0xC0 = 15;
            m_Result = common::RESULT_INVALID_STATE;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_Phase == 9) {
            m_MessageType = 6;
            SetStep(&NexJointSessionJob::SendAnswerToDestroyInvitation, "NexJointSessionJob::SendAnswerToDestroyInvitation");
        } else if (m_Unknown0x98) {
            SetStep(&NexJointSessionJob::CloseMatchmakeSession, "NexJointSessionJob::CloseMatchmakeSession");
        } else {
            m_MessageType = 6;
            m_JobPhase = 4;
            SetStep(&NexJointSessionJob::SendAnswerToInvitation, "NexJointSessionJob::SendAnswerToInvitation");
        }
        return common::ExecuteResult(CONTINUE);
    }
    m_Result = nn::Result();
    m_JobPhase = 22;
    return common::ExecuteResult(SUCCESS);
}

// 0x003E9EE0
u8 nn::pia::inet::NexJointSessionJob::GetPhase() const
{
    return m_JobPhase;
}

// 0x003E9EE8
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitJoinNextMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->m_IsCancelRequested) {
        common::CallContext::State state = m_pCallContext->m_State;
        if (state != common::CallContext::STATE_CALL_SUCCESS && state != common::CallContext::STATE_CALL_FAILURE &&
            state != common::CallContext::STATE_CALL_CANCEL) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    session::Session* pSession = session::Session::s_pInstance;
    if (!pMesh->IsJoinMeshAsyncCompleted()) {
        if (m_IsFailed) {
            pMesh->CancelJoinMeshAsync();
        }
        if ((IsLeaderPhase(m_Phase) && m_Unknown0x190) || ((m_Phase == 9 || m_Phase == 10) && m_Unknown0x188)) {
            pMesh->CancelJoinMeshAsync();
            if (m_Unknown0x1A8 != 2) {
                m_Unknown0x1A8 = 0;
            }
        }
    }
    if (pSession->m_LocalStationId == pSession->m_HostStationId) {
        if (pMesh->GetJoinMeshJobPhase() >= 2) {
            m_Unknown0x1A8 = 2;
        }
    }
    StationId ownerStationId;
    ownerStationId.m_Low = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90();
    if (pSession->m_pStationIdStatusTable->IsValid(ownerStationId) && !pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
        m_Unknown0x1A8 = 2;
    }
    if (!pMesh->IsJoinMeshAsyncCompleted()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    nn::Result result = session::Mesh::s_pInstance->GetJoinMeshAsyncResult();
    if (result.IsSuccess()) {
        if (IsLeaderPhase(m_Phase)) {
            m_Unknown0x9F = true;
        }
        StationId localStationId = pSession->m_LocalStationId;
        pSession->m_pStationIdStatusTable->SetUnknown0x13(localStationId, true);
        session::Session::s_pInstance->m_DisconnectState = 1;
        SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
        m_JobPhase = 17;
        common::g_SessionBeginMonitoringContent.m_Unknown0x180 =
            (common::Scheduler::s_pInstance->m_DispatchTime - m_Time0xB8).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
        return common::ExecuteResult(CONTINUE);
    }
    bool isRetry;
    if (result == common::RESULT_STATION_CONNECTION_FAILED_E7 || result == common::RESULT_STATION_CONNECTION_FAILED_E9 ||
        result == common::RESULT_STATION_CONNECTION_FAILED_ED || result == common::RESULT_STATION_CONNECTION_FAILED_EC ||
        result == common::RESULT_STATION_CONNECTION_FAILED_EB || result == common::RESULT_JOIN_FAILED ||
        result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
        isRetry = true;
    } else {
        isRetry = result == common::RESULT_CANCELED && !m_IsFailed;
    }
    if (isRetry) {
        Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x22B);
        m_Unknown0x1A8++;
        if (m_Unknown0x1A8 < RETRY_COUNT_MAX) {
            session::Session::s_pInstance->m_Unknown0xB1 = 1;
            session::Session::s_pInstance->m_DisconnectState = 0;
            session::Mesh::s_pInstance->Cleanup();
            if (IsLeaderPhase(m_Phase) && m_Unknown0x190 && pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x251);
                SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            if (m_IsMeshEvent19) {
                m_RetryDeadline = GetTimeAfter(RETRY_TIMEOUT_MSEC);
                Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x252);
            } else {
                m_RetryDeadline = GetTimeAfter(SHORT_RETRY_TIMEOUT_MSEC);
                if (result == common::RESULT_JOIN_FAILED || result == common::RESULT_STATION_CONNECTION_FAILED_E7 ||
                    result == common::RESULT_STATION_CONNECTION_FAILED_EA || result == common::RESULT_STATION_CONNECTION_FAILED_ED ||
                    result == common::RESULT_STATION_CONNECTION_FAILED_EB || result == common::RESULT_STATION_CONNECTION_FAILED_EC ||
                    result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
                    Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x24F);
                } else if (m_IsMeshEvent20) {
                    Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x253);
                }
            }
            SetStep(&NexJointSessionJob::WaitStartRetryJoinMesh, "NexJointSessionJob::WaitStartRetryJoinMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    pSession->m_pMeshLayerController->vf_0x14();
    if (m_IsFailed) {
        m_Unknown0xC0 = 6;
        m_Result = m_FailureResult;
    } else {
        u8 code;
        if (result == common::RESULT_CANCELED) {
            code = 39;
        } else if (result == common::RESULT_INVALID_STATE) {
            code = 40;
        } else if (result == common::RESULT_JOIN_FAILED) {
            code = 41;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_E7) {
            code = 42;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EA) {
            code = 43;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_ED) {
            code = 44;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EB) {
            code = 45;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EC) {
            code = 46;
        } else if (result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
            code = 47;
        } else if (result == common::RESULT_JOIN_DENIED) {
            code = 48;
        } else if (result == common::RESULT_JOIN_REFUSED) {
            code = 49;
        } else if (result == common::RESULT_JOIN_KICKED_OUT_4) {
            code = 50;
        } else if (result == common::RESULT_JOIN_KICKED_OUT_5) {
            code = 51;
        } else if (result == common::RESULT_JOIN_KICKED_OUT_6) {
            code = 52;
        } else if (result == common::RESULT_JOIN_KICKED_OUT) {
            code = 53;
        } else if (result == common::RESULT_INVALID_JOIN_RESPONSE) {
            code = 54;
        } else if (result == common::RESULT_INCOMPATIBLE_VERSION) {
            code = 55;
        } else {
            code = 28;
        }
        m_Unknown0xC0 = code;
        m_Result = result;
    }
    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
    UpdateMonitoringPhase();
    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EA700
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitNotification()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_Time0x170 < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Result = common::RESULT_UNREGISTER_FAILED;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    NexMatchMeshLayerController* pController = GetController();
    if (pController->HasParticipant(m_JointSessionId, NexFacade::s_pInstance->m_pNgsBridge->vf_0x0C())) {
        SetStep(&NexJointSessionJob::SendNextSessionId, "NexJointSessionJob::SendNextSessionId");
        return common::ExecuteResult(CONTINUE);
    }
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EA8EC
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendNextSessionId()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->m_IsCancelRequested) {
        common::CallContext::State state = m_pCallContext->m_State;
        if (state != common::CallContext::STATE_CALL_SUCCESS && state != common::CallContext::STATE_CALL_FAILURE &&
            state != common::CallContext::STATE_CALL_CANCEL) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    m_JobPhase = 8;
    session::Session* pSession = session::Session::s_pInstance;
    if (m_Unknown0x98 && m_Unknown0x17D) {
        StationId localStationId = pSession->m_LocalStationId;
        if (m_StationId0x180 != localStationId) {
            m_Unknown0x17D = 0;
            m_Unknown0x98 = 0;
            m_StationId = m_StationId0x180;
            SetStep(&NexJointSessionJob::SendPreparedForMigrateSession, "NexJointSessionJob::SendPreparedForMigrateSession");
            return common::ExecuteResult(CONTINUE);
        }
    }
    if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    StationId targets[11];
    u32 targetNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            targets[targetNum] = pNode->m_Value;
            targetNum++;
        }
    }
    if (targetNum > 1) {
        const common::SignatureSetting* pSignatureSetting = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->GetSignatureSetting();
        u32 ownerPrincipalId = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90();
        nn::Result result = pSession->m_pSessionProtocol->SendSessionInfo(m_Phase, pSession->m_SessionIds[pSession->m_CurrentIndex], m_JointSessionId,
                                                                          ownerPrincipalId, pSignatureSetting, targets, targetNum);
        if (result != common::RESULT_BUFFER_IS_FULL) {
            if (result == common::RESULT_NOT_FOUND) {
                u32 reachableNum = 0;
                for (u32 i = 0; i < targetNum; i++) {
                    if (targets[i] != pSession->m_LocalStationId && pSession->m_pStationIdStatusTable->IsValid(targets[i])) {
                        reachableNum++;
                    }
                }
                if (reachableNum != 0) {
                    m_Unknown0xC0 = 9;
                    m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else if (result.IsFailure()) {
                m_Unknown0xC0 = 3;
                m_Result = result;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    pSession->m_pStationIdStatusTable->ClearStatus();
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    StationId localStationId = pSession->m_LocalStationId;
    pSession->m_pStationIdStatusTable->SetStatus(localStationId, true);
    localStationId = pSession->m_LocalStationId;
    pSession->m_pStationIdStatusTable->SetUnknown0x12(localStationId, true);
    m_Time0x120 = common::Scheduler::s_pInstance->m_DispatchTime;
    m_Time0x150 = GetTimeAfter(PREPARED_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitCompanionStationPrepared, "NexJointSessionJob::WaitCompanionStationPrepared");
    return common::ExecuteResult(CONTINUE);
}

// 0x003EAF2C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartJoinNextMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session::s_pInstance->m_Unknown0xB1 = 0;
    nn::Result result = session::Mesh::s_pInstance->JoinMeshAsync(m_ConnectionInfo);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 28;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&NexJointSessionJob::WaitJoinNextMesh, "NexJointSessionJob::WaitJoinNextMesh");
    return common::ExecuteResult(CONTINUE);
}

// 0x003EB114
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitHostStationId()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    transport::StationIdTable::Entry entry;
    entry.m_StationId = StationId();
    // the station the host is checked against (the owner of the session for the leader)
    StationId checkStationId;
    StationId hostStationId;
    StationIndex hostStationIndex = pMesh->m_HostStationIndex;
    u8 currentIndex = pSession->m_CurrentIndex;
    if (IsLeaderPhase(m_Phase)) {
        NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
        if (!pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex]) ||
            !pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0])) {
            m_Unknown0xC0 = 24;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(hostStationIndex);
        if (pStation == nullptr) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        u32 principalId;
        if (pStation->GetPrincipalId(&principalId).IsFailure()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, principalId).IsFailure()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        hostStationId = entry.m_StationId;
        if (!pSession->m_pStationIdStatusTable->IsValid(hostStationId)) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90())
                .IsFailure()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        checkStationId = entry.m_StationId;
        if (!pSession->m_pStationIdStatusTable->IsValid(checkStationId)) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (hostStationId == pSession->m_LocalStationId) {
            if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x88()) {
                if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                    if (m_Unknown0x188) {
                        if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x138).GetTick() <
                            common::TimeSpan::GetTicksPerMSec().GetTick() * UPDATE_SESSION_HOST_INTERVAL_MSEC) {
                            return common::ExecuteResult(NEXT_DISPATCH);
                        }
                        m_Unknown0x188 = 0;
                        m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
                        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                        return common::ExecuteResult(NEXT_DISPATCH);
                    }
                } else {
                    if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x138).GetTick() >=
                        common::TimeSpan::GetTicksPerMSec().GetTick() * UPDATE_SESSION_HOST_INTERVAL_MSEC) {
                        m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
                        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                    }
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else {
                if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x138).GetTick() >=
                    common::TimeSpan::GetTicksPerMSec().GetTick() * UPDATE_SESSION_HOST_INTERVAL_MSEC) {
                    m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
                    pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x74(
                        pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]);
                }
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        } else {
            if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90() != hostStationId.m_Low) {
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            u32 sessionId;
            pSession->m_pStationIdStatusTable->GetSessionId(hostStationId, &sessionId);
            if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() != hostStationId.m_Low) {
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else if (checkStationId == pSession->m_LocalStationId) {
                pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
            }
        }
    } else {
        if (!static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController)->HasSessionEntry(pSession->m_SessionIds[currentIndex])) {
            m_Unknown0xC0 = 24;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(hostStationIndex);
        if (pStation == nullptr) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        u32 principalId;
        if (pStation->GetPrincipalId(&principalId).IsFailure()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, principalId).IsFailure()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        checkStationId = entry.m_StationId;
        if (!pSession->m_pStationIdStatusTable->IsValid(checkStationId)) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (checkStationId == pSession->m_LocalStationId) {
            if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x138).GetTick() >=
                    common::TimeSpan::GetTicksPerMSec().GetTick() * UPDATE_SESSION_HOST_INTERVAL_MSEC) {
                    m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
                    pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
                }
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        } else if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() != checkStationId.m_Low) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    // the host of the mesh must belong to another session than the station
    if (checkStationId != pSession->m_HostStationId) {
        StationId meshHostStationId = pSession->m_HostStationId;
        if (pSession->m_pStationIdStatusTable->IsValid(meshHostStationId)) {
            u32 hostSessionId = 0;
            u32 sessionId = 0;
            meshHostStationId = pSession->m_HostStationId;
            if (!pSession->m_pStationIdStatusTable->GetSessionId(meshHostStationId, &hostSessionId) ||
                !pSession->m_pStationIdStatusTable->GetSessionId(checkStationId, &sessionId) || hostSessionId == sessionId) {
                m_Unknown0xC0 = 36;
                m_Result = common::RESULT_SESSION_OWNER_LEFT;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    SetStep(&NexJointSessionJob::ProcessComplete, "NexJointSessionJob::ProcessComplete");
    return common::ExecuteResult(CONTINUE);
}

// 0x003EB9E4
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitNextSessionId()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->m_pStationIdStatusTable->IsValid(m_StationId)) {
        // the leader left: the session id of the other session is known only with host migration
        u8 code;
        if (!pSession->m_IsHostMigrationEnabled) {
            code = 5;
        } else if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == 0) {
            code = 20;
        } else if (session::Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
            code = 4;
        } else {
            code = 0;
        }
        if (code != 0) {
            m_Unknown0xC0 = code;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    }
    if (m_JointSessionId == 0 || !static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController)->HasSessionEntry(m_JointSessionId)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_JobPhase = 10;
    session::Session* pCurrentSession = session::Session::s_pInstance;
    if (pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex] == m_JointSessionId) {
        m_Unknown0xC0 = 17;
        m_Result = common::RESULT_INVALID_STATE;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = m_JointSessionId;
    common::g_SessionBeginMonitoringContent.m_Unknown0x170 = m_JointSessionId;
    m_MessageType = 0;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90() != 0) {
        SetStep(&NexJointSessionJob::SendPreparedForMigrateSession, "NexJointSessionJob::SendPreparedForMigrateSession");
    } else {
        SetStep(&NexJointSessionJob::GetNextMatchmakeSessionInfo, "NexJointSessionJob::GetNextMatchmakeSessionInfo");
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003EBE40
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCreateNextMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (!pMesh->IsCreateMeshAsyncCompleted()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    nn::Result result = pMesh->GetCreateMeshAsyncResult();
    if (result.IsFailure()) {
        m_Unknown0xC0 = 27;
        session::Session::s_pInstance->m_pMeshLayerController->vf_0x14();
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (IsLeaderPhase(m_Phase)) {
        if (session::Session::s_pInstance->IsUsingStationIdTable()) {
            session::Session* pSession = session::Session::s_pInstance;
            transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex == 0 ? 1 : 0]);
        }
        m_Unknown0x9F = true;
    } else if (session::Session::s_pInstance->IsUsingStationIdTable()) {
        session::Session* pSession = session::Session::s_pInstance;
        transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex]);
    }
    StationId localStationId = session::Session::s_pInstance->m_LocalStationId;
    session::Session::s_pInstance->m_pStationIdStatusTable->SetUnknown0x13(localStationId, true);
    session::Session::s_pInstance->m_DisconnectState = 1;
    SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
    m_JobPhase = 17;
    common::g_SessionBeginMonitoringContent.m_Unknown0x180 =
        (common::Scheduler::s_pInstance->m_DispatchTime - m_Time0xB8).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    return common::ExecuteResult(CONTINUE);
}

// 0x003EC1F8
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::ResendNextSessionId()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (m_Unknown0x98 && m_Unknown0x17D && m_StationId0x180 != pSession->m_LocalStationId) {
        m_Unknown0x17D = 0;
        m_Unknown0x98 = 0;
        m_StationId = m_StationId0x180;
        SetStep(&NexJointSessionJob::SendPreparedForMigrateSession, "NexJointSessionJob::SendPreparedForMigrateSession");
        return common::ExecuteResult(CONTINUE);
    }
    // to the stations that did not answer yet
    StationId targets[11];
    u32 targetNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value) && pSession->m_pStationIdStatusTable->GetStatus(pNode->m_Value) == 0 &&
            !pSession->m_pStationIdStatusTable->GetUnknown0x12(pNode->m_Value)) {
            targets[targetNum] = pNode->m_Value;
            targetNum++;
        }
    }
    if (targetNum != 0) {
        const common::SignatureSetting* pSignatureSetting = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->GetSignatureSetting();
        u32 ownerPrincipalId = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90();
        nn::Result result = pSession->m_pSessionProtocol->SendSessionInfo(m_Phase, pSession->m_SessionIds[pSession->m_CurrentIndex], m_JointSessionId,
                                                                          ownerPrincipalId, pSignatureSetting, targets, targetNum);
        if (result != common::RESULT_BUFFER_IS_FULL && result.IsFailure()) {
            if (result == common::RESULT_NOT_FOUND) {
                m_Unknown0xC0 = 9;
                m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
            } else {
                m_Unknown0xC0 = 3;
                m_Result = result;
            }
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    m_Time0x120 = common::Scheduler::s_pInstance->m_DispatchTime;
    SetStep(&NexJointSessionJob::WaitCompanionStationPrepared, "NexJointSessionJob::WaitCompanionStationPrepared");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EC740
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartCreateNextMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session::s_pInstance->m_Unknown0xB1 = 0;
    nn::Result result = session::Mesh::s_pInstance->CreateMeshAsync();
    if (result.IsFailure()) {
        m_Unknown0xC0 = 27;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&NexJointSessionJob::WaitCreateNextMesh, "NexJointSessionJob::WaitCreateNextMesh");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EC930
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitRandomMatchmake()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    u32 sessionId = 0;
    u32 jointSessionId;
    NexMatchmakeSession* pOtherSession = static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession());
    if (!pOtherSession->IsAutoMatchmakeWithParticipantsCompleted(&sessionId, &m_Unknown0x99, &jointSessionId, nullptr, nullptr)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    pOtherSession->FinishBrowse();
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 16;
        m_Result = m_CallContext.m_Result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (session::Session::s_pInstance->m_pMeshLayerController->vf_0x34() && jointSessionId != 0) {
        m_Unknown0xC0 = 17;
        m_Result = common::RESULT_INVALID_STATE;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_JointSessionId = sessionId;
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
    if (m_Phase == 8) {
        common::g_SessionBeginMonitoringContent.m_Unknown0x170 = m_JointSessionId;
    }
    m_Time0x170 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitNotification, "NexJointSessionJob::WaitNotification");
    return common::ExecuteResult(CONTINUE);
}

// 0x003ECC3C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartRandomMatchmake()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pOtherSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    // the principal ids of the other stations of the joint session
    u32 principalIds[11] = {};
    u32 principalNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pNode->m_Value != m_StationId && pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            principalIds[principalNum] = pNode->m_Value.m_Low;
            principalNum++;
        }
    }
    session::Session* pCurrentSession = session::Session::s_pInstance;
    u32 sessionId = pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex];
    nn::Result result = pOtherSession->AutoMatchmakeWithParticipantsAsync(&m_CallContext, principalIds, principalNum, sessionId,
                                                                          pCurrentSession->m_pMeshLayerController->vf_0x34());
    if (result.IsFailure()) {
        m_Unknown0xC0 = 16;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitRandomMatchmake, "NexJointSessionJob::WaitRandomMatchmake");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003ED028
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCompanionStation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (static_cast<u32>(m_StationIdList.GetCount()) < 2) {
        SetStep(&NexJointSessionJob::WaitHostStationId, "NexJointSessionJob::WaitHostStationId");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    transport::StationIdTable::Entry entry;
    entry.m_StationId = StationId();
    if ((common::Scheduler::s_pInstance->m_DispatchTime - m_RestartTime).GetTick() >=
        common::TimeSpan::GetTicksPerMSec().GetTick() * COMPANION_TIMEOUT_MSEC) {
        m_Unknown0xC0 = 35;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    bool hasEntries;
    if (!pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex])) {
        hasEntries = false;
    } else if (IsLeaderPhase(m_Phase)) {
        hasEntries = pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex]) &&
                     pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    } else {
        hasEntries = pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex]);
    }
    if (!hasEntries) {
        m_Unknown0xC0 = 24;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88() && pSession->m_LocalStationId == pSession->m_HostStationId) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    // all stations of the joint session must be in the new mesh
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        stationIds[stationNum] = pNode->m_Value;
        stationNum++;
    }
    if (!pSession->m_pStationIdStatusTable->AreValid(stationIds, stationNum) || session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90()).IsFailure()) {
        m_Unknown0xC0 = 21;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
        m_MessageType = 19;
        m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
        SetStep(&NexJointSessionJob::WaitUntilCompanionCompletion, "NexJointSessionJob::WaitUntilCompanionCompletion");
        return common::ExecuteResult(CONTINUE);
    }
    m_StationId = entry.m_StationId;
    m_Time0x160 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::SendCompletionForMigrateSession, "NexJointSessionJob::SendCompletionForMigrateSession");
    m_JobPhase = 18;
    return common::ExecuteResult(CONTINUE);
}

// 0x003ED7A4
void nn::pia::inet::NexJointSessionJob::vf_0x58(u32 sessionId, u32 principalId)
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
        for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
            if (pNode->m_Value.m_Low == principalId) {
                m_StationIdList.Erase(&pNode->m_Value);
                break;
            }
        }
    }
    UpdateUnknown0x1AD();
}

// 0x003ED83C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::CloseMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result =
        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->CloseParticipationAsync(&m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex]);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 1;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitCloseMatchmakeSession, "NexJointSessionJob::WaitCloseMatchmakeSession");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EDAA0
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendDestroyInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (m_Time0x158 < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Unknown0xC0 = 32;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (static_cast<u32>(m_StationIdList.GetCount()) > 1) {
        StationId stationIds[11];
        u32 stationNum = 0;
        for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
        }
        StationId localStationId = pSession->m_LocalStationId;
        nn::Result result = pSession->m_pSessionProtocol->SendStationList8(m_Phase, stationIds, stationNum, &localStationId, nullptr, 0);
        if (result != common::RESULT_BUFFER_IS_FULL) {
            if (result == common::RESULT_NOT_FOUND) {
                u32 reachableNum = 0;
                for (u32 i = 0; i < stationNum; i++) {
                    if (stationIds[i] != pSession->m_LocalStationId && pSession->m_pStationIdStatusTable->IsValid(stationIds[i])) {
                        reachableNum++;
                    }
                }
                if (reachableNum != 0) {
                    m_Unknown0xC0 = 7;
                    m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else if (result.IsFailure()) {
                m_Unknown0xC0 = 3;
                m_Result = result;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    m_MessageType = 9;
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    pSession->m_pStationIdStatusTable->ClearUnknown0x1();
    pSession->m_pStationIdStatusTable->SetUnknown0x1(m_StationId, true);
    pSession->m_pStationIdStatusTable->SetUnknown0x12(m_StationId, true);
    m_Time0x118 = common::Scheduler::s_pInstance->m_DispatchTime;
    m_Time0x150 = GetTimeAfter(PREPARED_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitForAnswerToDestroyInvitation, "NexJointSessionJob::WaitForAnswerToDestroyInvitation");
    return common::ExecuteResult(CONTINUE);
}

// 0x003EE044
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitLeavePreviousMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    switch (m_Unknown0x59) {
    case 1:
        if (!pMesh->IsLeaveMeshAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetLeaveMeshAsyncResult();
        break;
    case 2:
        if (!pMesh->IsLeaveMeshWithHostMigrationAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetLeaveMeshWithHostMigrationAsyncResult();
        break;
    case 3:
        if (!pMesh->IsDestroyMeshAsyncCompleted()) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        pMesh->GetDestroyMeshAsyncResult();
        break;
    }
    session::Mesh::s_pInstance->Cleanup();
    if (m_Phase == 9 || m_Phase == 10) {
        m_Unknown0x9F = false;
        SetStep(&NexJointSessionJob::StartLeavePreviousMatchmakeSession, "NexJointSessionJob::StartLeavePreviousMatchmakeSession");
    } else {
        m_MeshRestartDeadline = GetTimeAfter(MESH_RESTART_DELAY_MSEC);
        SetStep(&NexJointSessionJob::WaitMeshRestart, "NexJointSessionJob::WaitMeshRestart");
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003EE314 (name is ours)
void nn::pia::inet::NexJointSessionJob::ResetMonitoringContent()
{
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_JoinStationNum = INVALID_U8;
    content.m_JoinResult = INVALID_U32;
    content.m_RelayedStationNum = INVALID_U8;
    for (int i = 0; i < 23; i++) {
        content.m_RelayedStationPrincipalIdHashes[i] = INVALID_U32;
    }
    content.m_RelayStationNum = INVALID_U8;
    for (int i = 0; i < 12; i++) {
        content.m_RelayStationPrincipalIdHashes[i] = INVALID_U32;
    }
    content.m_Unknown0x170 = INVALID_U32;
    content.m_Unknown0x174 = INVALID_U32;
    content.m_Unknown0x178 = INVALID_U32;
    content.m_Unknown0x17C = INVALID_U8;
    content.m_Unknown0x17D = INVALID_U8;
    content.m_Unknown0x180 = INVALID_U32;
    content.m_Unknown0x184 = INVALID_U32;
    for (int i = 0; i < 6; i++) {
        content.m_Unknown0x188[i] = INVALID_U32;
        content.m_Unknown0x1AC[i] = INVALID_U8;
        content.m_Unknown0x1B4[i] = INVALID_U8;
        content.m_Unknown0x1CC[i] = INVALID_U8;
        content.m_Unknown0x1F0[i] = INVALID_U8;
        content.m_Unknown0x1F8[i] = INVALID_U8;
        content.m_Unknown0x210[i] = INVALID_U8;
    }
    content.m_Unknown0x1A0 = INVALID_U16;
    content.m_Unknown0x22D = INVALID_U8;
    content.m_Unknown0x1A4 = INVALID_U32;
    content.m_Unknown0x1A8 = INVALID_U8;
    content.m_Unknown0x1A9 = INVALID_U8;
    content.m_Unknown0x1AA = INVALID_U8;
    content.m_Unknown0x1AB = INVALID_U8;
    content.m_Unknown0x1E4 = INVALID_U8;
    content.m_Unknown0x1E5 = INVALID_U8;
    content.m_Unknown0x1E6 = INVALID_U8;
    content.m_Unknown0x230 = INVALID_U32;
    content.m_Unknown0x234 = INVALID_U32;
    content.m_Unknown0x238 = INVALID_U32;
    content.m_Unknown0x23C = INVALID_U8;
    content.m_Unknown0x23D = INVALID_U8;
    content.m_Unknown0x1E8 = INVALID_U32;
    content.m_Unknown0x1EC = INVALID_U8;
    content.m_Unknown0x1ED = INVALID_U8;
    content.m_Unknown0x1EE = INVALID_U8;
    content.m_Unknown0x1EF = INVALID_U8;
    content.m_Unknown0x228 = INVALID_U8;
    content.m_Unknown0x229 = INVALID_U8;
    content.m_Unknown0x22A = INVALID_U8;
    content.m_Unknown0x240 = INVALID_U32;
    content.m_Unknown0x244 = INVALID_U32;
    content.m_Unknown0x248 = INVALID_U32;
    content.m_Unknown0x24C = INVALID_U8;
    content.m_Unknown0x24D = INVALID_U8;
    content.m_Unknown0x22B = INVALID_U8;
    content.m_Unknown0x22C = INVALID_U8;
    content.m_Unknown0x24E = INVALID_U8;
    content.m_Unknown0x24F = INVALID_U8;
    content.m_Unknown0x250 = INVALID_U8;
    content.m_Unknown0x252 = INVALID_U8;
    content.m_Unknown0x251 = INVALID_U8;
    content.m_Unknown0x253 = INVALID_U8;
    content.m_HostPrincipalId = INVALID_U32;
    content.m_Unknown0x258 = INVALID_U32;
    content.m_Unknown0x25E = INVALID_U8;
    content.m_Unknown0x25F = INVALID_U8;
    content.m_JoinPhase = INVALID_U8;
    content.m_Unknown0x25D = INVALID_U8;
    content.m_Unknown0x25C = INVALID_U8;
    content.m_Unknown0x270 = INVALID_U8;
    content.m_Unknown0x272 = INVALID_U8;
    for (int i = 0; i < 23; i++) {
        content.m_Unknown0x274[i] = INVALID_U32;
        content.m_Unknown0x2D0[i] = INVALID_U32;
        content.m_Unknown0x32C[i] = INVALID_U32;
        content.m_Unknown0x388[i] = INVALID_U32;
        content.m_Unknown0x3E4[i] = INVALID_U32;
        content.m_Unknown0x440[i] = INVALID_U8;
        content.m_Unknown0x457[i] = INVALID_U8;
        content.m_Unknown0x46E[i] = INVALID_U8;
        content.m_Unknown0x485[i] = INVALID_U8;
    }
    content.m_Unknown0x49C = INVALID_U8;
    content.m_Unknown0x49D = INVALID_U8;
    content.m_Unknown0x49E = INVALID_U16;
    content.m_Unknown0x4A0 = INVALID_U16;
    content.m_Unknown0x4A2 = INVALID_U16;
    common::g_SessionStateMonitoringContent.Cleanup();
}

// 0x003EE500
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendAnswerToInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = pSession->m_pSessionProtocol->SendMessage7(m_Phase, m_StationId, 1);
    if (result == common::RESULT_BUFFER_IS_FULL) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (result.IsFailure()) {
        m_Unknown0xC0 = 5;
        m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (IsLeaderPhase(m_Phase)) {
        m_MessageType = 3;
        SetStep(&NexJointSessionJob::WaitNextSessionId, "NexJointSessionJob::WaitNextSessionId");
        m_JobPhase = 9;
    } else {
        StationId localStationId = pSession->m_LocalStationId;
        pSession->m_pStationIdStatusTable->SetStatus(localStationId, true);
        pSession->m_pStationIdStatusTable->SetStatus(m_StationId, true);
        session::Session::s_pInstance->m_Unknown0xB1 = 1;
        m_MessageType = 10;
        m_Unknown0x9B = 0;
        m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
        SetStep(&NexJointSessionJob::WaitUntilLeaderLeavesMesh, "NexJointSessionJob::WaitUntilLeaderLeavesMesh");
        m_JobPhase = 12;
    }
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EE854
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendLeaveMeshCompanion()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (m_Phase == 9 && pSession->IsHostOfBothSessions()) {
        // waits (until 0x140) while a station of the other session is still there
        bool isFound = false;
        transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
        session::StationIdStatusTable* pStatusTable = session::Session::s_pInstance->m_pStationIdStatusTable;
        for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
             pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
            u32 sessionId;
            if (!pStatusTable->IsValid(pNode->m_Value.m_StationId) && pStatusTable->GetSessionId(pNode->m_Value.m_StationId, &sessionId) &&
                pSession->m_SessionIds[pSession->m_CurrentIndex] != sessionId) {
                isFound = true;
                break;
            }
        }
        if (!(m_Time0x140 < common::Scheduler::s_pInstance->m_DispatchTime) && isFound) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
        }
    }
    if (stationNum > 1) {
        StationId localStationId = pSession->m_LocalStationId;
        nn::Result result = pSession->m_pSessionProtocol->SendStationList10(m_Phase, stationIds, stationNum, &localStationId, nullptr, 0);
        if (result != common::RESULT_BUFFER_IS_FULL) {
            if (result == common::RESULT_NOT_FOUND) {
                u32 reachableNum = 0;
                for (u32 i = 0; i < stationNum; i++) {
                    if (stationIds[i] != pSession->m_LocalStationId && pSession->m_pStationIdStatusTable->IsValid(stationIds[i])) {
                        reachableNum++;
                    }
                }
                if (reachableNum != 0) {
                    m_Unknown0xC0 = 10;
                    m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else if (result.IsFailure()) {
                m_Unknown0xC0 = 3;
                m_Result = result;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    session::Session::s_pInstance->m_Unknown0xB1 = 1;
    pSession->m_pStationIdStatusTable->ClearUnknown0x1();
    pSession->m_pStationIdStatusTable->SetUnknown0x1(m_StationId, true);
    pSession->m_pStationIdStatusTable->SetUnknown0x12(m_StationId, true);
    m_MessageType = 11;
    m_Time0x140 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitUntilCompanionLeavesMesh, "NexJointSessionJob::WaitUntilCompanionLeavesMesh");
    return common::ExecuteResult(CONTINUE);
}

// 0x003EEE18
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartLeavePreviousMesh()
{
    m_JobPhase = 14;
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session::s_pInstance->m_Unknown0xB1 = 1;
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->CheckJoined() == common::RESULT_NOT_JOINED) {
        if (m_Phase == 9 || m_Phase == 10) {
            SetStep(&NexJointSessionJob::StartLeavePreviousMatchmakeSession, "NexJointSessionJob::StartLeavePreviousMatchmakeSession");
        } else {
            m_MeshRestartDeadline = GetTimeAfter(MESH_RESTART_DELAY_MSEC);
            SetStep(&NexJointSessionJob::WaitMeshRestart, "NexJointSessionJob::WaitMeshRestart");
        }
        return common::ExecuteResult(CONTINUE);
    }
    nn::Result result;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        if (IsLeaderPhase(m_Phase) || m_Phase == 9 || !session::Session::s_pInstance->m_IsHostMigrationEnabled) {
            result = pMesh->DestroyMeshAsync();
            m_Unknown0x59 = 3;
        } else {
            result = pMesh->LeaveMeshWithHostMigrationAsync();
            m_Unknown0x59 = 2;
        }
    } else {
        result = pMesh->LeaveMeshAsync();
        m_Unknown0x59 = 1;
    }
    if (result.IsFailure()) {
        if (m_Phase == 9 || m_Phase == 10) {
            SetStep(&NexJointSessionJob::StartLeavePreviousMatchmakeSession, "NexJointSessionJob::StartLeavePreviousMatchmakeSession");
        } else {
            m_MeshRestartDeadline = GetTimeAfter(MESH_RESTART_DELAY_MSEC);
            SetStep(&NexJointSessionJob::WaitMeshRestart, "NexJointSessionJob::WaitMeshRestart");
        }
        return common::ExecuteResult(CONTINUE);
    }
    SetStep(&NexJointSessionJob::WaitLeavePreviousMesh, "NexJointSessionJob::WaitLeavePreviousMesh");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EF130
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitStartRetryJoinMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (!IsLeaderPhase(m_Phase)) {
        if (!(m_RetryDeadline < common::Scheduler::s_pInstance->m_DispatchTime) && !m_Unknown0x188) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_Unknown0x188) {
            m_Unknown0x1A8 = 0;
        }
        Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x251);
        SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
        return common::ExecuteResult(CONTINUE);
    }
    if (!(m_RetryDeadline < common::Scheduler::s_pInstance->m_DispatchTime) && !m_Unknown0x190) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_Unknown0x190) {
        m_Unknown0x1A8 = 0;
    }
    Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x251);
    bool isOwner = GetCurrentMatchmakeSession()->vf_0x88();
    SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
    if (isOwner) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003EF430
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::CallDestroySessionEvent()
{
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure()) {
        if (common::IsValidPointer(m_pCallContext)) {
            m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            m_pCallContext = nullptr;
        }
    } else if (IsSessionDisconnected()) {
        if (common::IsValidPointer(m_pCallContext)) {
            m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
            m_pCallContext = nullptr;
        }
    } else {
        BeginJointSession();
        session::Session::s_pInstance->m_Unknown0xB2 = true;
        session::Mesh::s_pInstance->m_IsMonitoringDataSent = true;
        m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
        m_Time0x138 = common::Scheduler::s_pInstance->m_DispatchTime;
        if (IsSessionDisconnected()) {
            m_Unknown0x9D = 1;
            m_Unknown0xC0 = 3;
            m_Result = common::RESULT_NOT_IN_SESSION;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
            return common::ExecuteResult(CONTINUE);
        }
        if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            m_Unknown0xC0 = 4;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_IsFailed) {
            m_Result = m_FailureResult;
            m_Unknown0xC0 = 6;
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (m_Unknown0x1AC) {
            m_Unknown0xC0 = 15;
            m_Result = common::RESULT_INVALID_STATE;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        SetStep(&NexJointSessionJob::CloseMatchmakeSession, "NexJointSessionJob::CloseMatchmakeSession");
        return common::ExecuteResult(CONTINUE);
    }
    m_Result = nn::Result();
    m_JobPhase = 22;
    return common::ExecuteResult(SUCCESS);
}

// 0x003EF74C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::ResendDestroyInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    // all reachable stations, and those of them that did not answer yet
    StationId stationIds[11];
    StationId targets[11];
    u32 stationNum = 0;
    u32 targetNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        StationIndex stationIndex = STATION_INDEX_UNIDENTIFIED;
        if (pSession->m_pStationIdStatusTable->GetStationIndex(pNode->m_Value, &stationIndex) &&
            session::Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
            if (!(pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value) | pSession->m_pStationIdStatusTable->GetUnknown0x12(pNode->m_Value))) {
                targets[targetNum] = pNode->m_Value;
                targetNum++;
            }
        }
    }
    if (targetNum != 0) {
        StationId localStationId = pSession->m_LocalStationId;
        nn::Result result = pSession->m_pSessionProtocol->SendStationList8(m_Phase, stationIds, stationNum, &localStationId, targets, targetNum);
        if (result != common::RESULT_BUFFER_IS_FULL && result.IsFailure()) {
            if (result == common::RESULT_NOT_FOUND) {
                m_Unknown0xC0 = 7;
                m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
            } else {
                m_Unknown0xC0 = 3;
                m_Result = result;
            }
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    m_Time0x118 = common::Scheduler::s_pInstance->m_DispatchTime;
    SetStep(&NexJointSessionJob::WaitForAnswerToDestroyInvitation, "NexJointSessionJob::WaitForAnswerToDestroyInvitation");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003EFC14 (name is ours)
void nn::pia::inet::NexJointSessionJob::SetupStationIdList()
{
    session::Session* pSession = session::Session::s_pInstance;
    m_StationIdList.ClearNodes();
    session::StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        u32 sessionId;
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId) && pStatusTable->GetSessionId(pNode->m_Value.m_StationId, &sessionId) &&
            pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    m_Unknown0x98 = 1;
    m_StationId = pSession->m_LocalStationId;
    m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    pSession->m_pStationIdStatusTable->ClearUnknown0x1();
}

// 0x003EFD9C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendCompletionInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
        }
    }
    StationId localStationId = pSession->m_LocalStationId;
    nn::Result result = pSession->m_pSessionProtocol->SendStationList20(m_Phase, stationIds, stationNum, &localStationId, nullptr, 0);
    if (result.IsFailure() && result == common::RESULT_NOT_IN_SESSION) {
        m_Unknown0xC0 = 3;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_MessageType = 21;
    pSession->m_pStationIdStatusTable->ClearUnknown0x1();
    localStationId = pSession->m_LocalStationId;
    pSession->m_pStationIdStatusTable->SetUnknown0x1(localStationId, true);
    m_JobPhase = 21;
    m_Time0x150 = GetTimeAfter(PREPARED_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitForAnswerToCompletionInvitation, "NexJointSessionJob::WaitForAnswerToCompletionInvitation");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F01A8
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCompletionInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pStationIdStatusTable->IsValid(m_StationId)) {
        if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() == m_StationId.m_Low) {
            if (m_Unknown0x9C) {
                SetStep(&NexJointSessionJob::WaitHostStationId, "NexJointSessionJob::WaitHostStationId");
                return common::ExecuteResult(CONTINUE);
            }
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
            m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
        } else {
            m_RestartTime = GetTimeBefore(COMPANION_RESTART_OFFSET_MSEC);
        }
        SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
        m_JobPhase = 17;
        return common::ExecuteResult(CONTINUE);
    }
    if (!pSession->m_IsHostMigrationEnabled) {
        m_Unknown0xC0 = 5;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->CheckJoined() == common::RESULT_NOT_JOINED) {
        m_Unknown0xC0 = 4;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (!m_Unknown0x188) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_Unknown0x188 = 0;
    if (pMesh->m_pProcessHostMigrationJob->m_IsRunning) {
        m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    } else {
        m_RestartTime = GetTimeBefore(COMPANION_RESTART_OFFSET_MSEC);
    }
    SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
    m_JobPhase = 17;
    return common::ExecuteResult(CONTINUE);
}

// 0x003F0654
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitJoinMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    u32 jointSessionId;
    if (!static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->IsJoinWithParticipantsCompleted(&jointSessionId, nullptr, nullptr)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 19;
        m_Result = m_CallContext.m_Result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (session::Session::s_pInstance->m_pMeshLayerController->vf_0x34() && jointSessionId != 0) {
        m_Unknown0xC0 = 17;
        m_Result = common::RESULT_INVALID_STATE;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = m_JointSessionId;
    common::g_SessionBeginMonitoringContent.m_Unknown0x170 = m_JointSessionId;
    m_Time0x170 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitNotification, "NexJointSessionJob::WaitNotification");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F08EC (name is ours)
void nn::pia::inet::NexJointSessionJob::UpdateUnknown0x1AD()
{
    session::Session* pSession = session::Session::s_pInstance;
    u32 sessionId;
    if (IsLeaderPhase(m_Phase)) {
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    } else {
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    }
    typedef NexMatchMeshLayerController::SessionEntry SessionEntry;
    SessionEntry* pEntry = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController)->FindSessionEntry(sessionId);
    bool isMissing = false;
    if (pEntry == nullptr) {
        isMissing = true;
    } else {
        for (SessionEntry::List::Node* pParticipant = pEntry->m_List.Begin(); pParticipant != pEntry->m_List.End();
             pParticipant = SessionEntry::List::Advance(pParticipant)) {
            bool isFound = false;
            for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
                if (pParticipant->m_Value.m_PrincipalId == pNode->m_Value.m_Low && pParticipant->m_Value.m_Count > 0) {
                    isFound = true;
                    break;
                }
            }
            if (!isFound) {
                isMissing = true;
            }
        }
    }
    m_Unknown0x1AD = !isMissing;
}

// 0x003F09CC
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x3C(u8 /*phase*/, u32 /*sessionId*/)
{
    return nn::Result();
}

// 0x003F09D4
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendInvitationAsCompanion()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (m_Time0x158 < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Unknown0xC0 = 32;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
        }
    }
    if (stationNum > 1) {
        StationId localStationId = pSession->m_LocalStationId;
        nn::Result result = pSession->m_pSessionProtocol->SendStationList6(m_Phase, stationIds, stationNum, &localStationId, nullptr, 0);
        if (result != common::RESULT_BUFFER_IS_FULL) {
            if (result == common::RESULT_NOT_FOUND) {
                u32 reachableNum = 0;
                for (u32 i = 0; i < stationNum; i++) {
                    if (stationIds[i] != pSession->m_LocalStationId && pSession->m_pStationIdStatusTable->IsValid(stationIds[i])) {
                        reachableNum++;
                    }
                }
                if (reachableNum != 0) {
                    m_Unknown0xC0 = 8;
                    m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
            } else if (result.IsFailure()) {
                m_Unknown0xC0 = 3;
                m_Result = result;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    m_MessageType = 7;
    pSession->m_pStationIdStatusTable->ClearUnknown0x1();
    pSession->m_pStationIdStatusTable->ClearUnknown0x12();
    pSession->m_pStationIdStatusTable->SetUnknown0x1(m_StationId, true);
    pSession->m_pStationIdStatusTable->SetUnknown0x12(m_StationId, true);
    m_Time0x118 = common::Scheduler::s_pInstance->m_DispatchTime;
    m_Time0x150 = GetTimeAfter(PREPARED_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitForAnswerToInvitation, "NexJointSessionJob::WaitForAnswerToInvitation");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F0F40
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartJoinMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    // the other stations of the joint session join the session with the local one
    u32 principalIds[11] = {};
    u32 principalNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pNode->m_Value != m_StationId && pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            principalIds[principalNum] = pNode->m_Value.m_Low;
            principalNum++;
        }
    }
    pSession = session::Session::s_pInstance;
    u32 currentSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    nn::Result result = pMatchmakeSession->JoinWithParticipantsAsync(&m_CallContext, m_JointSessionId, principalIds, principalNum, currentSessionId,
                                                                     pSession->m_pMeshLayerController->vf_0x34());
    if (result.IsFailure()) {
        m_Unknown0xC0 = 19;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitJoinMatchmakeSession, "NexJointSessionJob::WaitJoinMatchmakeSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F1354
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCloseMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsCloseParticipationCompleted()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 1;
        if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
        } else {
            m_Result = m_CallContext.m_Result;
        }
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session::s_pInstance->SetJoinable(pSession->m_SessionIds[pSession->m_CurrentIndex], false);
    if ((m_Phase == 9 || m_Phase == 10) && pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x88()) {
        SetStep(&NexJointSessionJob::CloseJointSessionParticipation, "NexJointSessionJob::CloseJointSessionParticipation");
    } else {
        m_JobPhase = 5;
        m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
        SetStep(&NexJointSessionJob::SendInvitationAsCompanion, "NexJointSessionJob::SendInvitationAsCompanion");
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003F1650
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitForAnswerToInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    // after the timeout the stations that did not answer are not waited for
    bool isTimeout = m_Time0x150 < common::Scheduler::s_pInstance->m_DispatchTime;
    bool isAnswered = true;
    session::Session* pSession = session::Session::s_pInstance;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        u8 answer = pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value);
        if (answer == 0) {
            if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value) && !isTimeout) {
                isAnswered = false;
            }
        } else if (answer == 2) {
            // refused
            m_Unknown0xC0 = 12;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    if (!isAnswered) {
        if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x118).GetTick() >=
            common::TimeSpan::GetTicksPerMSec().GetTick() * RESEND_INTERVAL_MSEC) {
            SetStep(&NexJointSessionJob::ResendInvitationAsCompanion, "NexJointSessionJob::ResendInvitationAsCompanion");
            return common::ExecuteResult(CONTINUE);
        }
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_MessageType = 0;
    m_Time0xB8 = common::Scheduler::s_pInstance->m_DispatchTime;
    switch (m_Phase) {
    case 6:
        m_JobPhase = 7;
        SetStep(&NexJointSessionJob::StartCreateMatchmakeSession, "NexJointSessionJob::StartCreateMatchmakeSession");
        break;
    case 7:
        m_JobPhase = 7;
        SetStep(&NexJointSessionJob::StartJoinMatchmakeSession, "NexJointSessionJob::StartJoinMatchmakeSession");
        break;
    case 8:
        m_JobPhase = 7;
        SetStep(&NexJointSessionJob::StartRandomMatchmake, "NexJointSessionJob::StartRandomMatchmake");
        break;
    case 9:
    case 10:
        m_JobPhase = 11;
        m_Time0x140 = GetTimeAfter(LEAVE_MESH_COMPANION_TIMEOUT_MSEC);
        SetStep(&NexJointSessionJob::SendLeaveMeshCompanion, "NexJointSessionJob::SendLeaveMeshCompanion");
        break;
    default:
        m_Unknown0xC0 = 31;
        m_Result = common::RESULT_INVALID_STATE;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003F1C38
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitUntilLeaderLeavesMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (!session::Session::s_pInstance->m_pStationIdStatusTable->IsValid(m_StationId)) {
        SetStep(&NexJointSessionJob::StartLeavePreviousMesh, "NexJointSessionJob::StartLeavePreviousMesh");
        return common::ExecuteResult(CONTINUE);
    }
    if ((m_Phase == 9 || m_Phase == 10) && m_Unknown0x9B) {
        SetStep(&NexJointSessionJob::StartLeavePreviousMesh, "NexJointSessionJob::StartLeavePreviousMesh");
        return common::ExecuteResult(CONTINUE);
    }
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F1E18
void nn::pia::inet::NexJointSessionJob::vf_0x4C(const nn::pia::StationId& stationId)
{
    m_Unknown0x17D = 1;
    m_StationId0x180 = stationId;
    if (!session::Session::s_pInstance->m_IsHostMigrationEnabled) {
        m_IsFailed = true;
        m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
    }
}

// 0x003F1E5C
void nn::pia::inet::NexJointSessionJob::vf_0x5C(u32 sessionId)
{
    session::Session* pSession = session::Session::s_pInstance;
    if ((IsLeaderPhase(m_Phase) && pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == sessionId) ||
        pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
        m_IsFailed = true;
        m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
    }
}

// 0x003F1ECC
void nn::pia::inet::NexJointSessionJob::vf_0x54(u32 sessionId, u32 value)
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
        m_Unknown0x19C = value;
        m_Unknown0x198 = 1;
    }
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == sessionId) {
        m_Unknown0x1A4 = value;
        m_Unknown0x1A0 = 1;
    }
}

// 0x003F1F24
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x38(u8 phase, u8 messageType, const nn::pia::StationId* pStationIds, u32 stationNum,
                                                      const nn::pia::StationId& stationId)
{
    session::Session* pSession = session::Session::s_pInstance;
    bool isFound = false;
    bool isLocalFound = false;
    StationId localStationId = pSession->m_LocalStationId;
    for (u32 i = 0; i < stationNum; i++) {
        if (pStationIds[i] == stationId) {
            isFound = true;
        }
        if (pStationIds[i] == localStationId) {
            isLocalFound = true;
        }
    }
    if (!isFound || !isLocalFound) {
        return common::RESULT_INVALID_STATE;
    }
    m_StationIdList.Clear();
    m_JointSessionId = 0;
    if (phase == 9) {
        if (pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
            return common::RESULT_INVALID_STATE;
        }
        m_Unknown0x1AC = 0;
        NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
        if (pController != nullptr) {
            // only the other session may have an entry in the notifications
            u8 sessionNum = 0;
            for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
                if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]) {
                    sessionNum++;
                    break;
                }
            }
            if (sessionNum != 1) {
                m_Unknown0x1AC = 1;
            }
        }
        if (messageType != 8) {
            nn::Result result = vf_0x40(phase, pStationIds, stationNum, stationId);
            if (result.IsFailure()) {
                return result;
            }
        } else {
            m_JobPhase = 3;
            session::StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
            transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
            for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
                 pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
                u32 sessionId;
                if (pStatusTable->IsValid(pNode->m_Value.m_StationId) && pStatusTable->GetSessionId(pNode->m_Value.m_StationId, &sessionId) &&
                    pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                    StationId* pStationId = m_StationIdList.PushBackNew();
                    if (pStationId != nullptr) {
                        *pStationId = pNode->m_Value.m_StationId;
                    }
                }
            }
        }
    } else if (phase == 10) {
        if (pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
            return common::RESULT_INVALID_STATE;
        }
        m_Unknown0x1AC = 0;
        NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
        if (pController != nullptr) {
            u8 sessionNum = 0;
            for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
                if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]) {
                    sessionNum++;
                    break;
                }
            }
            if (sessionNum != 1) {
                m_Unknown0x1AC = 1;
            }
        }
        m_JobPhase = 6;
        m_MessageType = 6;
        for (u32 i = 0; i < stationNum; i++) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pStationIds[i];
            }
        }
    } else {
        if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == 0) {
            return common::RESULT_INVALID_STATE;
        }
        m_Unknown0x1AC = 0;
        NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
        if (pController != nullptr) {
            // only the current session may have an entry in the notifications
            for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
                if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex]) {
                    m_Unknown0x1AC = 1;
                }
            }
        }
        m_JobPhase = 6;
        m_MessageType = 6;
        for (u32 i = 0; i < stationNum; i++) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pStationIds[i];
            }
        }
    }
    ResetMonitoringContent();
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallSessionEvent, "NexJointSessionJob::CallSessionEvent");
    return nn::Result();
}

// 0x003F252C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCreateMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    u32 sessionId = 0;
    if (!static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->IsCreateWithParticipantsCompleted(&sessionId, nullptr, nullptr)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 18;
        m_Result = m_CallContext.m_Result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_JointSessionId = sessionId;
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
    common::g_SessionBeginMonitoringContent.m_Unknown0x170 = m_JointSessionId;
    m_Time0x170 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::WaitNotification, "NexJointSessionJob::WaitNotification");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F2770
void nn::pia::inet::NexJointSessionJob::vf_0x50(u32 sessionId, u32 principalId)
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
        m_Unknown0x18C = principalId;
        m_Unknown0x188 = 1;
    }
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == sessionId) {
        m_Unknown0x194 = principalId;
        m_Unknown0x190 = 1;
    }
    if (!pSession->m_IsHostMigrationEnabled) {
        m_IsFailed = true;
        m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
    }
}

// 0x003F27E8
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x40(u8 /*phase*/, const nn::pia::StationId* pStationIds, u32 stationNum, const nn::pia::StationId& stationId)
{
    m_JobPhase = 6;
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_StationIdList.ClearNodes();
    bool isFound = false;
    bool isLocalFound = false;
    StationId localStationId = pSession->m_LocalStationId;
    for (u32 i = 0; i < stationNum; i++) {
        StationId* pStationId = m_StationIdList.PushBackNew();
        if (pStationId != nullptr) {
            *pStationId = pStationIds[i];
            if (*pStationId == stationId) {
                isFound = true;
            }
            if (*pStationId == localStationId) {
                isLocalFound = true;
            }
        }
    }
    if (isFound && isLocalFound) {
        return nn::Result();
    }
    return common::RESULT_INVALID_STATE;
}

// 0x003F2970
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::GetNextMatchmakeSessionInfo()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    nn::Result result = static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->GetSessionStatusAsync(&m_CallContext, m_JointSessionId);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 22;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitGetNextMatchmakeSessionInfo, "NexJointSessionJob::WaitGetNextMatchmakeSessionInfo");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F2BDC
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::ResendInvitationAsCompanion()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    // all reachable stations, and those of them that did not answer yet
    StationId stationIds[11];
    StationId targets[11];
    u32 stationNum = 0;
    u32 targetNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
            if (!(pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value) | pSession->m_pStationIdStatusTable->GetUnknown0x12(pNode->m_Value))) {
                targets[targetNum] = pNode->m_Value;
                targetNum++;
            }
        }
    }
    if (targetNum != 0) {
        StationId localStationId = pSession->m_LocalStationId;
        nn::Result result = pSession->m_pSessionProtocol->SendStationList6(m_Phase, stationIds, stationNum, &localStationId, targets, targetNum);
        if (result != common::RESULT_BUFFER_IS_FULL && result.IsFailure()) {
            if (result == common::RESULT_NOT_FOUND) {
                m_Unknown0xC0 = 8;
                m_Result = common::RESULT_SESSION_OWNER_LEFT;
            } else {
                m_Unknown0xC0 = 3;
                m_Result = result;
            }
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    m_Time0x118 = common::Scheduler::s_pInstance->m_DispatchTime;
    SetStep(&NexJointSessionJob::WaitForAnswerToInvitation, "NexJointSessionJob::WaitForAnswerToInvitation");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F3074
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartCreateMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    // the session is created for the local station and the other stations of the joint session
    u32 principalIds[11] = {};
    u32 principalNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pNode->m_Value != m_StationId && pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            principalIds[principalNum] = pNode->m_Value.m_Low;
            principalNum++;
        }
    }
    pSession = session::Session::s_pInstance;
    u32 currentSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    nn::Result result = pMatchmakeSession->CreateWithParticipantsAsync(&m_CallContext, principalIds, principalNum, currentSessionId,
                                                                       pSession->m_pMeshLayerController->vf_0x34());
    if (result.IsFailure()) {
        m_Unknown0xC0 = 18;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitCreateMatchmakeSession, "NexJointSessionJob::WaitCreateMatchmakeSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F3468
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x2C(const nn::pia::session::JoinSessionSetting* pSetting)
{
    m_JobPhase = 1;
    const NexJoinSessionSetting* pJoinSetting = static_cast<const NexJoinSessionSetting*>(pSetting);
    if (pJoinSetting->GetSessionId() == 0 || pJoinSetting->vf_0x20() == 0 ||
        pJoinSetting->vf_0x20() > transport::Transport::s_pInstance->m_StationNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x1AC = 0;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        // only the current session may have an entry in the notifications
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex]) {
                m_Unknown0x1AC = 1;
            }
        }
    }
    m_JointSessionId = pJoinSetting->GetSessionId();
    session::Session* pCurrentSession = session::Session::s_pInstance;
    pCurrentSession->m_StationIdEntryNumMax[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = pJoinSetting->vf_0x20();
    pMatchmakeSession->Cleanup();
    ResetMonitoringContent();
    common::g_SessionBeginMonitoringContent.m_Unknown0x22C = pJoinSetting->vf_0x20();
    static_cast<NexMatchmakeSession*>(pMatchmakeSession)->SetJoinSetting(pJoinSetting);
    // the stations of the current session form the joint session
    m_StationIdList.Clear();
    session::StationIdStatusTable* pStatusTable = session::Session::s_pInstance->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId)) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    m_MessageType = 0;
    m_JobPhase = 5;
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallSessionEvent, "NexJointSessionJob::CallSessionEvent");
    return nn::Result();
}

// 0x003F37B0
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCompanionStationPrepared()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (m_Unknown0x98 && m_Unknown0x17D && m_StationId0x180 != pSession->m_LocalStationId) {
        // the host changed: the new host works with the companions
        m_Unknown0x17D = 0;
        m_Unknown0x98 = 0;
        m_StationId = m_StationId0x180;
        SetStep(&NexJointSessionJob::SendPreparedForMigrateSession, "NexJointSessionJob::SendPreparedForMigrateSession");
        return common::ExecuteResult(CONTINUE);
    }
    bool isTimeout = m_Time0x150 < common::Scheduler::s_pInstance->m_DispatchTime;
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value) && pSession->m_pStationIdStatusTable->GetStatus(pNode->m_Value) != 1) {
            stationIds[stationNum] = pNode->m_Value;
            stationNum++;
        }
    }
    if (pSession->m_pStationIdStatusTable->CheckStatus(stationIds, stationNum) == 1 && !isTimeout) {
        if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x120).GetTick() >=
            common::TimeSpan::GetTicksPerMSec().GetTick() * RESEND_INTERVAL_MSEC) {
            SetStep(&NexJointSessionJob::ResendNextSessionId, "NexJointSessionJob::ResendNextSessionId");
            return common::ExecuteResult(CONTINUE);
        }
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::SendLeaveMeshCompanion, "NexJointSessionJob::SendLeaveMeshCompanion");
    m_Time0x140 = GetTimeAfter(LEAVE_MESH_COMPANION_TIMEOUT_MSEC);
    m_JobPhase = 11;
    return common::ExecuteResult(CONTINUE);
}

// 0x003F3C98
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitForInvitationAsCompanion()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x130).GetTick() >=
        common::TimeSpan::GetTicksPerMSec().GetTick() * PREPARED_TIMEOUT_MSEC) {
        m_Unknown0xC0 = 14;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_Unknown0x17D) {
        // the host changed: the local station invites the companions itself if it is the new host
        // or the owner of the current session
        m_Unknown0x17D = 0;
        session::Session* pSession = session::Session::s_pInstance;
        if (m_StationId0x180 == pSession->m_LocalStationId) {
            SetupStationIdList();
            m_JobPhase = 5;
            m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
            SetStep(&NexJointSessionJob::SendInvitationAsCompanion, "NexJointSessionJob::SendInvitationAsCompanion");
            return common::ExecuteResult(CONTINUE);
        }
        u32 sessionId;
        if (pSession->m_pStationIdStatusTable->GetSessionId(m_StationId0x180, &sessionId)) {
            session::Session* pCurrentSession = session::Session::s_pInstance;
            if (pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex] != sessionId &&
                pCurrentSession->m_pMatchmakeSessions[pCurrentSession->m_CurrentIndex]->vf_0x88()) {
                SetupStationIdList();
                m_JobPhase = 5;
                m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
                SetStep(&NexJointSessionJob::SendInvitationAsCompanion, "NexJointSessionJob::SendInvitationAsCompanion");
                return common::ExecuteResult(CONTINUE);
            }
        }
    }
    if (m_StationId == GetStationIdOfIndex253()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_MessageType = 6;
    SetStep(&NexJointSessionJob::SendAnswerToInvitation, "NexJointSessionJob::SendAnswerToInvitation");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F40F4
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitUntilCompanionCompletion()
{
    session::Session* pSession = session::Session::s_pInstance;
    if ((common::Scheduler::s_pInstance->m_DispatchTime - m_RestartTime).GetTick() >=
        common::TimeSpan::GetTicksPerMSec().GetTick() * COMPANION_TIMEOUT_MSEC) {
        m_Unknown0xC0 = 35;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    StationId stationIds[11];
    u32 stationNum = 0;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        stationIds[stationNum] = pNode->m_Value;
        stationNum++;
    }
    if (!pSession->m_pStationIdStatusTable->AreValid(stationIds, stationNum)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (!pSession->m_pStationIdStatusTable->GetUnknown0x13(pNode->m_Value)) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    SetStep(&NexJointSessionJob::SendCompletionInvitation, "NexJointSessionJob::SendCompletionInvitation");
    m_JobPhase = 20;
    return common::ExecuteResult(CONTINUE);
}

// 0x003F4328
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitUntilCompanionLeavesMesh()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    // until the timeout it waits for the reachable companions that did not answer
    if (common::Scheduler::s_pInstance->m_DispatchTime < m_Time0x140) {
        for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
            if (pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value) == 0 && pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    SetStep(&NexJointSessionJob::StartLeavePreviousMesh, "NexJointSessionJob::StartLeavePreviousMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, LEAVE_MESH_WAIT_MSEC);
}

// 0x003F4588
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x30()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x1AC = 0;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        // only the other session may have an entry in the notifications
        u8 sessionNum = 0;
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]) {
                sessionNum++;
                break;
            }
        }
        if (sessionNum != 1) {
            m_Unknown0x1AC = 1;
        }
    }
    // the stations of the current session
    m_StationIdList.Clear();
    session::StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        u32 sessionId;
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId) && pStatusTable->GetSessionId(pNode->m_Value.m_StationId, &sessionId) &&
            pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    ResetMonitoringContent();
    m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    m_MessageType = 0;
    m_JobPhase = 5;
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallSessionEvent, "NexJointSessionJob::CallSessionEvent");
    return nn::Result();
}

// 0x003F4828
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendAnswerToDestroyInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    StationId hostStationId = pSession->GetJointHostStationId();
    nn::Result result = pSession->m_pSessionProtocol->SendMessage9(m_Phase, hostStationId, 1);
    if (result == common::RESULT_BUFFER_IS_FULL) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (result.IsFailure()) {
        m_Unknown0xC0 = 5;
        m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
        SetupStationIdList();
        SetStep(&NexJointSessionJob::CloseMatchmakeSession, "NexJointSessionJob::CloseMatchmakeSession");
        return common::ExecuteResult(CONTINUE);
    }
    m_Time0x130 = common::Scheduler::s_pInstance->m_DispatchTime;
    m_MessageType = 6;
    SetStep(&NexJointSessionJob::WaitForInvitationAsCompanion, "NexJointSessionJob::WaitForInvitationAsCompanion");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F4B68
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendPreparedForMigrateSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pStationIdStatusTable->IsValid(m_StationId)) {
        nn::Result result = pSession->m_pSessionProtocol->SendMessage4(m_Phase, m_StationId, m_JointSessionId, 1);
        if (result == common::RESULT_BUFFER_IS_FULL) {
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (result.IsFailure()) {
            m_Unknown0xC0 = 5;
            m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        StationId localStationId = pSession->m_LocalStationId;
        pSession->m_pStationIdStatusTable->SetStatus(localStationId, true);
        pSession->m_pStationIdStatusTable->SetStatus(m_StationId, true);
        session::Session::s_pInstance->m_Unknown0xB1 = 1;
        m_MessageType = 10;
        m_Unknown0x9B = 0;
        SetStep(&NexJointSessionJob::WaitUntilLeaderLeavesMesh, "NexJointSessionJob::WaitUntilLeaderLeavesMesh");
        m_JobPhase = 12;
        return common::ExecuteResult(CONTINUE);
    }
    if (!pSession->m_IsHostMigrationEnabled) {
        m_Unknown0xC0 = 5;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (session::Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        m_Unknown0xC0 = 4;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_Unknown0x17D) {
        // the host changed: the new host works as the leader
        m_Unknown0x17D = 0;
        if (m_StationId0x180 == pSession->m_LocalStationId) {
            SetupStationIdList();
            m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
            SetStep(&NexJointSessionJob::SendNextSessionId, "NexJointSessionJob::SendNextSessionId");
        } else {
            m_StationId = m_StationId0x180;
            m_JointSessionId = 0;
            session::Session* pCurrentSession = session::Session::s_pInstance;
            pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
            m_MessageType = 3;
            SetStep(&NexJointSessionJob::WaitNextSessionId, "NexJointSessionJob::WaitNextSessionId");
            m_JobPhase = 9;
        }
    }
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F5050
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x48(u8 /*phase*/, const nn::pia::StationId& /*stationId*/)
{
    m_Unknown0x9C = 1;
    return nn::Result();
}

// 0x003F5060
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x28(const nn::pia::session::CreateSessionSetting* pSetting)
{
    m_JobPhase = 1;
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x1AC = 0;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        // only the current session may have an entry in the notifications
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex]) {
                m_Unknown0x1AC = 1;
            }
        }
    }
    ResetMonitoringContent();
    const NexCreateSessionSetting* pCreateSetting = static_cast<const NexCreateSessionSetting*>(pSetting);
    if (pCreateSetting->m_Unknown0x6 == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u16 stationNum = pCreateSetting->GetUnknown0x474() + pCreateSetting->m_Unknown0x6;
    if (stationNum > transport::Transport::s_pInstance->m_StationNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    static_cast<NexMatchmakeSession*>(pMatchmakeSession)->SetCreateSetting(pCreateSetting, pSession->m_IsHostMigrationEnabled);
    if (pSession->IsUsingStationIdTable()) {
        session::Session* pCurrentSession = session::Session::s_pInstance;
        pCurrentSession->m_StationIdEntryNumMax[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = stationNum;
    }
    // the stations of the current session form the joint session
    m_StationIdList.Clear();
    session::StationIdStatusTable* pStatusTable = session::Session::s_pInstance->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId)) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    m_JointSessionId = 0;
    m_MessageType = 0;
    m_JobPhase = 5;
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallSessionEvent, "NexJointSessionJob::CallSessionEvent");
    return nn::Result();
}

// 0x003F5350
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::CloseJointSessionParticipation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->CloseParticipationAsync(
        &m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 2;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitCloseJointSessionParticipation, "NexJointSessionJob::WaitCloseJointSessionParticipation");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F55D0
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x44(u8 /*phase*/, const nn::pia::StationId& /*stationId*/)
{
    m_JobPhase = 13;
    m_Unknown0x9B = 1;
    return nn::Result();
}

// 0x003F55E8
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x34()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x1AC = 0;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        // only the other session may have an entry in the notifications
        u8 sessionNum = 0;
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0]) {
                sessionNum++;
                break;
            }
        }
        if (sessionNum != 1) {
            m_Unknown0x1AC = 1;
        }
    }
    // the valid stations of the transport
    m_StationIdList.Clear();
    session::StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId)) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    ResetMonitoringContent();
    m_JointSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    m_MessageType = 0;
    m_JobPhase = 2;
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallDestroySessionEvent, "NexJointSessionJob::CallDestroySessionEvent");
    return nn::Result();
}

// 0x003F5858
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::SendCompletionForMigrateSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (m_Time0x160 < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Unknown0xC0 = 34;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0])->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (pSession->m_pStationIdStatusTable->IsValid(m_StationId)) {
        if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90() == m_StationId.m_Low) {
            nn::Result result = pSession->m_pSessionProtocol->SendMessage19(m_Phase, m_StationId, 1);
            if (result == common::RESULT_BUFFER_IS_FULL) {
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            if (result.IsFailure()) {
                m_Unknown0xC0 = 5;
                m_Result = common::RESULT_JOINT_SESSION_STATION_LOST;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            m_MessageType = 20;
            m_Unknown0x9C = 0;
            m_Time0xA8 = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
            m_JobPhase = 19;
            SetStep(&NexJointSessionJob::WaitCompletionInvitation, "NexJointSessionJob::WaitCompletionInvitation");
            return common::ExecuteResult(CONTINUE);
        }
        if (session::Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
            m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
        } else {
            m_RestartTime = GetTimeBefore(COMPANION_RESTART_OFFSET_MSEC);
        }
        SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
        m_JobPhase = 17;
        return common::ExecuteResult(CONTINUE);
    }
    if (!pSession->m_IsHostMigrationEnabled) {
        m_Unknown0xC0 = 5;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->CheckJoined() == common::RESULT_NOT_JOINED) {
        m_Unknown0xC0 = 4;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (!m_Unknown0x188) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_Unknown0x188 = 0;
    if (pMesh->m_pProcessHostMigrationJob->m_IsRunning) {
        m_RestartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    } else {
        m_RestartTime = GetTimeBefore(COMPANION_RESTART_OFFSET_MSEC);
    }
    SetStep(&NexJointSessionJob::WaitCompanionStation, "NexJointSessionJob::WaitCompanionStation");
    m_JobPhase = 17;
    return common::ExecuteResult(CONTINUE);
}

// 0x003F5E24
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitGetNextMatchmakeSessionInfo()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession());
    if (!pMatchmakeSession->IsGetSessionStatusCompleted()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 22;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex == 0 ? 1 : 0] = pMatchmakeSession->m_MaxParticipants;
    SetStep(&NexJointSessionJob::SendPreparedForMigrateSession, "NexJointSessionJob::SendPreparedForMigrateSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F601C
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitLeaveBufferMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    bool isCompleted;
    if (m_IsUnregistering) {
        isCompleted = GetOtherMatchmakeSession()->IsUnregisterCompleted();
    } else {
        isCompleted = GetOtherMatchmakeSession()->IsLeaveCompleted();
    }
    if (!isCompleted) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_FAILURE && m_CallContext.m_Result == common::RESULT_UNREGISTER_FAILED) {
        m_Result = m_CallContext.m_Result;
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
    SetStep(&NexJointSessionJob::StartLeaveBufferMatchmakeSession, "NexJointSessionJob::StartLeaveBufferMatchmakeSession");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F61DC
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartLeaveBufferMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    // the other matchmake session is left (its owner unregisters it without host migration)
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
        session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        m_CallContext.Reset();
        nn::Result result;
        if (!session::Session::s_pInstance->m_IsHostMigrationEnabled && pMatchmakeSession->vf_0x88()) {
            m_IsUnregistering = 1;
            result = pMatchmakeSession->UnregisterAsync(&m_CallContext, sessionId);
        } else {
            m_IsUnregistering = 0;
            result = pMatchmakeSession->LeaveAsync(&m_CallContext, sessionId);
        }
        if (result.IsSuccess()) {
            SetStep(&NexJointSessionJob::WaitLeaveBufferMatchmakeSession, "NexJointSessionJob::WaitLeaveBufferMatchmakeSession");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
    }
    // then the other sessions that the notifications still have
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            u32 sessionId = pController->GetSessionId(i);
            if (sessionId == 0 || pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                continue;
            }
            session::Session* pCurrentSession = session::Session::s_pInstance;
            pCurrentSession->m_SessionIds[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
            session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
            m_CallContext.Reset();
            nn::Result result;
            if (!session::Session::s_pInstance->m_IsHostMigrationEnabled && pMatchmakeSession->vf_0x88()) {
                m_IsUnregistering = 1;
                result = pMatchmakeSession->UnregisterAsync(&m_CallContext, sessionId);
            } else {
                m_IsUnregistering = 0;
                result = pMatchmakeSession->LeaveAsync(&m_CallContext, sessionId);
            }
            if (result.IsSuccess()) {
                SetStep(&NexJointSessionJob::WaitLeaveBufferMatchmakeSession, "NexJointSessionJob::WaitLeaveBufferMatchmakeSession");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
        }
    }
    SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F6504
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitForAnswerToDestroyInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    // after the timeout the stations that did not answer are not waited for
    bool isTimeout = m_Time0x150 < common::Scheduler::s_pInstance->m_DispatchTime;
    bool isAnswered = true;
    session::Session* pSession = session::Session::s_pInstance;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        u8 answer = pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value);
        if (answer == 0) {
            if (pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value) && !isTimeout) {
                isAnswered = false;
            }
        } else if (answer == 2) {
            // refused
            m_Unknown0xC0 = 11;
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
            static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
            UpdateMonitoringPhase();
            SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
    }
    if (!isAnswered) {
        if ((common::Scheduler::s_pInstance->m_DispatchTime - m_Time0x118).GetTick() >=
            common::TimeSpan::GetTicksPerMSec().GetTick() * RESEND_INTERVAL_MSEC) {
            SetStep(&NexJointSessionJob::ResendDestroyInvitation, "NexJointSessionJob::ResendDestroyInvitation");
            return common::ExecuteResult(CONTINUE);
        }
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetupStationIdList();
    m_JobPhase = 5;
    m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
    SetStep(&NexJointSessionJob::SendInvitationAsCompanion, "NexJointSessionJob::SendInvitationAsCompanion");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F6968
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitLeaveCurrentMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    bool isCompleted;
    if (m_IsUnregistering) {
        isCompleted = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsUnregisterCompleted();
    } else {
        isCompleted = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsLeaveCompleted();
    }
    if (!isCompleted) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_FAILURE && m_CallContext.m_Result == common::RESULT_UNREGISTER_FAILED) {
        m_Result = m_CallContext.m_Result;
    }
    pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
    SetStep(&NexJointSessionJob::StartLeaveBufferMatchmakeSession, "NexJointSessionJob::StartLeaveBufferMatchmakeSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F6AF8
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartLeaveCurrentMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    if (sessionId != 0) {
        session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        m_CallContext.Reset();
        nn::Result result;
        if (!session::Session::s_pInstance->m_IsHostMigrationEnabled && pMatchmakeSession->vf_0x88()) {
            m_IsUnregistering = 1;
            result = pMatchmakeSession->UnregisterAsync(&m_CallContext, sessionId);
        } else {
            m_IsUnregistering = 0;
            result = pMatchmakeSession->LeaveAsync(&m_CallContext, sessionId);
        }
        if (result.IsSuccess()) {
            SetStep(&NexJointSessionJob::WaitLeaveCurrentMatchmakeSession, "NexJointSessionJob::WaitLeaveCurrentMatchmakeSession");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
    }
    SetStep(&NexJointSessionJob::StartLeaveBufferMatchmakeSession, "NexJointSessionJob::StartLeaveBufferMatchmakeSession");
    return common::ExecuteResult(NEXT_DISPATCH);
}

// 0x003F6CFC
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitLeavePreviousMatchmakeSession()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    bool isCompleted;
    if (m_IsUnregistering) {
        isCompleted = GetOtherMatchmakeSession()->IsUnregisterCompleted();
    } else {
        isCompleted = GetOtherMatchmakeSession()->IsLeaveCompleted();
    }
    if (!isCompleted) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
    m_MeshRestartDeadline = GetTimeAfter(MESH_RESTART_DELAY_MSEC);
    SetStep(&NexJointSessionJob::WaitMeshRestart, "NexJointSessionJob::WaitMeshRestart");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F6F08
nn::Result nn::pia::inet::NexJointSessionJob::vf_0x24(const nn::pia::session::CreateSessionSetting* pCreateSetting,
                                                      const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum)
{
    m_JobPhase = 1;
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x1AC = 0;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        // only the current session may have an entry in the notifications
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            if (pController->GetSessionId(i) != 0 && pController->GetSessionId(i) != pSession->m_SessionIds[pSession->m_CurrentIndex]) {
                m_Unknown0x1AC = 1;
            }
        }
    }
    ResetMonitoringContent();
    if (!static_cast<NexMatchmakeSession*>(pMatchmakeSession)->SetSearchCriteria(static_cast<const NexSessionSearchCriteria*>(pCriteria), criteriaNum)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const NexCreateSessionSetting* pSetting = static_cast<const NexCreateSessionSetting*>(pCreateSetting);
    if (pSetting->m_Unknown0x6 == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u16 stationNum = pSetting->GetUnknown0x474() + pSetting->m_Unknown0x6;
    if (stationNum > transport::Transport::s_pInstance->m_StationNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    static_cast<NexMatchmakeSession*>(pMatchmakeSession)->SetCreateSetting(pSetting, pSession->m_IsHostMigrationEnabled);
    if (pSession->IsUsingStationIdTable()) {
        session::Session* pCurrentSession = session::Session::s_pInstance;
        pCurrentSession->m_StationIdEntryNumMax[pCurrentSession->m_CurrentIndex == 0 ? 1 : 0] = stationNum;
    }
    // the stations of the current session form the joint session
    m_StationIdList.Clear();
    session::StationIdStatusTable* pStatusTable = session::Session::s_pInstance->m_pStationIdStatusTable;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    for (common::ObjList<transport::StationIdTable::Entry>::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End();
         pNode = common::ObjList<transport::StationIdTable::Entry>::Advance(pNode)) {
        if (pStatusTable->IsValid(pNode->m_Value.m_StationId)) {
            StationId* pStationId = m_StationIdList.PushBackNew();
            if (pStationId != nullptr) {
                *pStationId = pNode->m_Value.m_StationId;
            }
        }
    }
    m_JointSessionId = 0;
    m_MessageType = 0;
    m_JobPhase = 5;
    m_Unknown0x17D = 0;
    m_Unknown0x188 = 0;
    m_Unknown0x190 = 0;
    m_Unknown0x1A8 = 0;
    m_RetryCount = 0;
    SetStep(&NexJointSessionJob::CallSessionEvent, "NexJointSessionJob::CallSessionEvent");
    return nn::Result();
}

// 0x003F7218
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartLeavePreviousMatchmakeSession()
{
    session::Mesh::s_pInstance->Cleanup();
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    nn::Result result;
    if (!pSession->m_IsHostMigrationEnabled && pMatchmakeSession->vf_0x88()) {
        m_IsUnregistering = 1;
        result = pMatchmakeSession->UnregisterAsync(&m_CallContext, sessionId);
    } else {
        m_IsUnregistering = 0;
        result = pMatchmakeSession->LeaveAsync(&m_CallContext, sessionId);
    }
    if (result.IsFailure()) {
        m_Unknown0xC0 = 38;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitLeavePreviousMatchmakeSession, "NexJointSessionJob::WaitLeavePreviousMatchmakeSession");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F7490
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitCloseJointSessionParticipation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsCloseParticipationCompleted()) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        m_Unknown0xC0 = 2;
        if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
            m_Result = common::RESULT_SESSION_OWNER_LEFT;
        } else {
            m_Result = m_CallContext.m_Result;
        }
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session::s_pInstance->SetJoinable(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0], false);
    if (m_Phase == 9) {
        m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
        SetStep(&NexJointSessionJob::SendDestroyInvitation, "NexJointSessionJob::SendDestroyInvitation");
    } else {
        m_JobPhase = 5;
        m_Time0x158 = GetTimeAfter(INVITATION_TIMEOUT_MSEC);
        SetStep(&NexJointSessionJob::SendInvitationAsCompanion, "NexJointSessionJob::SendInvitationAsCompanion");
    }
    return common::ExecuteResult(CONTINUE);
}

// 0x003F7788
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitForAnswerToCompletionInvitation()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (session::Mesh::s_pInstance->CheckJoined().IsFailure() || session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_4) {
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        m_Unknown0xC0 = 4;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    if (m_Time0x150 < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Unknown0xC0 = 56;
        m_Result = common::RESULT_SESSION_OWNER_LEFT;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    bool isAnswered = true;
    for (StationIdList::Node* pNode = m_StationIdList.Begin(); pNode != m_StationIdList.End(); pNode = StationIdList::Advance(pNode)) {
        if (pSession->m_pStationIdStatusTable->GetUnknown0x1(pNode->m_Value) == 0 && pSession->m_pStationIdStatusTable->IsValid(pNode->m_Value)) {
            isAnswered = false;
        }
    }
    if (!isAnswered) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_Unknown0x9C = 1;
    SetStep(&NexJointSessionJob::WaitHostStationId, "NexJointSessionJob::WaitHostStationId");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F7AD4
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::WaitGetNextMeshHostStationConnectionInfo()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (common::IsValidPointer(m_pCallContext) && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        m_JobPhase = 22;
        return common::ExecuteResult(SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    // the leader gets the info from the other session, the others from the current one
    session::CommonMatchmakeSession* pMatchmakeSession;
    if (IsLeaderPhase(m_Phase)) {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    } else {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    }
    if (!pMatchmakeSession->vf_0x54(&m_ConnectionInfo)) {
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        if (m_CallContext.m_Result == common::RESULT_MATCHMAKE_SESSION_GONE) {
            // the session is retried a few times
            m_RetryCount++;
            NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
            if (m_RetryCount > RETRY_COUNT_MAX) {
                m_Result = common::RESULT_SESSION_OWNER_LEFT;
                static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                UpdateMonitoringPhase();
                SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                return common::ExecuteResult(NEXT_DISPATCH);
            }
            if (!IsLeaderPhase(m_Phase)) {
                if (!pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex])) {
                    m_Unknown0xC0 = 24;
                    m_Result = common::RESULT_SESSION_OWNER_LEFT;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
                if (m_Unknown0x188) {
                    m_RetryDeadline = GetTimeAfter(SHORT_RETRY_TIMEOUT_MSEC);
                } else {
                    m_RetryDeadline = GetTimeAfter(RETRY_TIMEOUT_MSEC);
                }
            } else {
                if (!pController->HasSessionEntry(pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0])) {
                    m_Unknown0xC0 = 24;
                    m_Result = common::RESULT_SESSION_OWNER_LEFT;
                    static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
                    UpdateMonitoringPhase();
                    SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
                    return common::ExecuteResult(NEXT_DISPATCH);
                }
                if (m_Unknown0x190) {
                    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
                        SetStep(&NexJointSessionJob::MeshRestart, "NexJointSessionJob::MeshRestart");
                        return common::ExecuteResult(NEXT_DISPATCH);
                    }
                    m_RetryDeadline = GetTimeAfter(SHORT_RETRY_TIMEOUT_MSEC);
                } else {
                    m_RetryDeadline = GetTimeAfter(RETRY_TIMEOUT_MSEC);
                }
            }
            SetStep(&NexJointSessionJob::WaitStartRetryJoinMesh, "NexJointSessionJob::WaitStartRetryJoinMesh");
            return common::ExecuteResult(NEXT_DISPATCH);
        }
        m_Unknown0xC0 = 29;
        m_Result = m_CallContext.m_Result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    m_ConnectionInfo.Trace(CONNECTION_INFO_TRACE_FLAG);
    SetStep(&NexJointSessionJob::StartJoinNextMesh, "NexJointSessionJob::StartJoinNextMesh");
    m_JobPhase = 16;
    return common::ExecuteResult(CONTINUE);
}

// 0x003F80B8
nn::pia::common::ExecuteResult nn::pia::inet::NexJointSessionJob::StartGetNextMeshHostStationConnectionInfo()
{
    if (IsSessionDisconnected()) {
        m_Unknown0x9D = 1;
        m_Unknown0xC0 = 3;
        m_Result = common::RESULT_NOT_IN_SESSION;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::ProcessFailure, "NexJointSessionJob::ProcessFailure");
        return common::ExecuteResult(CONTINUE);
    }
    if (m_IsFailed) {
        m_Result = m_FailureResult;
        m_Unknown0xC0 = 6;
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession;
    if (IsLeaderPhase(m_Phase)) {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    } else {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    }
    transport::StationConnectionInfo connectionInfo;
    m_ConnectionInfo.SetStationConnectionInfo(connectionInfo);
    nn::Result result = pMatchmakeSession->vf_0x50(&m_CallContext, m_JointSessionId);
    if (result.IsFailure()) {
        m_Unknown0xC0 = 29;
        m_Result = result;
        static_cast<NexMatchmakeSession*>(GetOtherMatchmakeSession())->FinishBrowse();
        UpdateMonitoringPhase();
        SetStep(&NexJointSessionJob::StartLeaveMesh, "NexJointSessionJob::StartLeaveMesh");
        return common::ExecuteResult(NEXT_DISPATCH);
    }
    SetStep(&NexJointSessionJob::WaitGetNextMeshHostStationConnectionInfo, "NexJointSessionJob::WaitGetNextMeshHostStationConnectionInfo");
    return common::ExecuteResult(CONTINUE);
}

// 0x003F8334
nn::pia::inet::NexJointSessionJob::NexJointSessionJob() : m_Result(common::RESULT_NOT_SET), m_JobPhase(0)
{
}

// 0x003F8400
// 0x003F83DC (deleting dtor)
nn::pia::inet::NexJointSessionJob::~NexJointSessionJob()
{
    // only the members and the base (in the original too)
}

// 0x0072F154
void nn::pia::inet::NexJointSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
