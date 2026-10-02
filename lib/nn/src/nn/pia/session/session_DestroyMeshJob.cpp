#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_DestroyMeshJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x0043159C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::DestroyMeshJob::~DestroyMeshJob()
{
}

// 0x00733870 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::DestroyMeshJob::Trace(unsigned long long) const
{
}

// 0x004312D4 | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::AssociateSystemWith(nn::pia::common::CallContext*)
{
}

// 0x00431318 | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::WaitDestroyResponse()
{
}

// 0x004313D4 | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::ReceiveDestroyResponse(nn::pia::StationIndex)
{
}

// 0x004313EC | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::Cleanup()
{
}

// 0x00431444 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::DestroyMeshJob::Startup(nn::pia::common::CallContext*, bool)
{
}

// 0x00431540 | fefates:bytes [tier B]
nn::pia::session::DestroyMeshJob::DestroyMeshJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
