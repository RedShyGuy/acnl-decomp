#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/session/session_SyncClockProtocol.h"

namespace nn {
namespace pia {
namespace session {
// 0x004398C8 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::session::SyncClockProtocol::~SyncClockProtocol()
{
}

// 0x00733908 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::session::SyncClockProtocol::vf_0x08()
{
}

// 0x00733900 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::session::SyncClockProtocol::GetProtocolType() const
{
}

// 0x004394B4 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::session::SyncClockProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x0043949C slot 0x14 | fefates:bytes
void nn::pia::session::SyncClockProtocol::Cleanup()
{
}

// 0x004395DC slot 0x18 | fefates:callseq
void nn::pia::session::SyncClockProtocol::Dispatch()
{
}

// 0x004393CC slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
void nn::pia::session::SyncClockProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x00439028 | fefates:bytes [tier B]
void nn::pia::session::SyncClockProtocol::processByClient(const nn::pia::transport::ProtocolMessageReader*)
{
}

// 0x00439854 | fefates:bytes [tier B]
nn::pia::session::SyncClockProtocol::SyncClockProtocol()
{
}

} // namespace session
} // namespace pia
} // namespace nn
