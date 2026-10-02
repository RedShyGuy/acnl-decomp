#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace common {
// 0x004275A0 slot 0x00 | fefates:callgraph
nn::pia::common::StepSequenceJob::~StepSequenceJob()
{
}

// 0x00427514 slot 0x08 | fefates:bytes
void nn::pia::common::StepSequenceJob::Reset(bool)
{
}

// 0x004273C0 slot 0x0C | fefates:bytes
void nn::pia::common::StepSequenceJob::ExecuteCore()
{
}

// 0x00427434 slot 0x10 | slot vf_0x10 of nn::pia::common::StepSequenceJob
void nn::pia::common::StepSequenceJob::CancelCleanup()
{
}

// 0x00731AF4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::common::StepSequenceJob::Trace(unsigned long long) const
{
}

// 0x0042744C | fefates:bytes [tier B]
void nn::pia::common::StepSequenceJob::WaitForCompletion(unsigned int)
{
}

// 0x0042752C | fefates:bytes [tier B]
nn::pia::common::StepSequenceJob::StepSequenceJob()
{
}

} // namespace common
} // namespace pia
} // namespace nn
