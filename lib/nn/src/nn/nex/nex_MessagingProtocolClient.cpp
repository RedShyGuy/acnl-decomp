#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_MessagingProtocolClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003B11F8 (unverified)
nn::nex::MessagingProtocolClient::MessagingProtocolClient()
{
}

// 0x003B12F0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::MessagingProtocolClient::~MessagingProtocolClient()
{
}

// 0x003B1174 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
void nn::nex::MessagingProtocolClient::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072D25C slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
void nn::nex::MessagingProtocolClient::CreateResponder() const
{
}

} // namespace nex
} // namespace nn
