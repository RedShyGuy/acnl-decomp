#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x00370E64 (unverified)
nn::nex::ClientProtocol::ClientProtocol()
{
}

// 0x00370EF0 slot 0x00 | fefates:bytes
nn::nex::ClientProtocol::~ClientProtocol()
{
}

// 0x0072AE64 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ClientProtocol::vf_0x08()
{
}

// 0x0072AE8C slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ClientProtocol::vf_0x0C()
{
}

// 0x00370E60 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ClientProtocol::vf_0x10()
{
}

// 0x003D5D8C slot 0x20 | fefates:bytes
void nn::nex::ClientProtocol::BeginInitialization()
{
}

// 0x003D5BD0 slot 0x28 | fefates:bytes
void nn::nex::ClientProtocol::BeginTermination()
{
}

// 0x0072AE5C slot 0x40 | slot vf_0x40 of nn::nex::ClientProtocol
void nn::nex::ClientProtocol::GetProtocolType() const
{
}

// 0x003D5DF8 slot 0x44 | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ClientProtocol::vf_0x44()
{
}

// 0x003D5A48 slot 0x48 | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ClientProtocol::vf_0x48()
{
}

// 0x0072E9E8 slot 0x4C | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ClientProtocol::vf_0x4C()
{
}

// 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
void nn::nex::ClientProtocol::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
void nn::nex::ClientProtocol::CreateResponder() const
{
}

// 0x00370DD4 slot 0x58 | fefates:bytes
void nn::nex::ClientProtocol::SetDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x00370A18 | fefates:bytes [tier B]
void nn::nex::ClientProtocol::ProcessResponse(nn::nex::Message*, nn::nex::EndPoint*)
{
}

// 0x00370C88 | fefates:bytes [tier B]
void nn::nex::ClientProtocol::SendOverLocalLoopback(nn::nex::ProtocolCallContext*, nn::nex::Message*)
{
}

// 0x00370E64 | fefates:bytes [tier B]
nn::nex::ClientProtocol::ClientProtocol(unsigned int)
{
}

} // namespace nex
} // namespace nn
