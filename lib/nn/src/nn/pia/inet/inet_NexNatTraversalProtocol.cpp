#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00407AF0 slot 0x00 | fefates:bytes
nn::pia::inet::NexNatTraversalProtocol::~NexNatTraversalProtocol()
{
}

// 0x0072EFE4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::inet::NexNatTraversalProtocol::vf_0x08()
{
}

// 0x0072F530 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::inet::NexNatTraversalProtocol::GetProtocolType() const
{
}

// 0x00407374 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
void nn::pia::inet::NexNatTraversalProtocol::Dispatch()
{
}

// 0x0072F538 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
void nn::pia::inet::NexNatTraversalProtocol::IsEnableProtocolFiltering() const
{
}

// 0x004054A4 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::IsTraversed(const nn::pia::transport::StationLocation&)
{
}

// 0x00405570 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::IsTraversed(unsigned int)
{
}

// 0x004055B0 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::RegisterRelay(nn::pia::inet::NatRelayInterface*)
{
}

// 0x00405A64 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::UnregisterRelay()
{
}

// 0x00405A98 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::addProbeRequest(const nn::pia::inet::NatProbeRequest&)
{
}

// 0x00405C54 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::sendDummyPacket(const nn::pia::transport::StationLocation&, unsigned char, unsigned char)
{
}

// 0x00405D54 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ResetProbeStatus(const nn::pia::transport::StationLocation&, bool)
{
}

// 0x00405E3C | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::SendProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationLocation&)
{
}

// 0x00405E64 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::getPeer2PeerPort(const nn::pia::transport::StationLocation&)
{
}

// 0x00406508 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::sendProbeRequests()
{
}

// 0x004067F0 | fefates:bytes-fuzzy [tier B]
void nn::pia::inet::NexNatTraversalProtocol::CleanupNatTraversal()
{
}

// 0x00406F28 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ReportNatProperties(const unsigned int&, const unsigned int&, const unsigned int&)
{
}

// 0x00407050 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::StartServerKeepAlive()
{
}

// 0x00407090 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::scheduleProbeRequest(const nn::pia::inet::NatProbeRequest&)
{
}

// 0x0040716C | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::GetLatestStationLocation(const nn::pia::transport::StationLocation&, nn::pia::transport::StationLocation*)
{
}

// 0x004071BC | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::ReportNatTraversalResult(const nn::pia::transport::StationLocation&, bool)
{
}

// 0x00407240 | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::findProbeRequestByConnectionId(unsigned int)
{
}

// 0x0040727C | fefates:bytes [tier B]
void nn::pia::inet::NexNatTraversalProtocol::eraseProbeRequestByConnectionId(unsigned int)
{
}

// 0x00407808 | fefates:bytes [tier B]
nn::pia::inet::NexNatTraversalProtocol::NexNatTraversalProtocol()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
