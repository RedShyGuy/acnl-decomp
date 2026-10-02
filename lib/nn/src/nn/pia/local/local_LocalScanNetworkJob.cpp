#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalScanNetworkJob.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x0041A340 (unverified)
nn::pia::local::LocalScanNetworkJob::LocalScanNetworkJob()
{
}

// 0x0041A3C8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalScanNetworkJob::~LocalScanNetworkJob()
{
}

// 0x007311CC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalScanNetworkJob::Trace(unsigned long long) const
{
}

// 0x0041A0E4 | fefates:bytes [tier B]
void nn::pia::local::LocalScanNetworkJob::WaitForCancel()
{
}

// 0x0041A290 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalScanNetworkJob::Startup(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*)
{
}

} // namespace local
} // namespace pia
} // namespace nn
