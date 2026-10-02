#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/inet/inet_NexConnectStationJob.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00401380 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::inet::NexConnectStationJob::~NexConnectStationJob()
{
}

// 0x0072F174 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::inet::NexConnectStationJob::Trace(unsigned long long) const
{
}

// 0x004001F0 slot 0x18 | slot vf_0x18 of nn::pia::transport::ConnectStationJob
void nn::pia::inet::NexConnectStationJob::StartupImpl(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool)
{
}

// 0x00400140 slot 0x1C | fefates:callseq
void nn::pia::inet::NexConnectStationJob::CleanupImpl()
{
}

// 0x004004FC | fefates:bytes [tier B]
void nn::pia::inet::NexConnectStationJob::tryCurrentAddress()
{
}

// 0x004005CC | fefates:bytes [tier B]
void nn::pia::inet::NexConnectStationJob::testCurrentAddress()
{
}

// 0x00400A24 | fefates:bytes [tier B]
void nn::pia::inet::NexConnectStationJob::resolveCurrentAddress()
{
}

// 0x004012E4 | fefates:bytes [tier B]
nn::pia::inet::NexConnectStationJob::NexConnectStationJob()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
