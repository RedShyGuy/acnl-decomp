#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_ReliableProtocol.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045311C slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::transport::ReliableProtocol::~ReliableProtocol()
{
}

// 0x007356D0 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::transport::ReliableProtocol::vf_0x08()
{
}

// 0x00735668 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::transport::ReliableProtocol::GetProtocolType() const
{
}

// 0x00452CEC slot 0x10 | fefates:bytes-fuzzy
void nn::pia::transport::ReliableProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x00452C18 slot 0x14 | fefates:bytes
void nn::pia::transport::ReliableProtocol::Cleanup()
{
}

// 0x00452D4C slot 0x18 | fefates:callseq
void nn::pia::transport::ReliableProtocol::Dispatch()
{
}

// 0x00452924 slot 0x1C | fefates:bytes
void nn::pia::transport::ReliableProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x00452664 | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::Initialize(unsigned int, unsigned int)
{
}

// 0x004527C0 | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::ReceiveImpl(nn::pia::StationIndex*, void*, unsigned int*, unsigned int, bool)
{
}

// 0x00452BCC | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::Send(nn::pia::StationId, const void*, unsigned int)
{
}

// 0x00452C84 | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::Receive(nn::pia::StationId*, void*, unsigned int*, unsigned int)
{
}

// 0x00452F48 | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::Finalize()
{
}

// 0x00452FC4 | fefates:bytes [tier B]
nn::pia::transport::ReliableProtocol::ReliableProtocol()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
