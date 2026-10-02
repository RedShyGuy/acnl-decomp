#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045FA58 slot 0x00 | fefates:callgraph
nn::pia::transport::Protocol::~Protocol()
{
}

// 0x00736E80 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::transport::Protocol::vf_0x08()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::pia::transport::Protocol::GetProtocolType() const
{
}

// 0x0045FA18 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::Startup(nn::pia::StationIndex)
{
}

// 0x0045F9D8 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::Cleanup()
{
}

// 0x0045FA20 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::Dispatch()
{
}

// 0x0045F9D0 slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x00736E78 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
void nn::pia::transport::Protocol::IsEnableProtocolFiltering() const
{
}

// 0x0045F9DC | fefates:bytes [tier B]
void nn::pia::transport::Protocol::SetPort(unsigned short)
{
}

// 0x0045FA28 | fefates:bytes [tier B]
nn::pia::transport::Protocol::Protocol()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
