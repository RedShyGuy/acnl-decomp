#include "nn/pia/local/local_LocalParseSystemMessageJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x0042134C
nn::pia::common::ExecuteResult nn::pia::local::LocalParseSystemMessageJob::ParseSystemMessage()
{
    LocalNetwork::s_pInstance->m_pNetworkManager->ParseSystemMessages();
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00421378 (name is ours)
nn::Result nn::pia::local::LocalParseSystemMessageJob::Startup()
{
    SetStep(&LocalParseSystemMessageJob::ParseSystemMessage, "LocalParseSystemMessageJob::ParseSystemMessage");
    return nn::Result();
}

// 0x004213C8
nn::pia::local::LocalParseSystemMessageJob::LocalParseSystemMessageJob()
{
    // only the base and the vptr (in the original too)
}

// 0x004213F0
// 0x004213E0 (deleting dtor)
nn::pia::local::LocalParseSystemMessageJob::~LocalParseSystemMessageJob()
{
    // empty (in the original too)
}

// 0x007316B0
void nn::pia::local::LocalParseSystemMessageJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
