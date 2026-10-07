#include "nn/pia/local/local_LocalSendSystemMessageBackgroundJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
const u16 SEND_INTERVAL_MSEC = 15;
} // namespace

// 0x004257F4
nn::pia::common::ExecuteResult nn::pia::local::LocalSendSystemMessageBackgroundJob::SendSystemMessage()
{
    LocalNetwork::s_pInstance->m_pNetworkManager->SendSystemMessages();
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, SEND_INTERVAL_MSEC);
}

// 0x00425820 (name is ours)
nn::Result nn::pia::local::LocalSendSystemMessageBackgroundJob::Startup()
{
    Reset(false);
    SetStep(&LocalSendSystemMessageBackgroundJob::SendSystemMessage, "LocalSendSystemMessageBackgroundJob::SendSystemMessage");
    return nn::Result();
}

// 0x0042589C
nn::pia::local::LocalSendSystemMessageBackgroundJob::LocalSendSystemMessageBackgroundJob()
{
    // only the base and the vptr (in the original too)
}

// 0x004258C4
// 0x004258B4 (deleting dtor)
nn::pia::local::LocalSendSystemMessageBackgroundJob::~LocalSendSystemMessageBackgroundJob()
{
    // empty (in the original too)
}

// 0x00731800
void nn::pia::local::LocalSendSystemMessageBackgroundJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
