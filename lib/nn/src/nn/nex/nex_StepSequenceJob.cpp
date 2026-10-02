#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::StepSequenceJob::StepSequenceJob()
{
}

// 0x0037DC4C slot 0x00 | fefates:callgraph
nn::nex::StepSequenceJob::~StepSequenceJob()
{
}

// 0x0037DB18 slot 0x0C | fefates:bytes
void nn::nex::StepSequenceJob::Execute()
{
}

// 0x0037D958 slot 0x14 | slot vf_0x14 of nn::nex::Job
void nn::nex::StepSequenceJob::AddActivity(const wchar_t*)
{
}

// 0x0072B6E0 slot 0x18 | virtual slot, introduced by nn::nex::Job
void nn::nex::StepSequenceJob::vf_0x18()
{
}

// 0x0037D95C slot 0x30 | slot vf_0x30 of nn::nex::StepSequenceJob
void nn::nex::StepSequenceJob::CheckExceptions()
{
}

// 0x0037D960 | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::ProcessCallResult(nn::nex::StepSequenceJob::Step*)
{
}

// 0x0037DA0C | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::ResumeOnCallCompletion(nn::nex::CallContext*, nn::nex::StepSequenceJob::Step*)
{
}

// 0x0037DB88 | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::SetStep(const nn::nex::StepSequenceJob::Step&)
{
}

// 0x0037DBC8 | fefates:bytes [tier B]
nn::nex::StepSequenceJob::StepSequenceJob(nn::nex::Job::JobType, const nn::nex::DebugString&)
{
}

} // namespace nex
} // namespace nn
