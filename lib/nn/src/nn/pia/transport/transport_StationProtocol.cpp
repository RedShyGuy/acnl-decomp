#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_StationProtocol.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00452660 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::transport::StationProtocol::~StationProtocol()
{
}

// 0x00735664 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::transport::StationProtocol::vf_0x08()
{
}

// 0x0073551C slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocol::GetProtocolType() const
{
}

// 0x00452440 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x00452324 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocol::Cleanup()
{
}

// 0x00452450 slot 0x18 | fefates:bytes
void nn::pia::transport::StationProtocol::Dispatch()
{
}

// 0x00735524 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
void nn::pia::transport::StationProtocol::IsEnableProtocolFiltering() const
{
}

// 0x00451458 | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::StationProtocol::ParseHelper(const nn::pia::transport::ReceivedMessageAccessor&)
{
}

// 0x004515E4 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDisconnectionRequest(nn::pia::StationIndex, bool)
{
}

// 0x0045164C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDisconnectionRequest(const nn::pia::common::StationAddress&)
{
}

// 0x004516B0 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::ParseDisconnectionRequest(const nn::pia::transport::ReceivedMessageAccessor&)
{
}

// 0x0045229C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendDenyingConnectionResponse(const nn::pia::common::StationAddress&, unsigned char)
{
}

// 0x00452330 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendAck(unsigned int, nn::pia::StationIndex)
{
}

// 0x004523B8 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::SendAck(unsigned int, const nn::pia::common::StationAddress&)
{
}

// 0x0045262C | fefates:bytes [tier B]
nn::pia::transport::StationProtocol::StationProtocol()
{
}

// 0x0073552C | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::MakeConnectionRequestData(unsigned char*, unsigned char, bool, bool) const
{
}

// 0x00735598 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::MakeConnectionResponseData(unsigned char*, bool) const
{
}

// 0x00735640 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocol::GetConnectionRequestDataSize() const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
