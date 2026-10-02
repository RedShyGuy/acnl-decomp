#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatTraverser.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E69A8 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatTraverser
void nn::pia::inet::NatTraverser::vf_0x00()
{
}

// 0x003E68B4 slot 0x04 | fefates:bytes
nn::pia::inet::NatTraverser::~NatTraverser()
{
}

// 0x003E6714 slot 0x08 | slot vf_0x08 of nn::pia::inet::NatTraverser
void nn::pia::inet::NatTraverser::Startup(nn::pia::common::CallContext*)
{
}

// 0x003E6694 slot 0x0C | fefates:bytes
void nn::pia::inet::NatTraverser::Cleanup()
{
}

// 0x0072EFF0 slot 0x10 | virtual slot, introduced by nn::pia::inet::NatTraverser
void nn::pia::inet::NatTraverser::vf_0x10()
{
}

// 0x003E526C | fefates:bytes [tier B]
void nn::pia::inet::NatTraverser::CreateProtocols()
{
}

// 0x003E53F8 | fefates:bytes [tier B]
void nn::pia::inet::NatTraverser::StartNatTraversal()
{
}

// 0x003E5438 | fefates:bytes [tier B]
void nn::pia::inet::NatTraverser::IsStartupCancelled()
{
}

// 0x003E56D4 | fefates:bytes [tier B]
void nn::pia::inet::NatTraverser::UpdateNatServerAddress()
{
}

// 0x003E651C | fefates:bytes [tier B]
void nn::pia::inet::NatTraverser::CheckLatestStationLocation(nn::pia::transport::StationLocation&)
{
}

// 0x003E67D8 | fefates:bytes [tier B]
nn::pia::inet::NatTraverser::NatTraverser()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
