#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/pia/inet/inet_NexNatRelayInterface.h"

namespace nn {
namespace pia {
namespace inet {
// ctor address unknown
nn::pia::inet::NexNatRelayInterface::NexNatRelayInterface()
{
}

// 0x003873B8 slot 0x00 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x00()
{
}

// 0x00401560 slot 0x04 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x04()
{
}

// 0x00401480 slot 0x08 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x08()
{
}

// 0x004014E0 slot 0x0C | slot vf_0x0C of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::UnregisterRelayClient()
{
}

// 0x00401438 slot 0x10 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x10()
{
}

// 0x0040143C slot 0x14 | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>&, const nn::nex::StationURL&)
{
}

// 0x00401508 slot 0x18 | slot vf_0x18 of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::CheckCurrentPublicPort(nn::nex::CallContext*, const nn::nex::InetAddress&, bool)
{
}

// 0x00401558 slot 0x1C | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x1C()
{
}

// 0x004013C8 slot 0x20 | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::RequestProbe(const nn::nex::StationURL&)
{
}

// 0x0040150C slot 0x24 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x24()
{
}

// 0x0040148C slot 0x28 | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x28()
{
}

// 0x004014D8 slot 0x2C | virtual slot, introduced by nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x2C()
{
}

// 0x0040155C slot 0x30 | slot vf_0x30 of nn::nex::NATRelayInterface
void nn::pia::inet::NexNatRelayInterface::ReportNATTraversalResultDetail(const unsigned int&, const bool&, const nn::nex::NATTraversalResult&, unsigned int&)
{
}

// 0x004014EC slot 0x34 | fefates:bytes
void nn::pia::inet::NexNatRelayInterface::UpdateConnectionState(unsigned int, const nn::nex::StationURL&)
{
}

// 0x0040142C slot 0x38 | virtual slot, introduced by nn::pia::inet::NexNatRelayInterface
void nn::pia::inet::NexNatRelayInterface::vf_0x38()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
