#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_ProcessJoinRequestJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x004406B8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::ProcessJoinRequestJob::~ProcessJoinRequestJob()
{
}

// 0x0073405C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::ProcessJoinRequestJob::Trace(unsigned long long) const
{
}

// 0x0043F848 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::InitialStep()
{
}

// 0x0043F96C | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::WaitResponseAck()
{
}

// 0x00440320 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::CancellationNotice(nn::pia::StationIndex)
{
}

// 0x00440334 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::SendDenyingJoinResponse()
{
}

// 0x00440408 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::Cleanup()
{
}

// 0x00440488 | fefates:bytes [tier B]
void nn::pia::session::ProcessJoinRequestJob::Startup()
{
}

// 0x00440520 | fefates:bytes-fuzzy [tier B]
nn::pia::session::ProcessJoinRequestJob::ProcessJoinRequestJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
