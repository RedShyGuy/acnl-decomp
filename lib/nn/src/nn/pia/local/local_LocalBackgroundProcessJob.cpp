#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"

namespace nn {
namespace pia {
namespace local {
// 0x0042026C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalBackgroundProcessJob::~LocalBackgroundProcessJob()
{
}

// 0x0073169C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalBackgroundProcessJob::Trace(unsigned long long) const
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalBackgroundProcessJob::StartupCreateNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalCreateNetworkSetting*)
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalBackgroundProcessJob::StartupDestroyNetwork(nn::pia::common::CallContext*)
{
}

// 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalBackgroundProcessJob::StartupScanNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*)
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalBackgroundProcessJob::vf_0x24()
{
}

// 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
void nn::pia::local::LocalBackgroundProcessJob::StartupDisconnectNetwork(nn::pia::common::CallContext*)
{
}

// 0x0042005C | fefates:bytes [tier B]
void nn::pia::local::LocalBackgroundProcessJob::PrepareDestroyNetwork()
{
}

// 0x004200E4 | fefates:bytes [tier B]
void nn::pia::local::LocalBackgroundProcessJob::PrepareDisconnectNetwork()
{
}

// 0x0042016C | fefates:bytes [tier B]
void nn::pia::local::LocalBackgroundProcessJob::Cleanup()
{
}

// 0x004201B0 | fefates:bytes [tier B]
void nn::pia::local::LocalBackgroundProcessJob::Startup(nn::pia::common::CallContext*, nn::pia::local::LocalBackgroundProcessJob::JobPriority)
{
}

// 0x00420228 | fefates:bytes [tier B]
nn::pia::local::LocalBackgroundProcessJob::LocalBackgroundProcessJob()
{
}

} // namespace local
} // namespace pia
} // namespace nn
