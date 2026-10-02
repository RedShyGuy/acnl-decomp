#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalSendMessageJob.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x0041A660 (unverified)
nn::pia::local::LocalSendMessageJob::LocalSendMessageJob()
{
}

// 0x0041A750 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::local::LocalSendMessageJob::~LocalSendMessageJob()
{
}

// 0x007311D0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::local::LocalSendMessageJob::Trace(unsigned long long) const
{
}

// 0x0041A404 | fefates:bytes [tier B]
void nn::pia::local::LocalSendMessageJob::ReceiveAck(unsigned char, unsigned int)
{
}

// 0x0041A5B4 | fefates:bytes [tier B]
void nn::pia::local::LocalSendMessageJob::Startup()
{
}

} // namespace local
} // namespace pia
} // namespace nn
