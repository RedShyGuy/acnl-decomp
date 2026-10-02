#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_UdsBackgroundProcessJob.h"

namespace nn {
namespace pia {
namespace local {
// 0x00420268 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::UdsBackgroundProcessJob::~UdsBackgroundProcessJob()
{
}

// 0x00731688 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::UdsBackgroundProcessJob::Trace(unsigned long long) const
{
}

// 0x0041F16C slot 0x18 | slot vf_0x18 of nn::pia::local::LocalBackgroundProcessJob
void nn::pia::local::UdsBackgroundProcessJob::StartupCreateNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalCreateNetworkSetting*)
{
}

// 0x0041F41C slot 0x1C | fefates:bytes
void nn::pia::local::UdsBackgroundProcessJob::StartupDestroyNetwork(nn::pia::common::CallContext*)
{
}

// 0x0041F0E8 slot 0x20 | slot vf_0x20 of nn::pia::local::LocalBackgroundProcessJob
void nn::pia::local::UdsBackgroundProcessJob::StartupScanNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*)
{
}

// 0x0041F298 slot 0x24 | fefates:callseq
void nn::pia::local::UdsBackgroundProcessJob::vf_0x24()
{
}

// 0x0041F600 slot 0x28 | fefates:bytes
void nn::pia::local::UdsBackgroundProcessJob::StartupDisconnectNetwork(nn::pia::common::CallContext*)
{
}

// 0x0041EBF8 | fefates:bytes [tier B]
void nn::pia::local::UdsBackgroundProcessJob::CreateNetwork()
{
}

// 0x0041ED64 | fefates:bytes [tier B]
void nn::pia::local::UdsBackgroundProcessJob::ConnectNetwork()
{
}

// 0x0041EF58 | fefates:bytes [tier B]
void nn::pia::local::UdsBackgroundProcessJob::DestroyNetwork()
{
}

// 0x0041F000 | fefates:bytes [tier B]
void nn::pia::local::UdsBackgroundProcessJob::DisconnectNetwork()
{
}

// 0x0041F670 | fefates:bytes [tier B]
nn::pia::local::UdsBackgroundProcessJob::UdsBackgroundProcessJob()
{
}

} // namespace local
} // namespace pia
} // namespace nn
