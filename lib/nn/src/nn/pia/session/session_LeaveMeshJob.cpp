#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"

namespace nn {
namespace pia {
namespace session {
// 0x0042CAFC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::session::LeaveMeshJob::~LeaveMeshJob()
{
}

// 0x00733838 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::session::LeaveMeshJob::Trace(unsigned long long) const
{
}

// 0x0042C4C0 | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::SendLeaveRequest()
{
}

// 0x0042C544 | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::WaitLeaveResponse()
{
}

// 0x0042C660 | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::WaitLeavingProcess()
{
}

// 0x0042C744 | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::RegisterExtraCallback(nn::pia::common::CallContext*)
{
}

// 0x0042C76C | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::StartDisconnectStations()
{
}

// 0x0042C8EC | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::Cleanup()
{
}

// 0x0042CAA8 | fefates:bytes [tier B]
nn::pia::session::LeaveMeshJob::LeaveMeshJob()
{
}

} // namespace session
} // namespace pia
} // namespace nn
