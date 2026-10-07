#include "nn/pia/local/local_LocalMatchLeaveSessionJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace local {
// 0x00420784
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchLeaveSessionJob::CompleteProcess()
{
    if (m_Result.IsFailure()) {
        m_pCallContext->SignalFailure(m_Result);
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004207C8
void nn::pia::local::LocalMatchLeaveSessionJob::vf_0x24()
{
    SetStep(&LeaveSessionJob::LeaveCurrentMatchmakeSession, "LeaveSessionJob::LeaveCurrentMatchmakeSession");
}

// 0x00420820
void nn::pia::local::LocalMatchLeaveSessionJob::vf_0x1C()
{
    SetStep(&LeaveSessionJob::MeshCleanup, "LeaveSessionJob::MeshCleanup");
}

// 0x00420868
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchLeaveSessionJob::RetryLeaveCurrentMatchmakeSession()
{
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (!pNetwork->IsHost() && !pNetwork->IsClient()) {
        // the network is left already
        vf_0x20();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    if (pSession->m_pMatchmakeSessions[index]->LeaveAsync(&m_CallContext, pSession->m_SessionIds[index]).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LeaveSessionJob::WaitLeaveCurrentMatchmakeSession, "LeaveSessionJob::WaitLeaveCurrentMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00420960
bool nn::pia::local::LocalMatchLeaveSessionJob::vf_0x2C()
{
    return false;
}

// 0x00420968
void nn::pia::local::LocalMatchLeaveSessionJob::vf_0x20()
{
    SetStep(&LocalMatchLeaveSessionJob::CompleteProcess, "LocalMatchLeaveSessionJob::CompleteProcess");
}

// 0x004209BC
void nn::pia::local::LocalMatchLeaveSessionJob::vf_0x28()
{
    SetStep(&LocalMatchLeaveSessionJob::RetryLeaveCurrentMatchmakeSession, "LocalMatchLeaveSessionJob::RetryLeaveCurrentMatchmakeSession");
}

// 0x00420A24
nn::pia::local::LocalMatchLeaveSessionJob::LocalMatchLeaveSessionJob()
{
    // nothing more than the base (in the original too)
}

// 0x00420A4C
// 0x00420A3C (deleting dtor)
nn::pia::local::LocalMatchLeaveSessionJob::~LocalMatchLeaveSessionJob()
{
    // empty (in the original too)
}

// 0x007316A8
void nn::pia::local::LocalMatchLeaveSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
