#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x0036D600 (unverified)
nn::nex::ServiceClient::ServiceClient()
{
}

// 0x0036D6F4 slot 0x00 | fefates:bytes
nn::nex::ServiceClient::~ServiceClient()
{
}

// 0x0036D484 slot 0x08 | virtual slot, introduced by nn::nex::ServiceClient
void nn::nex::ServiceClient::vf_0x08()
{
}

// 0x0036D528 slot 0x0C | slot vf_0x0C of nn::nex::ServiceClient
bool nn::nex::ServiceClient::Bind(nn::nex::Credentials*)
{
}

// 0x0036D5A4 slot 0x10 | slot vf_0x10 of nn::nex::ServiceClient
void nn::nex::ServiceClient::Unbind()
{
}

// 0x0072AA08 slot 0x14 | slot vf_0x14 of nn::nex::ServiceClient
void nn::nex::ServiceClient::IsConnected() const
{
}

// 0x0072AB28 slot 0x18 | fefates:bytes
void nn::nex::ServiceClient::IsFaulty() const
{
}

// 0x0036D524 slot 0x1C | slot vf_0x1C of nn::nex::ServiceClient
void nn::nex::ServiceClient::ConnectionStateHasChanged()
{
}

// 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
void nn::nex::ServiceClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x0072AAEC slot 0x24 | virtual slot, introduced by nn::nex::ServiceClient
void nn::nex::ServiceClient::vf_0x24()
{
}

// 0x0036D488 | fefates:bytes [tier B]
void nn::nex::ServiceClient::SetDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x00383520 | fefates:bytes [tier B]
void nn::nex::ServiceClient::RegisterProtocol(nn::nex::Protocol*)
{
}

// 0x0072AAA4 | fefates:bytes [tier B]
void nn::nex::ServiceClient::GetConnection(unsigned short) const
{
}

} // namespace nex
} // namespace nn
