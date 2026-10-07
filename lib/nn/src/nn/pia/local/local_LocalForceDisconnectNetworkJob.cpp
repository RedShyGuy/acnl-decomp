#include "nn/pia/local/local_LocalForceDisconnectNetworkJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// 0x00423614
nn::pia::common::ExecuteResult nn::pia::local::LocalForceDisconnectNetworkJob::WaitDisconnected()
{
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (pNetwork->IsHost() || pNetwork->IsClient()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004236AC
nn::pia::common::ExecuteResult nn::pia::local::LocalForceDisconnectNetworkJob::WaitHostMigrationEnd()
{
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the leave request starts again with the CallContext of the caller
    m_pCallContext->SignalSuccess(nn::Result());
    nn::Result result = LocalNetwork::s_pInstance->DisconnectNetwork(m_pCallContext);
    if (result.IsFailure()) {
        m_pCallContext->Reset();
        m_pCallContext->InitiateCall();
        m_pCallContext->SignalFailure(result);
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00423778
void nn::pia::local::LocalForceDisconnectNetworkJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x004237AC
nn::Result nn::pia::local::LocalForceDisconnectNetworkJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::local::LocalForceDisconnectNetworkJob::ProcType procType)
{
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    switch (procType) {
    case PROC_TYPE_WAIT_HOST_MIGRATION_END:
        SetStep(&LocalForceDisconnectNetworkJob::WaitHostMigrationEnd, "LocalForceDisconnectNetworkJob::WaitHostMigrationEnd");
        break;
    case PROC_TYPE_WAIT_DISCONNECTED:
        SetStep(&LocalForceDisconnectNetworkJob::WaitDisconnected, "LocalForceDisconnectNetworkJob::WaitDisconnected");
        break;
    }
    return nn::Result();
}

// 0x0042388C
nn::pia::local::LocalForceDisconnectNetworkJob::LocalForceDisconnectNetworkJob() : m_pCallContext(nullptr)
{
}

// 0x004238BC
// 0x004238AC (deleting dtor)
nn::pia::local::LocalForceDisconnectNetworkJob::~LocalForceDisconnectNetworkJob()
{
    // empty (in the original too)
}

// 0x0073176C
void nn::pia::local::LocalForceDisconnectNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
