#include "nn/pia/inet/inet_NexMatchDestroySessionJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040BCFC
nn::Result nn::pia::inet::NexMatchDestroySessionJob::vf_0x1C()
{
    return nn::Result();
}

// 0x0040BD04
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchDestroySessionJob::CompleteProcess()
{
    if (m_Result.IsFailure()) {
        m_pCallContext->SignalFailure(m_Result);
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0040BD48
void nn::pia::inet::NexMatchDestroySessionJob::vf_0x24()
{
    SetStep(&NexMatchDestroySessionJob::CompleteProcess, "NexMatchDestroySessionJob::CompleteProcess");
}

// 0x0040BD9C
void nn::pia::inet::NexMatchDestroySessionJob::vf_0x20()
{
    SetStep(&NexMatchDestroySessionJob::UnregisterBufferMatchmakeSession, "NexMatchDestroySessionJob::UnregisterBufferMatchmakeSession");
}

// 0x0040BE00
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchDestroySessionJob::UnregisterBufferMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // the other matchmake session of a joint session (the "buffer") goes first
    session::Session* pSession = session::Session::s_pInstance;
    u32 otherIndex = pSession->m_CurrentIndex == 0 ? 1 : 0;
    if (pSession->m_SessionIds[otherIndex] == 0) {
        SetStep(&NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession, "NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (pSession->m_pMatchmakeSessions[otherIndex]->UnregisterAsync(&m_CallContext, pSession->m_SessionIds[otherIndex]).IsFailure()) {
        m_Result = m_CallContext.m_Result;
        SetStep(&NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession, "NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchDestroySessionJob::WaitUnregisterBufferMatchmakeSession, "NexMatchDestroySessionJob::WaitUnregisterBufferMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040BFB0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    session::CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[index];
    if (pSession->m_SessionIds[index] == 0) {
        SetStep(&DestroySessionJob::MeshCleanup, "DestroySessionJob::MeshCleanup");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    nn::Result result = pMatchmakeSession->UnregisterAsync(&m_CallContext, pSession->m_SessionIds[index]);
    if (result.IsFailure()) {
        m_Result = result;
        SetStep(&DestroySessionJob::MeshCleanup, "DestroySessionJob::MeshCleanup");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchDestroySessionJob::WaitUnregisterCurrentMatchmakeSession, "NexMatchDestroySessionJob::WaitUnregisterCurrentMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040C0CC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchDestroySessionJob::WaitUnregisterBufferMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsUnregisterCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
    SetUnregisterResult();
    SetStep(&NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession, "NexMatchDestroySessionJob::UnregisterCurrentMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040C230
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchDestroySessionJob::WaitUnregisterCurrentMatchmakeSession()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsUnregisterCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    pSession = session::Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
    SetUnregisterResult();
    SetStep(&DestroySessionJob::MeshCleanup, "DestroySessionJob::MeshCleanup");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040C360
nn::pia::inet::NexMatchDestroySessionJob::NexMatchDestroySessionJob()
{
    // nothing more than the base (in the original too)
}

// 0x00438F30
// 0x0040C378 (deleting dtor)
nn::pia::inet::NexMatchDestroySessionJob::~NexMatchDestroySessionJob()
{
    // empty (in the original too)
}

// 0x00438D38
void nn::pia::inet::NexMatchDestroySessionJob::Cleanup()
{
    DestroySessionJob::Cleanup();
}

// 0x0072F850
void nn::pia::inet::NexMatchDestroySessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
