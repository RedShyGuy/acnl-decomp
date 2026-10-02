#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalForceDisconnectNetworkJob.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x0042388C (unverified)
nn::pia::local::LocalForceDisconnectNetworkJob::LocalForceDisconnectNetworkJob()
{
}

// 0x004238BC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalForceDisconnectNetworkJob::~LocalForceDisconnectNetworkJob()
{
}

// 0x0073176C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalForceDisconnectNetworkJob::Trace(unsigned long long) const
{
}

// 0x00423614 | fefates:bytes [tier B]
void nn::pia::local::LocalForceDisconnectNetworkJob::WaitDisconnected()
{
}

// 0x004236AC | fefates:bytes [tier B]
void nn::pia::local::LocalForceDisconnectNetworkJob::WaitHostMigrationEnd()
{
}

// 0x00423778 | fefates:bytes [tier B]
void nn::pia::local::LocalForceDisconnectNetworkJob::Cleanup()
{
}

// 0x004237AC | fefates:bytes [tier B]
void nn::pia::local::LocalForceDisconnectNetworkJob::Startup(nn::pia::common::CallContext*, nn::pia::local::LocalForceDisconnectNetworkJob::ProcType)
{
}

} // namespace local
} // namespace pia
} // namespace nn
