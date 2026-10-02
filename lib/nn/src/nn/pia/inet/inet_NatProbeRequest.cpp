#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatProbeRequest.h"

namespace nn {
namespace pia {
namespace inet {
// ctor candidate(s) 0x003E7E9C, 0x003E7EDC, 0x00405A98, 0x00406558 (unverified)
nn::pia::inet::NatProbeRequest::NatProbeRequest()
{
}

// 0x003E7F60 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
void nn::pia::inet::NatProbeRequest::vf_0x00()
{
}

// 0x003E7F38 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
void nn::pia::inet::NatProbeRequest::vf_0x04()
{
}

// 0x0072F100 slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
void nn::pia::inet::NatProbeRequest::vf_0x08()
{
}

// 0x003E7E9C | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequest::NatProbeRequest(const nn::pia::transport::StationLocation&)
{
}

// 0x003E7EDC | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequest::NatProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationConnectionInfo&, bool, nn::pia::common::CallContext*)
{
}

} // namespace inet
} // namespace pia
} // namespace nn
