#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045F164 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::transport::ProcessConnectionRequestJob::~ProcessConnectionRequestJob()
{
}

// 0x00736D0C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::transport::ProcessConnectionRequestJob::Trace(unsigned long long) const
{
}

// 0x0045E904 | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::WaitResponseAck()
{
}

// 0x0045E9EC | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::WaitInverseConnection()
{
}

// 0x0045EAF8 | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::SendConnectionResponse()
{
}

// 0x0045EC7C | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::StartupRelayConnection(nn::pia::transport::Station*, int, bool)
{
}

// 0x0045EE2C | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::ConnectToRequesterStation()
{
}

// 0x0045EE98 | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::Cleanup()
{
}

// 0x0045EEF4 | fefates:bytes [tier B]
void nn::pia::transport::ProcessConnectionRequestJob::Startup(nn::pia::transport::Station*, int, bool)
{
}

// 0x0045F0A4 | fefates:bytes [tier B]
nn::pia::transport::ProcessConnectionRequestJob::ProcessConnectionRequestJob()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
