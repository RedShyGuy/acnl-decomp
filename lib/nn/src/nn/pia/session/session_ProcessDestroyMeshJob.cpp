#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_ProcessDestroyMeshJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x0043F844 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::ProcessDestroyMeshJob::~ProcessDestroyMeshJob()
{
}

// 0x00734058 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::ProcessDestroyMeshJob::Trace(unsigned long long) const
{
}

// 0x0043F630 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessDestroyMeshJob::Startup()
{
}

// 0x0043F7FC | fefates:bytes [tier B]
nn::pia::session::ProcessDestroyMeshJob::ProcessDestroyMeshJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
