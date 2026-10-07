#include "nn/pia/inet/inet_NexMatchAutoMatchmakeJob.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteria.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
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

// 0x00407D7C
void nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x2C()
{
    m_OwnerPrincipalId = 0;
    m_JointOwnerPrincipalId = 0;
    m_JointSessionId = 0;
    m_IsWaitingForOwnerChange = false;
    if (m_pSession != nullptr) {
        m_pSession->FinishBrowse();
        m_pSession = nullptr;
    }
    m_IsJoinable = false;
}

// 0x00407DB4
nn::Result nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x28(const nn::pia::session::CreateSessionSetting* pCreateSetting,
                                                             const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum)
{
    session::Session* pSession = session::Session::s_pInstance;
    if (criteriaNum > CRITERIA_NUM_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    if (!common::IsValidPointer(m_pSession)) {
        return common::RESULT_INVALID_STATE;
    }
    const NexCreateSessionSetting* pNexSetting = static_cast<const NexCreateSessionSetting*>(pCreateSetting);
    if (!m_pSession->SetSearchCriteria(static_cast<const NexSessionSearchCriteria*>(pCriteria), criteriaNum) || pCreateSetting->m_Unknown0x6 == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u16 entryNum = pNexSetting->GetUnknown0x474() + pCreateSetting->m_Unknown0x6;
    if (entryNum > transport::Transport::s_pInstance->m_StationNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pSession->SetCreateSetting(pNexSetting, pSession->m_IsHostMigrationEnabled);
    m_IsJoinable = pNexSetting->IsOpenParticipation();
    if (pSession->IsUsingStationIdTable()) {
        session::Session::s_pInstance->m_StationIdEntryNumMax[session::Session::s_pInstance->m_CurrentIndex] = entryNum;
    }
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
    SetStep(&NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession");
    return nn::Result();
}

// 0x00407F70
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::AutoMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&AutoMatchmakeJob::CompleteFailure, "NexMatchAutoMatchmakeJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_OwnerPrincipalId = 0;
    m_JointOwnerPrincipalId = 0;
    GetController()->ResetSessionEntries();
    nn::Result result = m_pSession->AutoMatchmakeAsync(&m_CallContext);
    if (result.IsFailure()) {
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
        }
        m_pSession->FinishBrowse();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchAutoMatchmakeJob::WaitAutoMatchmake, "NexMatchAutoMatchmakeJob::WaitAutoMatchmake");
    m_Phase = 1;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004080F4
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::StartNatSession()
{
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nn::Result result = GetController()->StartNatSession();
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchAutoMatchmakeJob::WaitStartNatSession, "NexMatchAutoMatchmakeJob::WaitStartNatSession");
    m_Phase = 3;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00408224
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::JoinJointSession()
{
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->JoinAsync(&m_CallContext, m_JointSessionId);
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchAutoMatchmakeJob::WaitJoinJointSession, "NexMatchAutoMatchmakeJob::WaitJoinJointSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00408334
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitNotification()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_NotificationDeadline < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Result = common::RESULT_UNREGISTER_FAILED;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the notification of the own participation
    NexMatchMeshLayerController* pController = GetController();
    if (!pController->HasParticipant(m_SessionId, NexFacade::s_pInstance->m_pNgsBridge->vf_0x0C())) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (session::Session::s_pInstance->IsUsingStationIdTable()) {
        if (session::Session::s_pInstance->m_pMeshLayerController->vf_0x34() && m_JointSessionId != 0) {
            m_CallContext.Reset();
            SetStep(&NexMatchAutoMatchmakeJob::JoinJointSession, "NexMatchAutoMatchmakeJob::JoinJointSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_JointSessionId = 0;
    }
    SetStep(&NexMatchAutoMatchmakeJob::StartNatSession, "NexMatchAutoMatchmakeJob::StartNatSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00408580
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitAutoMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pSession->IsAutoMatchmakeCompleted(&m_SessionId, &m_IsCreator, &m_JointSessionId, nullptr, nullptr)) {
        m_pSession->FinishBrowse();
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_Result = m_CallContext.m_Result;
            SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            session::Session* pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex] = m_SessionId;
            if (m_IsCreator) {
                session::Session::s_pInstance->SetJoinable(m_SessionId, m_IsJoinable);
            }
            SetSessionState(1);
            m_NotificationDeadline = GetTimeAfter(NOTIFICATION_TIMEOUT_MSEC);
            SetStep(&NexMatchAutoMatchmakeJob::WaitNotification, "NexMatchAutoMatchmakeJob::WaitNotification");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00408778
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitStartNatSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    NexMatchMeshLayerController* pController = GetController();
    nn::Result result = common::RESULT_NOT_SET;
    common::ExecuteResult::State state = pController->WaitStartNatSession(&result);
    if (IsCancelRequested()) {
        pController->CancelStartNatSession();
    }
    if (state == common::ExecuteResult::STATE_CONTINUE) {
        // the session of the NAT session
        session::Session* pSession = session::Session::s_pInstance;
        u32 index = m_JointSessionId != 0 ? (pSession->m_CurrentIndex == 0 ? 1 : 0) : pSession->m_CurrentIndex;
        NexFacade::s_pInstance->m_Unknown0x10 = pSession->m_SessionIds[index];
        SetStep(&AutoMatchmakeJob::MeshStartup, "AutoMatchmakeJob::MeshStartup");
    } else if (state == common::ExecuteResult::STATE_SUCCESS) {
        // the NAT session failed
        if (result == common::RESULT_NAT_SERVER_NOT_FOUND || result == common::RESULT_NAT_CHECK_FAILED || result == common::RESULT_CANCELED ||
            result == common::RESULT_INVALID_STATE) {
            m_Result = result;
            SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_NOT_IN_SESSION) {
            SetSessionDisconnectState(3);
            m_Result = common::RESULT_NOT_IN_SESSION;
            SetStep(&AutoMatchmakeJob::CompleteFailure, "NexMatchAutoMatchmakeJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_Result = common::RESULT_INVALID_STATE;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    return common::ExecuteResult(state);
}

// 0x004089C0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitJoinJointSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 jointSessionId;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsJoinCompleted(&jointSessionId, nullptr, nullptr)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_CallContext.m_Result.IsFailure()) {
        if (m_CallContext.m_Result == common::RESULT_MATCHMAKE_SESSION_GONE) {
            // only the session itself
            m_JointSessionId = 0;
            SetStep(&NexMatchAutoMatchmakeJob::StartNatSession, "NexMatchAutoMatchmakeJob::StartNatSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        m_Result = m_CallContext.m_Result;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (jointSessionId != 0) {
        // the joint session belongs to another one
        m_Result = common::RESULT_INVALID_STATE;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = m_JointSessionId;
    SetStep(&NexMatchAutoMatchmakeJob::StartNatSession, "NexMatchAutoMatchmakeJob::StartNatSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00408BA4
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::LeaveMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // the other (buffer) matchmake session first
    session::Session* pSession = session::Session::s_pInstance;
    u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (sessionId != 0) {
        m_CallContext.Reset();
        nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->LeaveAsync(&m_CallContext, sessionId);
        if (result.IsSuccess()) {
            SetStep(&NexMatchAutoMatchmakeJob::WaitLeaveBufferMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitLeaveBufferMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
        session::Session* pSession2 = session::Session::s_pInstance;
        pSession2->m_SessionIds[pSession2->m_CurrentIndex == 0 ? 1 : 0] = 0;
    }
    // the sessions that the notifications still know
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    if (pController != nullptr) {
        for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
            u32 joinedSessionId = pController->GetSessionId(i);
            if (joinedSessionId == 0 || pSession->m_SessionIds[pSession->m_CurrentIndex] == joinedSessionId) {
                continue;
            }
            session::Session* pSession2 = session::Session::s_pInstance;
            pSession2->m_SessionIds[pSession2->m_CurrentIndex == 0 ? 1 : 0] = joinedSessionId;
            m_CallContext.Reset();
            nn::Result result = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->LeaveAsync(&m_CallContext, joinedSessionId);
            if (result.IsSuccess()) {
                SetStep(&NexMatchAutoMatchmakeJob::WaitLeaveBufferMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitLeaveBufferMatchmakeSession");
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            if (result == common::RESULT_UNREGISTER_FAILED) {
                m_Result = result;
            }
            pSession2 = session::Session::s_pInstance;
            pSession2->m_SessionIds[pSession2->m_CurrentIndex == 0 ? 1 : 0] = 0;
        }
    }
    // the current one: a creator unregisters it
    sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    if (sessionId != 0) {
        session::CommonMatchmakeSession* pCurrent = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        nn::Result result = m_IsCreator ? pCurrent->UnregisterAsync(&m_CallContext, sessionId) : pCurrent->LeaveAsync(&m_CallContext, sessionId);
        if (result.IsSuccess()) {
            SetStep(&NexMatchAutoMatchmakeJob::WaitLeaveCurrentMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitLeaveCurrentMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
        session::Session* pSession2 = session::Session::s_pInstance;
        pSession2->m_SessionIds[pSession2->m_CurrentIndex] = 0;
    }
    SetStep(&AutoMatchmakeJob::CompleteFailure, "NexMatchAutoMatchmakeJob::CompleteFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00408F14
void nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x24(u32 sessionId)
{
    if (m_SessionId == sessionId || m_JointSessionId == sessionId) {
        m_IsHostLeft = true;
    }
}

// 0x00408F30
void nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x20(u32 sessionId, u32 principalId)
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
        m_Unknown0x58 = true;
    }
    if (m_IsWaitingForOwnerChange) {
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        if (m_OwnerChangeDeadline < m_OwnerChangeRetryTime) {
            m_OwnerChangeRetryTime = m_OwnerChangeDeadline;
        }
    }
}

// 0x004090A4
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh()
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
        SetStep(&NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsMeshEvent20) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC_20);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsConnectionFailed) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_Unknown0x58) {
        m_OwnerChangeDeadline = GetTimeAfter(OWNER_CHANGE_TIMEOUT_MSEC);
        m_OwnerChangeRetryTime = GetTimeAfter(OWNER_CHANGE_RETRY_MSEC);
        m_IsWaitingForOwnerChange = true;
        SetStep(&NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_Unknown0x54) {
        SetStep(&NexMatchAutoMatchmakeJob::StartNatSession, "NexMatchAutoMatchmakeJob::StartNatSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004094B0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x3C()
{
    SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409514
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_JoinedSessionNum == 0) {
        SetStep(&NexMatchAutoMatchmakeJob::AutoMatchmake, "NexMatchAutoMatchmakeJob::AutoMatchmake");
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
            SetStep(&AutoMatchmakeJob::CompleteFailure, "NexMatchAutoMatchmakeJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // the next one
        if (m_JoinedSessionNum != 0) {
            m_JoinedSessionNum--;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchAutoMatchmakeJob::WaitLeaveJoinedMatchmakeSession, "NexMatchAutoMatchmakeJob::WaitLeaveJoinedMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004096F0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x38(nn::Result result)
{
    m_Result = result;
    m_RetryCount++;
    if (m_RetryCount < RETRY_COUNT_MAX) {
        // the mesh join is retried
        common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
        if (m_Unknown0x58) {
            Increment(content.m_Unknown0x251);
            SetStep(&NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh, "NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsMeshEvent19) {
            Increment(content.m_Unknown0x252);
            SetStep(&NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh, "NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsMeshEvent20) {
            Increment(content.m_Unknown0x253);
            SetStep(&NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh, "NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_IsConnectionFailed) {
            Increment(content.m_Unknown0x24F);
            SetStep(&NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh, "NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (!m_Unknown0x54 && (result == common::RESULT_STATION_CONNECTION_FAILED_E7 || result == common::RESULT_STATION_CONNECTION_FAILED_E9 ||
                               result == common::RESULT_STATION_CONNECTION_FAILED_ED || result == common::RESULT_STATION_CONNECTION_FAILED_EC ||
                               result == common::RESULT_STATION_CONNECTION_FAILED_EB)) {
            m_Unknown0x54 = true;
            Increment(content.m_Unknown0x250);
            SetStep(&NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh, "NexMatchAutoMatchmakeJob::CleanupForRetryJoinMesh");
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
    SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004099A4
void nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x30()
{
    SetStep(&AutoMatchmakeJob::CompleteProcess, "AutoMatchmakeJob::CompleteProcess");
}

// 0x004099F0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::vf_0x34(nn::Result result)
{
    m_Result = result;
    SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409A58 (name is ours)
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitGetJoinedSessions()
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
            SetStep(&AutoMatchmakeJob::CompleteFailure, "NexMatchAutoMatchmakeJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            m_CallContext.Reset();
            SetStep(&NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409BC8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitLeaveBufferMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsLeaveCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
        }
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409CEC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitLeaveJoinedMatchmakeSession()
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
        SetStep(&NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveJoinedMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409DFC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitLeaveCurrentMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    session::CommonMatchmakeSession* pCurrent = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    if (m_IsCreator ? pCurrent->IsUnregisterCompleted() : pCurrent->IsLeaveCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
        }
        pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00409F10
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchAutoMatchmakeJob::WaitChangeOwnerOfMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    bool isLeaving;
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_Result = common::RESULT_CANCELED;
        isLeaving = true;
    } else {
        const common::Time& now = common::Scheduler::s_pInstance->m_DispatchTime;
        if (m_OwnerChangeRetryTime < now) {
            // the NAT session starts again (with the new host)
            m_IsWaitingForOwnerChange = false;
            SetStep(&NexMatchAutoMatchmakeJob::StartNatSession, "NexMatchAutoMatchmakeJob::StartNatSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_OwnerChangeDeadline < now) {
            isLeaving = true;
        } else if (m_JointSessionId != 0) {
            // the local station became the owner: it leaves
            isLeaving = m_JointOwnerPrincipalId != 0 && m_JointOwnerPrincipalId == session::Session::s_pInstance->m_Unknown0x8C;
        } else {
            isLeaving = m_OwnerPrincipalId != 0 && m_OwnerPrincipalId == session::Session::s_pInstance->m_Unknown0x8C;
        }
    }
    if (isLeaving) {
        m_IsWaitingForOwnerChange = false;
        SetStep(&NexMatchAutoMatchmakeJob::LeaveMatchmakeSession, "NexMatchAutoMatchmakeJob::LeaveMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040A0CC
nn::pia::inet::NexMatchAutoMatchmakeJob::NexMatchAutoMatchmakeJob()
    : m_pSession(nullptr), m_OwnerChangeDeadline(), m_OwnerChangeRetryTime(), m_NotificationDeadline(), m_IsJoinable(false)
{
}

// 0x00437CC8
// 0x0040A118 (deleting dtor)
nn::pia::inet::NexMatchAutoMatchmakeJob::~NexMatchAutoMatchmakeJob()
{
    // empty (in the original too)
}

// 0x0072F844
void nn::pia::inet::NexMatchAutoMatchmakeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
