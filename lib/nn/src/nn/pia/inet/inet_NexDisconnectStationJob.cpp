#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/inet/inet_NexDisconnectStationJob.h"

namespace nn {
namespace pia {
namespace inet {
// ctor candidate(s) 0x00404058 (unverified)
nn::pia::inet::NexDisconnectStationJob::NexDisconnectStationJob()
{
}

// 0x004588E4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
nn::pia::inet::NexDisconnectStationJob::~NexDisconnectStationJob()
{
}

// 0x0072F518 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::inet::NexDisconnectStationJob::Trace(unsigned long long) const
{
}

// 0x00403EF8 slot 0x18 | virtual slot, introduced by nn::pia::transport::DisconnectStationJob
void nn::pia::inet::NexDisconnectStationJob::vf_0x18()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
