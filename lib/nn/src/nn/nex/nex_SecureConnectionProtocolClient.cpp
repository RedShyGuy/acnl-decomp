#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_SecureConnectionProtocolClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003C42BC (unverified)
nn::nex::SecureConnectionProtocolClient::SecureConnectionProtocolClient()
{
}

// 0x003C4338 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::SecureConnectionProtocolClient::~SecureConnectionProtocolClient()
{
}

// 0x003C3C20 slot 0x50 | fefates:bytes
void nn::nex::SecureConnectionProtocolClient::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072DEB0 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
void nn::nex::SecureConnectionProtocolClient::CreateResponder() const
{
}

// 0x003C3CAC | fefates:bytes [tier B]
void nn::nex::SecureConnectionProtocolClient::ProtoReturn_RequestConnectionData(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

} // namespace nex
} // namespace nn
