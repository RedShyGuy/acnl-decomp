#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_SecureConnectionClient.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::SecureConnectionClient::SecureConnectionClient()
{
}

// 0x0039DDF4 slot 0x00 | slot vf_0x00 of nn::nex::ServiceClient
nn::nex::SecureConnectionClient::~SecureConnectionClient()
{
}

// 0x0039DDBC slot 0x20 | slot vf_0x20 of nn::nex::ServiceClient
void nn::nex::SecureConnectionClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x0039DB10 | fefates:bytes [tier B]
void nn::nex::SecureConnectionClient::ReplaceURL(nn::nex::ProtocolCallContext*, const nn::nex::StationURL&, const nn::nex::StationURL&)
{
}

// 0x0039DB7C | fefates:bytes [tier B]
void nn::nex::SecureConnectionClient::SendReport(unsigned int, const void*, unsigned int)
{
}

} // namespace nex
} // namespace nn
