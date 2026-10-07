#include "nn/pia/local/local_LocalMatchDestroySessionJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace local {
// 0x00422C44
nn::Result nn::pia::local::LocalMatchDestroySessionJob::vf_0x1C()
{
    return nn::Result();
}

// 0x00422C4C
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchDestroySessionJob::DestroyLocalNetwork()
{
    session::Session* pSession = session::Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    nn::Result result = pSession->m_pMatchmakeSessions[index]->UnregisterAsync(&m_CallContext, pSession->m_SessionIds[index]);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&LocalMatchDestroySessionJob::WaitDestroyLocalNetwork, "LocalMatchDestroySessionJob::WaitDestroyLocalNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00422D10
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchDestroySessionJob::WaitDestroyLocalNetwork()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsUnregisterCompleted()) {
        pSession = session::Session::s_pInstance;
        pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            // a failure of the mesh comes first
            if (m_Result.IsFailure()) {
                m_pCallContext->SignalFailure(m_Result);
            } else {
                m_pCallContext->SignalSuccess(nn::Result());
            }
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00422DC4
void nn::pia::local::LocalMatchDestroySessionJob::vf_0x24()
{
    SetStep(&LocalMatchDestroySessionJob::DestroyLocalNetwork, "LocalMatchDestroySessionJob::DestroyLocalNetwork");
}

// 0x00422E20
void nn::pia::local::LocalMatchDestroySessionJob::vf_0x20()
{
    SetStep(&DestroySessionJob::MeshCleanup, "DestroySessionJob::MeshCleanup");
}

// 0x00422E68
void nn::pia::local::LocalMatchDestroySessionJob::Cleanup()
{
    DestroySessionJob::Cleanup();
}

// 0x00422E6C
nn::pia::local::LocalMatchDestroySessionJob::LocalMatchDestroySessionJob()
{
    // nothing more than the base (in the original too)
}

// 0x00422E94
// 0x00422E84 (deleting dtor)
nn::pia::local::LocalMatchDestroySessionJob::~LocalMatchDestroySessionJob()
{
    // empty (in the original too)
}

// 0x007316C4
void nn::pia::local::LocalMatchDestroySessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
