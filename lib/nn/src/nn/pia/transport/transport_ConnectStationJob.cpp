#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045414C slot 0x00 | fefates:callgraph
nn::pia::transport::ConnectStationJob::~ConnectStationJob()
{
}

// 0x007356D4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::transport::ConnectStationJob::Trace(unsigned long long) const
{
}

// 0x004534A8 slot 0x18 | fefates:bytes
void nn::pia::transport::ConnectStationJob::StartupImpl(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool)
{
}

// 0x004534A4 slot 0x1C | slot vf_0x1C of nn::pia::transport::ConnectStationJob
void nn::pia::transport::ConnectStationJob::CleanupImpl()
{
}

// 0x00453604 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::WaitRequestAck()
{
}

// 0x00453898 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::ConnectionFailed()
{
}

// 0x004538FC | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::WaitForConnection()
{
}

// 0x00453A50 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::ConnectionSucceeded()
{
}

// 0x00453AF4 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::SendConnectionRequest()
{
}

// 0x00453CD0 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::StartupRelayConnection(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool, int)
{
}

// 0x00453D60 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::SendRelayConnectionRequest()
{
}

// 0x00453F18 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::SendConnectionRequestMessage()
{
}

// 0x00454024 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::Cleanup()
{
}

// 0x004540D0 | fefates:bytes [tier B]
void nn::pia::transport::ConnectStationJob::Startup(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool, int)
{
}

// 0x004540F4 | fefates:bytes [tier B]
nn::pia::transport::ConnectStationJob::ConnectStationJob()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
