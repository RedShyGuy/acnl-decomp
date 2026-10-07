#include "nn/pia/inet/inet_NexMatchCreateSessionJob.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040A128
nn::Result nn::pia::inet::NexMatchCreateSessionJob::vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting)
{
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    if (!common::IsValidPointer(m_pSession)) {
        return common::RESULT_INVALID_STATE;
    }
    const NexCreateSessionSetting* pNexSetting = static_cast<const NexCreateSessionSetting*>(pSetting);
    if (pSetting->m_Unknown0x6 == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u16 entryNum = pNexSetting->GetUnknown0x474() + pSetting->m_Unknown0x6;
    if (entryNum > transport::Transport::s_pInstance->m_StationNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pSession->SetCreateSetting(pNexSetting, pSession->m_IsHostMigrationEnabled);
    m_IsJoinable = pNexSetting->IsOpenParticipation();
    if (pSession->IsUsingStationIdTable()) {
        session::Session::s_pInstance->m_StationIdEntryNumMax[session::Session::s_pInstance->m_CurrentIndex] = entryNum;
    }
    // the sessions that the notifications still know are left first
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    m_JoinedSessionNum = 0;
    for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
        if (pController->GetSessionId(i) != 0) {
            m_JoinedSessionIds[m_JoinedSessionNum] = pController->GetSessionId(i);
            m_JoinedSessionNum++;
        }
    }
    SetStep(&NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession, "NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession");
    return nn::Result();
}

// 0x0040A2CC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::StartNatSession()
{
    nn::Result result = static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController)->StartNatSession();
    if (result.IsFailure()) {
        m_Result = result;
        Cleanup();
        SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexMatchCreateSessionJob::WaitStartNatSession, "NexMatchCreateSessionJob::WaitStartNatSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040A3B8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitNotification()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_NotificationDeadline < common::Scheduler::s_pInstance->m_DispatchTime) {
        m_Result = common::RESULT_UNREGISTER_FAILED;
        SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the notification of the own participation
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(pSession->m_pMeshLayerController);
    u32 principalId = NexFacade::s_pInstance->m_pNgsBridge->vf_0x0C();
    if (pController->HasParticipant(session::Session::s_pInstance->m_SessionIds[session::Session::s_pInstance->m_CurrentIndex], principalId)) {
        SetStep(&NexMatchCreateSessionJob::StartNatSession, "NexMatchCreateSessionJob::StartNatSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040A58C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::UnregisterGathering()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    nn::Result result = pSession->m_pMatchmakeSessions[index]->UnregisterAsync(&m_CallContext, pSession->m_SessionIds[index]);
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&CreateSessionJob::CompleteFailure, "NexMatchCreateSessionJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchCreateSessionJob::WaitUnregisterGathering, "NexMatchCreateSessionJob::WaitUnregisterGathering");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040A6C0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitCreateMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    u32 sessionId = 0;
    if (m_pSession->IsCreateCompleted(&sessionId)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_Result = m_CallContext.m_Result;
            SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            session::Session* pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex] = sessionId;
            SetSessionState(1);
            session::Session::s_pInstance->SetJoinable(sessionId, m_IsJoinable);
            m_NotificationDeadline =
                common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * NOTIFICATION_TIMEOUT_MSEC);
            SetStep(&NexMatchCreateSessionJob::WaitNotification, "NexMatchCreateSessionJob::WaitNotification");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040A87C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitStartNatSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nn::Result result = common::RESULT_NOT_SET;
    common::ExecuteResult::State state =
        static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController)->WaitStartNatSession(&result);
    if (state == common::ExecuteResult::STATE_CONTINUE) {
        SetStep(&CreateSessionJob::MeshStartup, "CreateSessionJob::MeshStartup");
    } else if (state == common::ExecuteResult::STATE_SUCCESS) {
        // the NAT session failed
        if (result == common::RESULT_NAT_SERVER_NOT_FOUND || result == common::RESULT_NAT_CHECK_FAILED || result == common::RESULT_CANCELED ||
            result == common::RESULT_INVALID_STATE) {
            m_Result = result;
            SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (result == common::RESULT_NOT_IN_SESSION) {
            SetSessionDisconnectState(3);
            m_Result = result;
            SetStep(&CreateSessionJob::CompleteFailure, "NexMatchCreateSessionJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_Result = common::RESULT_INVALID_STATE;
        SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    return common::ExecuteResult(state);
}

// 0x0040AAB4
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::CreateMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController)->ResetSessionEntries();
    nn::Result result = m_pSession->CreateAsync(&m_CallContext);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
    }
    SetStep(&NexMatchCreateSessionJob::WaitCreateMatchmake, "NexMatchCreateSessionJob::WaitCreateMatchmake");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040ABDC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitUnregisterGathering()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsUnregisterCompleted()) {
        session::Session::s_pInstance->m_SessionIds[session::Session::s_pInstance->m_CurrentIndex] = 0;
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                m_Result = common::RESULT_UNREGISTER_FAILED;
            } else if (m_CallContext.m_Result == common::RESULT_FATAL_196) {
                m_Result = common::RESULT_FATAL_196;
            }
            SetStep(&CreateSessionJob::CompleteFailure, "NexMatchCreateSessionJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            // the creation had failed
            m_pCallContext->SignalFailure(m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040AD2C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_JoinedSessionNum == 0) {
        SetStep(&NexMatchCreateSessionJob::CreateMatchmakeSession, "NexMatchCreateSessionJob::CreateMatchmakeSession");
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
            SetStep(&CreateSessionJob::CompleteFailure, "NexMatchCreateSessionJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // the next one
        if (m_JoinedSessionNum != 0) {
            m_JoinedSessionNum--;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchCreateSessionJob::WaitLeaveJoinedMatchmakeSession, "NexMatchCreateSessionJob::WaitLeaveJoinedMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040AF14
void nn::pia::inet::NexMatchCreateSessionJob::vf_0x20()
{
    SetStep(&CreateSessionJob::CompleteProcess, "CreateSessionJob::CompleteProcess");
}

// 0x0040AF60
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::vf_0x24(nn::Result result)
{
    m_Result = result;
    SetStep(&NexMatchCreateSessionJob::UnregisterGathering, "NexMatchCreateSessionJob::UnregisterGathering");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040AFC8 (name is ours)
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitGetJoinedSessions()
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
            SetStep(&CreateSessionJob::CompleteFailure, "NexMatchCreateSessionJob::CompleteFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            m_CallContext.Reset();
            SetStep(&NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession, "NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040B130
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchCreateSessionJob::WaitLeaveJoinedMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsLeaveCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_MATCHMAKE_SESSION_GONE) {
                // (nothing: the session is gone anyway)
            } else if (m_CallContext.m_Result == common::RESULT_NOT_IN_SESSION) {
                m_Result = m_CallContext.m_Result;
            }
        }
        // the next one
        if (m_JoinedSessionNum != 0) {
            m_JoinedSessionNum--;
        }
        SetStep(&NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession, "NexMatchCreateSessionJob::LeaveJoinedMatchmakeSession");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040B240
void nn::pia::inet::NexMatchCreateSessionJob::Cleanup()
{
    CreateSessionJob::Cleanup();
    m_pSession = nullptr;
    m_IsJoinable = false;
}

// 0x0040B25C
nn::pia::inet::NexMatchCreateSessionJob::NexMatchCreateSessionJob() : m_pSession(nullptr), m_NotificationDeadline(), m_IsJoinable(false)
{
}

// 0x004382A0
// 0x0040B288 (deleting dtor)
nn::pia::inet::NexMatchCreateSessionJob::~NexMatchCreateSessionJob()
{
    // empty (in the original too)
}

// 0x0072F848
void nn::pia::inet::NexMatchCreateSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
