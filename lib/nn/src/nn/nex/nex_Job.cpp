#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003CC778 (unverified)
nn::nex::Job::Job()
{
}

// 0x003CC80C slot 0x00 | fefates:callgraph
nn::nex::Job::~Job()
{
}

// 0x003CC644 slot 0x08 | slot vf_0x08 of nn::nex::Job
void nn::nex::Job::DecoratedExecute()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::Job::Execute()
{
}

// 0x003CC704 slot 0x10 | slot vf_0x10 of nn::nex::Job
void nn::nex::Job::TestSuspendedJobState()
{
}

// 0x003CC588 slot 0x14 | slot vf_0x14 of nn::nex::Job
void nn::nex::Job::AddActivity(const wchar_t*)
{
}

// 0x0072DF28 slot 0x18 | virtual slot, introduced by nn::nex::Job
void nn::nex::Job::vf_0x18()
{
}

// 0x003CC710 slot 0x1C | slot vf_0x1C of nn::nex::Job
void nn::nex::Job::SetDefaultPostExecutionState()
{
}

// 0x003CC708 slot 0x20 | slot vf_0x20 of nn::nex::Job
void nn::nex::Job::SkipWaitDelayAtTermination()
{
}

// 0x003CC774 slot 0x24 | slot vf_0x24 of nn::nex::Job
void nn::nex::Job::CancelJob()
{
}

// 0x003CC700 slot 0x28 | virtual slot, introduced by nn::nex::Job
void nn::nex::Job::vf_0x28()
{
}

// 0x003CC638 slot 0x2C | virtual slot, introduced by nn::nex::Job
void nn::nex::Job::vf_0x2C()
{
}

// 0x003CC594 | fefates:bytes [tier B]
void nn::nex::Job::SetToWaiting(int)
{
}

// 0x003CC630 | mk7dlp:callseq-callee [tier A]
void nn::nex::Job::SetToComplete()
{
}

// 0x003CC750 | fefates:bytes [tier B]
void nn::nex::Job::SetState(nn::nex::Job::State)
{
}

} // namespace nex
} // namespace nn
