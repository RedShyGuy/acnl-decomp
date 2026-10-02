#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_JobTerminate.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::JobTerminate::JobTerminate()
{
}

// 0x0035D4F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::JobTerminate::~JobTerminate()
{
}

// 0x0035D428 | fefates:bytes [tier B]
void nn::nex::JobTerminate::StepWaitOnPendingJobs()
{
}

} // namespace nex
} // namespace nn
