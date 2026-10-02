#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/session/session_SessionProtocol.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x00436C64 (unverified)
nn::pia::session::SessionProtocol::SessionProtocol()
{
}

// 0x00436CA4 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::session::SessionProtocol::~SessionProtocol()
{
}

// 0x007338E4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::vf_0x08()
{
}

// 0x007338DC slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::GetProtocolType() const
{
}

// 0x0043687C slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x00436810 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::Cleanup()
{
}

// 0x004368CC slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::Dispatch()
{
}

// 0x004350EC slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
void nn::pia::session::SessionProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

} // namespace session
} // namespace pia
} // namespace nn
