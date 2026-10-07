#include "nn/pia/inet/inet_NexMatchJoinSessionJob.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexJoinSessionSetting.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_BandwidthCheckerProtocol.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// a counter of the monitoring data (0xFF means "not set", so it wraps to 1)
inline void Increment(u8& count)
{
    count = count == 0xFF ? 1 : count + 1;
}

inline common::Time GetTimeAfter(s64 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

inline NexMatchMeshLayerController* GetController()
{
    return static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController);
}
} // namespace

// 0x0040161C
void nn::pia::inet::NexMatchJoinSessionJob::vf_0x34()
{
    m_HostPrincipalId = 0;
    m_OwnerPrincipalId = 0;
    m_JointOwnerPrincipalId = 0;
    m_JointSessionId = 0;
    m_IsWaitingForOwnerChange = false;
}

// 0x00401638
nn::Result nn::pia::inet::NexMatchJoinSessionJob::vf_0x30(const nn::pia::session::JoinSessionSetting* pSetting)
{
    session::Session* pSession = session::Session::s_pInstance;
    const NexJoinSessionSetting* pNexSetting = static_cast<const NexJoinSessionSetting*>(pSetting);
    if (pNexSetting->GetSessionId() == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex])->SetJoinSetting(pNexSetting);
    m_SessionId = pNexSetting->GetSessionId();
    m_JointSessionId = 0;
    m_IsWaitingForOwnerChange = false;
    // the sessions that the notifications still know are left first
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    m_JoinedSessionNum = 0;
    for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
        if (pController->GetSessionId(i) != 0) {
            m_JoinedSessionIds[m_JoinedSessionNum] = pController->GetSessionId(i);
            m_JoinedSessionNum++;
        }
    }
    SetStep(&NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession, "NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession");
    return nn::Result();
}

// 0x0040176C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::StartNatSession()
{
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nn::Result result = GetController()->StartNatSession();
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchJoinSessionJob::WaitStartNatSession, "NexMatchJoinSessionJob::WaitStartNatSession");
    m_Phase = 3;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004018A4
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::JoinJointSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->JoinAsync(&m_CallContext, m_JointSessionId);
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchJoinSessionJob::WaitJoinJointSession, "NexMatchJoinSessionJob::WaitJoinJointSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004019E0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitNotification()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_NotificationDeadline < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Result = common::RESULT_UNREGISTER_FAILED;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the notification of the own participation
    NexMatchMeshLayerController* pController = GetController();
    if (!pController->HasParticipant(m_SessionId, NexFacade::s_pInstance->m_pNgsBridge->vf_0x0C())) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (session::Session::s_pInstance->IsUsingStationIdTable()) {
        if (session::Session::s_pInstance->m_pMeshLayerController->vf_0x34() && m_JointSessionId != 0) {
            SetStep(&NexMatchJoinSessionJob::JoinJointSession, "NexMatchJoinSessionJob::JoinJointSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_JointSessionId = 0;
    }
    SetStep(&NexMatchJoinSessionJob::GetStationConnectionInfo, "NexMatchJoinSessionJob::GetStationConnectionInfo");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00401C40
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitJoinMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsJoinCompleted(&m_JointSessionId, nullptr, nullptr)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_Result = m_CallContext.m_Result;
            SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex] = m_SessionId;
            SetSessionState(1);
            m_CallContext.Reset();
            m_NotificationDeadline = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
            SetStep(&NexMatchJoinSessionJob::WaitNotification, "NexMatchJoinSessionJob::WaitNotification");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00401E04
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitStartNatSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (IsCancelRequested()) {
        GetController()->CancelStartNatSession();
    }
    nn::Result result = common::RESULT_NOT_SET;
    common::ExecuteResult::State state = GetController()->WaitStartNatSession(&result);
    if (state == common::ExecuteResult::STATE_CONTINUE) {
        // the session of the NAT session
        session::Session* pSession = session::Session::s_pInstance;
        u32 index = m_JointSessionId != 0 ? (pSession->m_CurrentIndex == 0 ? 1 : 0) : pSession->m_CurrentIndex;
        NexFacade::s_pInstance->m_Unknown0x10 = pSession->m_SessionIds[index];
        SetStep(&JoinSessionJob::MeshStartup, "JoinSessionJob::MeshStartup");
    } else if (state == common::ExecuteResult::STATE_SUCCESS) {
        // the NAT session failed
        if (result == common::RESULT_NAT_SERVER_NOT_FOUND || result == common::RESULT_NAT_CHECK_FAILED || result == common::RESULT_CANCELED ||
            result == common::RESULT_INVALID_STATE) {
            m_Result = result;
            SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_NOT_IN_SESSION) {
            SetSessionDisconnectState(3);
            m_Result = common::RESULT_NOT_IN_SESSION;
            SetStep(&JoinSessionJob::MeshCleanup, "NexMatchJoinSessionJob::MeshCleanup");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_Result = result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    return common::ExecuteResult(state);
}

// 0x00402048
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::JoinMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&JoinSessionJob::MeshCleanup, "NexMatchJoinSessionJob::MeshCleanup");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_OwnerPrincipalId = 0;
    m_JointOwnerPrincipalId = 0;
    GetController()->ResetSessionEntries();
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->JoinAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchJoinSessionJob::WaitJoinMatchmake, "NexMatchJoinSessionJob::WaitJoinMatchmake");
    m_Phase = 1;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004021F8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitJoinJointSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 jointSessionId;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsJoinCompleted(&jointSessionId, nullptr, nullptr)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_MATCHMAKE_SESSION_GONE) {
                // only the session itself
                m_JointSessionId = 0;
                SetStep(&NexMatchJoinSessionJob::GetStationConnectionInfo, "NexMatchJoinSessionJob::GetStationConnectionInfo");
                return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
            }
            m_Result = m_CallContext.m_Result;
            SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = m_JointSessionId;
            SetSessionState(1);
            m_CallContext.Reset();
            if (jointSessionId != 0) {
                // the joint session belongs to another one
                m_Result = common::RESULT_INVALID_STATE;
                SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            SetStep(&NexMatchJoinSessionJob::GetStationConnectionInfo, "NexMatchJoinSessionJob::GetStationConnectionInfo");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00402418
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::LeaveMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // the other (buffer) matchmake session first
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
        session::CommonMatchmakeSession* pOther = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        m_CallContext.Reset();
        nn::Result result = pOther->LeaveAsync(&m_CallContext, sessionId);
        if (result.IsSuccess()) {
            SetStep(&NexMatchJoinSessionJob::WaitLeaveBufferMatchmakeSession, "NexMatchJoinSessionJob::WaitLeaveBufferMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
    }
    // the sessions that the notifications still know
    NexMatchMeshLayerController* pController = GetController();
    if (pController != nullptr) {
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            u32 sessionId = pController->GetSessionId(i);
            if (sessionId == 0) {
                continue;
            }
            pSession = session::Session::s_pInstance;
            if (pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
                continue;
            }
            pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
            session::CommonMatchmakeSession* pOther = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
            m_CallContext.Reset();
            nn::Result result = pOther->LeaveAsync(&m_CallContext, sessionId);
            if (result.IsSuccess()) {
                SetStep(&NexMatchJoinSessionJob::WaitLeaveBufferMatchmakeSession, "NexMatchJoinSessionJob::WaitLeaveBufferMatchmakeSession");
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            if (result == common::RESULT_UNREGISTER_FAILED) {
                m_Result = result;
            }
            pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
        }
    }
    // the current one
    pSession = session::Session::s_pInstance;
    u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    if (sessionId != 0) {
        session::CommonMatchmakeSession* pCurrent = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        m_CallContext.Reset();
        nn::Result result = pCurrent->LeaveAsync(&m_CallContext, sessionId);
        if (result.IsSuccess()) {
            SetStep(&NexMatchJoinSessionJob::WaitLeaveCurrentMatchmakeSession, "NexMatchJoinSessionJob::WaitLeaveCurrentMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
    }
    SetStep(&JoinSessionJob::MeshCleanup, "NexMatchJoinSessionJob::MeshCleanup");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00402790
void nn::pia::inet::NexMatchJoinSessionJob::vf_0x1C(u32 sessionId)
{
    if (m_SessionId == sessionId || m_JointSessionId == sessionId) {
        m_IsHostLeft = true;
    }
}

// 0x004027AC
void nn::pia::inet::NexMatchJoinSessionJob::vf_0x18(u32 sessionId, u32 principalId)
{
    session::Session* pSession = session::Session::s_pInstance;
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    if (m_SessionId == sessionId) {
        m_OwnerPrincipalId = principalId;
    } else if (m_JointSessionId == sessionId) {
        m_JointOwnerPrincipalId = principalId;
    } else {
        return;
    }
    if (pSession->m_Unknown0x8C == principalId) {
        // the local station became the owner
        m_IsHostLeft = true;
        return;
    }
    transport::StationIdTable::Entry entry;
    if (pTransport->m_pStationIdTable->Find(&entry, principalId).IsFailure()) {
        m_Unknown0xB9 = true;
    }
    if (m_IsWaitingForOwnerChange) {
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        if (m_OwnerChangeDeadline < m_OwnerChangeRetryTime) {
            m_OwnerChangeRetryTime = m_OwnerChangeDeadline;
        }
    }
}

// 0x00402914
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::CleanupForRetryJoinMesh()
{
    Increment(common::g_SessionBeginMonitoringContent.m_Unknown0x22B);
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->IsUsingStationIdTable()) {
        session::Session::s_pInstance->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->RemoveLostStations();
    }
    pSession->m_pMeshLayerController->vf_0x14();
    if (m_IsMeshEvent19) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC_19);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsMeshEvent20) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC_20);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsConnectionFailed) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_Unknown0xB9) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_Unknown0xB5) {
        // again with the connection info of the host
        transport::StationConnectionInfo info;
        m_ConnectionInfo.SetStationConnectionInfo(info);
        SetStep(&NexMatchJoinSessionJob::GetStationConnectionInfo, "NexMatchJoinSessionJob::GetStationConnectionInfo");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00402D54
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::GetStationConnectionInfo()
{
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the host of the joint session if there is one
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession;
    u32 sessionId;
    if (m_JointSessionId == 0) {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    } else {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    }
    transport::StationConnectionInfo info;
    m_ConnectionInfo.SetStationConnectionInfo(info);
    nn::Result result = pMatchmakeSession->vf_0x50(&m_CallContext, sessionId);
    if (result.IsFailure()) {
        if (result == common::RESULT_NOT_IN_SESSION || result == common::RESULT_UNREGISTER_FAILED) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
            SetJoinTimeToMonitoringContent();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        m_Result = m_CallContext.m_Result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchJoinSessionJob::WaitGetStationConnectionInfo, "NexMatchJoinSessionJob::WaitGetStationConnectionInfo");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00402F6C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::vf_0x40()
{
    SetStep(&JoinSessionJob::CompleteFailure, "NexMatchJoinSessionJob::CompleteFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00402FCC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::vf_0x3C()
{
    SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403030
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_JoinedSessionNum == 0) {
        SetStep(&NexMatchJoinSessionJob::JoinMatchmakeSession, "NexMatchJoinSessionJob::JoinMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u32 sessionId = m_JoinedSessionIds[m_JoinedSessionNum - 1];
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    m_CallContext.Reset();
    nn::Result result = pMatchmakeSession->LeaveAsync(&m_CallContext, sessionId);
    if (result.IsFailure()) {
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
            SetStep(&JoinSessionJob::MeshCleanup, "NexMatchJoinSessionJob::MeshCleanup");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // the next one
        if (m_JoinedSessionNum != 0) {
            m_JoinedSessionNum--;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchJoinSessionJob::WaitLeaveJoinedMatchmakeSession, "NexMatchJoinSessionJob::WaitLeaveJoinedMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403208
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::vf_0x38(nn::Result result)
{
    m_Result = result;
    if (session::Mesh::s_pInstance->m_IsBandwidthCheckEnabled) {
        transport::BandwidthCheckerProtocol* pProtocol = session::Mesh::s_pInstance->m_pBandwidthCheckerProtocol;
        if (pProtocol->IsCheckStartable()) {
            pProtocol->Cancel();
        }
    }
    m_RetryCount++;
    if (m_RetryCount < RETRY_COUNT_MAX) {
        // the mesh join is retried
        common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
        if (m_Unknown0xB9) {
            Increment(content.m_Unknown0x251);
            SetStep(&NexMatchJoinSessionJob::CleanupForRetryJoinMesh, "NexMatchJoinSessionJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsMeshEvent19) {
            Increment(content.m_Unknown0x252);
            SetStep(&NexMatchJoinSessionJob::CleanupForRetryJoinMesh, "NexMatchJoinSessionJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsMeshEvent20) {
            Increment(content.m_Unknown0x253);
            SetStep(&NexMatchJoinSessionJob::CleanupForRetryJoinMesh, "NexMatchJoinSessionJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsConnectionFailed) {
            Increment(content.m_Unknown0x24F);
            SetStep(&NexMatchJoinSessionJob::CleanupForRetryJoinMesh, "NexMatchJoinSessionJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (!m_Unknown0xB5 && (result == common::RESULT_STATION_CONNECTION_FAILED_E7 || result == common::RESULT_STATION_CONNECTION_FAILED_E9 || result == common::RESULT_STATION_CONNECTION_FAILED_ED ||
                               result == common::RESULT_STATION_CONNECTION_FAILED_EC || result == common::RESULT_STATION_CONNECTION_FAILED_EB)) {
            m_Unknown0xB5 = true;
            Increment(content.m_Unknown0x250);
            SetStep(&NexMatchJoinSessionJob::CleanupForRetryJoinMesh, "NexMatchJoinSessionJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    // the sessions are left
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->IsUsingStationIdTable()) {
        session::Session::s_pInstance->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->RemoveLostStations();
    }
    pSession->m_pMeshLayerController->vf_0x14();
    SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004034F0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitGetStationConnectionInfo()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pMatchmakeSession = m_JointSessionId == 0 ? pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]
                                                                               : pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (!pMatchmakeSession->vf_0x54(&m_ConnectionInfo)) {
        // (the result is not used)
        IsCancelRequested();
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
        m_Result = m_CallContext.m_Result;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
        m_HostPrincipalId = m_ConnectionInfo.m_PublicLocation.m_PrincipalId;
        SetStep(&NexMatchJoinSessionJob::StartNatSession, "NexMatchJoinSessionJob::StartNatSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004036B0 (name is ours)
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitGetJoinedSessions()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    if (pMatchmakeSession->IsGetJoinedSessionsCompleted(JOINED_SESSION_NUM_MAX, m_JoinedSessionIds, &m_JoinedSessionNum)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_Result = m_CallContext.m_Result;
            SetStep(&JoinSessionJob::MeshCleanup, "NexMatchJoinSessionJob::MeshCleanup");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            m_CallContext.Reset();
            SetStep(&NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession, "NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040380C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitLeaveBufferMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsLeaveCompleted()) {
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
        }
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403930
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitLeaveJoinedMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsLeaveCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
        }
        // the next one
        if (m_JoinedSessionNum != 0) {
            m_JoinedSessionNum--;
        }
        SetStep(&NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession, "NexMatchJoinSessionJob::LeaveJoinedMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403A3C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitLeaveCurrentMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsLeaveCompleted()) {
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
        }
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403B4C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchJoinSessionJob::WaitChangeOwnerOfMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_Result = common::RESULT_CANCELED;
        m_IsWaitingForOwnerChange = false;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    const common::Time& now = common::Scheduler::s_pInstance->m_DispatchTime;
    if (m_OwnerChangeRetryTime < now) {
        // again with the connection info of the (new) host
        transport::StationConnectionInfo info;
        m_ConnectionInfo.SetStationConnectionInfo(info);
        m_IsWaitingForOwnerChange = false;
        SetStep(&NexMatchJoinSessionJob::GetStationConnectionInfo, "NexMatchJoinSessionJob::GetStationConnectionInfo");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_OwnerChangeDeadline < now) {
        m_IsWaitingForOwnerChange = false;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the local station became the owner: it leaves
    if (m_JointSessionId != 0) {
        if (m_JointOwnerPrincipalId != 0 && m_JointOwnerPrincipalId == session::Session::s_pInstance->m_Unknown0x8C) {
            m_IsWaitingForOwnerChange = false;
            SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    } else if (m_OwnerPrincipalId != 0 && m_OwnerPrincipalId == session::Session::s_pInstance->m_Unknown0x8C) {
        m_IsWaitingForOwnerChange = false;
        SetStep(&NexMatchJoinSessionJob::LeaveMatchmakeSession, "NexMatchJoinSessionJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00403DC8
nn::pia::inet::NexMatchJoinSessionJob::NexMatchJoinSessionJob() : m_OwnerChangeDeadline(), m_OwnerChangeRetryTime(), m_NotificationDeadline()
{
}

// 0x004320C8
// 0x00403E08 (deleting dtor)
nn::pia::inet::NexMatchJoinSessionJob::~NexMatchJoinSessionJob()
{
    // empty (in the original too)
}

// 0x0072F1D4
void nn::pia::inet::NexMatchJoinSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
