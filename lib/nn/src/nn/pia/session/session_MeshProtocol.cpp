#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/session/session_MeshProtocol.h"

namespace nn {
namespace pia {
namespace session {
// ctor candidate(s) 0x00430918 (unverified)
nn::pia::session::MeshProtocol::MeshProtocol()
{
}

// 0x0045FA54 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
nn::pia::session::MeshProtocol::~MeshProtocol()
{
}

// 0x00733868 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
void nn::pia::session::MeshProtocol::vf_0x08()
{
}

// 0x0073383C slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
void nn::pia::session::MeshProtocol::GetProtocolType() const
{
}

// 0x0045FA14 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
void nn::pia::session::MeshProtocol::Startup(nn::pia::StationIndex)
{
}

// 0x0042FB30 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
void nn::pia::session::MeshProtocol::Dispatch()
{
}

// 0x0042E8F8 slot 0x1C | fefates:callseq
void nn::pia::session::MeshProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x0042CE28 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::ParseHelper(const nn::pia::transport::ReceivedMessageAccessor&)
{
}

// 0x0042D7C4 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendGreeting(nn::pia::StationIndex)
{
}

// 0x0042DDB8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendDestroyMesh(nn::pia::StationIndex)
{
}

// 0x0042DEFC | fefates:bytes [tier B]
void nn::pia::session::MeshProtocol::ParseJoinRequest(const nn::pia::transport::ReceivedMessageAccessor&)
{
}

// 0x0042E3EC | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendLeaveResponse(nn::pia::transport::Station*)
{
}

// 0x0042E648 | fefates:bytes [tier B]
void nn::pia::session::MeshProtocol::MakeJoinRequestData(unsigned char*)
{
}

// 0x0042E7FC | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendMigrationFinish(bool)
{
}

// 0x0042ED90 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendConnectionReport(nn::pia::StationIndex, unsigned int, unsigned int, unsigned int*)
{
}

// 0x0042F518 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendRelayRouteDirections()
{
}

// 0x0042F68C | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendConnectionFailureNotice(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex, unsigned char, unsigned int)
{
}

// 0x0042F89C | fefates:bytes-fuzzy [tier B]
void nn::pia::session::MeshProtocol::SendMultiMigrationRankDecision(nn::pia::StationIndex, long long, unsigned int*)
{
}

// 0x00733844 | fefates:bytes [tier B]
void nn::pia::session::MeshProtocol::GetJoinRequestDataSize() const
{
}

} // namespace session
} // namespace pia
} // namespace nn
