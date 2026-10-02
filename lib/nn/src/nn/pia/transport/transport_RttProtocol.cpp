#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_RttProtocol.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0044DAA8 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::transport::RttProtocol::~RttProtocol()
{
}

// 0x00734B78 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::transport::RttProtocol::vf_0x08()
{
}

// 0x00734B70 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::transport::RttProtocol::GetProtocolType() const
{
}

// 0x0044D72C slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::transport::RttProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x0044D6D0 slot 0x14 | fefates:bytes
void nn::pia::transport::RttProtocol::Cleanup()
{
}

// 0x0044D778 slot 0x18 | fefates:callseq
void nn::pia::transport::RttProtocol::Dispatch()
{
}

// 0x0044D4A4 slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
void nn::pia::transport::RttProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x0044DA2C | fefates:bytes [tier B]
void nn::pia::transport::RttProtocol::Finalize()
{
}

// 0x0044DA6C | fefates:bytes [tier B]
nn::pia::transport::RttProtocol::RttProtocol()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
