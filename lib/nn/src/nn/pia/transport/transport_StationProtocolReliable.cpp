#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_StationProtocolReliable.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045D454 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::transport::StationProtocolReliable::~StationProtocolReliable()
{
}

// 0x00736778 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::transport::StationProtocolReliable::vf_0x08()
{
}

// 0x00736770 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocolReliable::GetProtocolType() const
{
}

// 0x0045D2DC slot 0x10 | fefates:bytes
void nn::pia::transport::StationProtocolReliable::Startup(nn::pia::StationIndex)
{
}

// 0x0045D1A0 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocolReliable::Cleanup()
{
}

// 0x0045D2F4 slot 0x18 | fefates:bytes
void nn::pia::transport::StationProtocolReliable::Dispatch()
{
}

// 0x0045D0E8 slot 0x1C | fefates:bytes
void nn::pia::transport::StationProtocolReliable::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x0045D1AC | fefates:bytes [tier B]
void nn::pia::transport::StationProtocolReliable::Receive(nn::pia::StationIndex, unsigned int, unsigned char*, unsigned int*, nn::pia::common::StationAddress*)
{
}

// 0x0045D388 | fefates:bytes [tier B]
nn::pia::transport::StationProtocolReliable::StationProtocolReliable()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
