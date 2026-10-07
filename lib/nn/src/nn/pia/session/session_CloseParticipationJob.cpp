#include "nn/pia/session/session_CloseParticipationJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the milliseconds since the time (name is ours)
inline s32 GetElapsedMSec(const common::Time& time)
{
    common::Time now;
    now.SetNow();
    return (now - time).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
}
} // namespace

// 0x0043EEE0
nn::pia::common::ExecuteResult nn::pia::session::CloseParticipationJob::WaitP2PStable()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // all stations of the session agree after a while
    if (GetElapsedMSec(m_StartTime) > P2P_STABLE_WAIT_MSEC) {
        Session* pSession = Session::s_pInstance;
        if (pSession->GetMeshLayerControllerValue() == pSession->GetStationNum()) {
            Session::s_pInstance->SetJoinable(m_SessionId, false);
            m_pCallContext->SignalSuccess(nn::Result());
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
    }
    if (GetElapsedMSec(m_StartTime) > P2P_STABLE_TIMEOUT_MSEC) {
        Session::s_pInstance->SetJoinable(m_SessionId, false);
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE_103);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043F098
nn::pia::common::ExecuteResult nn::pia::session::CloseParticipationJob::CloseParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = m_pSession->CloseParticipationAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&CloseParticipationJob::WaitCloseParticipation, "CloseParticipationJob::WaitCloseParticipation");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043F1E0
nn::pia::common::ExecuteResult nn::pia::session::CloseParticipationJob::WaitCloseParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
    } else {
        if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (!m_pSession->IsCloseParticipationCompleted()) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                Session::s_pInstance->SetDisconnectedByError();
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
        } else if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&CloseParticipationJob::WaitP2PStable, "CloseParticipationJob::WaitP2PStable");
            m_StartTime.SetNow();
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043F340 (name is ours)
void nn::pia::session::CloseParticipationJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
}

// 0x0043F384 (name is ours)
nn::Result nn::pia::session::CloseParticipationJob::Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId,
                                                            nn::pia::session::CommonMatchmakeSession* pSession)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSession)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!pSession->vf_0x88()) {
        return common::RESULT_INVALID_STATE;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_pSession = pSession;
    m_SessionId = sessionId;
    Reset(true);
    SetStep(&CloseParticipationJob::CloseParticipation, "CloseParticipationJob::CloseParticipation");
    return nn::Result();
}

// 0x0043F474
nn::pia::session::CloseParticipationJob::CloseParticipationJob() : m_pCallContext(nullptr), m_SessionId(0), m_pSession(nullptr)
{
}

// 0x0043F4D4
// 0x0043F4B0 (deleting dtor)
nn::pia::session::CloseParticipationJob::~CloseParticipationJob()
{
    // empty (in the original too)
}

// 0x00734054
void nn::pia::session::CloseParticipationJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
