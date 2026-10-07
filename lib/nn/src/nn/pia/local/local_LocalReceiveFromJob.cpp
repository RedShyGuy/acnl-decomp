#include "nn/pia/local/local_LocalReceiveFromJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
const u16 RECEIVE_INTERVAL_MSEC = 15;
// while the input stream reads, the job looks again after that many milliseconds
const u16 INPUT_STREAM_CHECK_INTERVAL_MSEC = 100;
} // namespace

// 0x00419F20
nn::pia::common::ExecuteResult nn::pia::local::LocalReceiveFromJob::ReceiveFrom()
{
    LocalNetwork::s_pInstance->m_pNetworkManager->ReceiveFrom(nullptr, 0, nullptr, nullptr, true);
    SetStep(&LocalReceiveFromJob::CheckLocalInputStream, "LocalReceiveFromJob::CheckLocalInputStream");
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, RECEIVE_INTERVAL_MSEC);
}

// 0x00419FB8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalReceiveFromJob::CheckLocalInputStream()
{
    if (LocalNetwork::s_pInstance->m_pNetworkManager->IsActiveLocalInputStream()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, INPUT_STREAM_CHECK_INTERVAL_MSEC);
    }
    SetStep(&LocalReceiveFromJob::ReceiveFrom, "LocalReceiveFromJob::ReceiveFrom");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041A048 (name is ours)
nn::Result nn::pia::local::LocalReceiveFromJob::Startup()
{
    Reset(false);
    SetStep(&LocalReceiveFromJob::CheckLocalInputStream, "LocalReceiveFromJob::CheckLocalInputStream");
    return nn::Result();
}

// 0x0041A0B8
nn::pia::local::LocalReceiveFromJob::LocalReceiveFromJob()
{
    // only the base and the vptr (in the original too)
}

// 0x0041A0E0
// 0x0041A0D0 (deleting dtor)
nn::pia::local::LocalReceiveFromJob::~LocalReceiveFromJob()
{
    // empty (in the original too)
}

// 0x007311C8
void nn::pia::local::LocalReceiveFromJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
