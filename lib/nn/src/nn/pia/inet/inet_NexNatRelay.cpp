#include "nn/pia/inet/inet_NatRelayInterface.h"
#include "nn/pia/inet/inet_NexNatRelay.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E4154 slot 0x00 | fefates:bytes
nn::pia::inet::NexNatRelay::~NexNatRelay()
{
}

// 0x003E3B50 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::RequestProbe(const nn::pia::transport::StationLocation&)
{
}

// 0x003E3B6C slot 0x0C | slot vf_0x0C of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::RelayProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationLocation&)
{
}

// 0x003E3EC4 slot 0x10 | fefates:bytes
void nn::pia::inet::NexNatRelay::ReportNatTraversalResult(const unsigned int&, const bool&, const unsigned int&)
{
}

// 0x003E3E48 slot 0x14 | fefates:bytes
void nn::pia::inet::NexNatRelay::ReportNatProperties(const unsigned int&, const unsigned int&, const unsigned int&)
{
}

// 0x003E3B3C slot 0x18 | slot vf_0x18 of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::SetLocalCID(unsigned int)
{
}

// 0x003E3B64 slot 0x1C | slot vf_0x1C of nn::pia::inet::NexNatRelay
void nn::pia::inet::NexNatRelay::AssociateProtocol(nn::pia::inet::NexNatTraversalProtocol*)
{
}

// 0x003E3F28 | fefates:bytes [tier B]
nn::pia::inet::NexNatRelay::NexNatRelay()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
