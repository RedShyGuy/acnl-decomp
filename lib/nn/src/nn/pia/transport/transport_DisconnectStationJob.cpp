#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"

namespace nn {
namespace pia {
namespace transport {
// 0x004588E8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::transport::DisconnectStationJob::~DisconnectStationJob()
{
}

// 0x007360A8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::transport::DisconnectStationJob::Trace(unsigned long long) const
{
}

// 0x0045841C slot 0x18 | virtual slot, introduced by nn::pia::transport::DisconnectStationJob
void nn::pia::transport::DisconnectStationJob::vf_0x18()
{
}

// 0x00458274 slot 0x1C | fefates:bytes
void nn::pia::transport::DisconnectStationJob::StartupImpl(nn::pia::transport::Station*)
{
}

// 0x00458420 | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::WaitForDisconnection()
{
}

// 0x00458588 | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::DisconnectionSucceeded()
{
}

// 0x00458664 | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::SendDisconnectionRequest()
{
}

// 0x004587B8 | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::CutRouteOfRelayConnection()
{
}

// 0x0045885C | fefates:bytes [tier B]
void nn::pia::transport::DisconnectStationJob::Cleanup()
{
}

// 0x00458884 | fefates:bytes [tier B]
nn::pia::transport::DisconnectStationJob::DisconnectStationJob()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
