#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalDisconnectNetworkJob.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x004206C0 (unverified)
nn::pia::local::LocalDisconnectNetworkJob::LocalDisconnectNetworkJob()
{
}

// 0x00420748 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalDisconnectNetworkJob::~LocalDisconnectNetworkJob()
{
}

// 0x007316A4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalDisconnectNetworkJob::Trace(unsigned long long) const
{
}

// 0x0042034C | fefates:bytes [tier B]
void nn::pia::local::LocalDisconnectNetworkJob::WaitDisconnectNetwork()
{
}

// 0x00420648 | fefates:bytes [tier B]
void nn::pia::local::LocalDisconnectNetworkJob::Startup(nn::pia::common::CallContext*)
{
}

} // namespace local
} // namespace pia
} // namespace nn
