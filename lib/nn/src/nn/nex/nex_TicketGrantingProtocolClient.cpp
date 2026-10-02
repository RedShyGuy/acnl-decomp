#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_TicketGrantingProtocolClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003BF6DC (unverified)
nn::nex::TicketGrantingProtocolClient::TicketGrantingProtocolClient()
{
}

// 0x003BF758 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::TicketGrantingProtocolClient::~TicketGrantingProtocolClient()
{
}

// 0x003BF08C slot 0x50 | fefates:bytes
void nn::nex::TicketGrantingProtocolClient::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072DE60 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
void nn::nex::TicketGrantingProtocolClient::CreateResponder() const
{
}

// 0x003BEE84 | fefates:bytes [tier B]
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_RequestTicket(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003BF118 | fefates:bytes [tier B]
void nn::nex::TicketGrantingProtocolClient::ProtoReturn_LoginWithContext(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

} // namespace nex
} // namespace nn
