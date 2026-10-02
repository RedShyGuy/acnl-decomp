#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_MatchMakingProtocolClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003B82EC (unverified)
nn::nex::MatchMakingProtocolClient::MatchMakingProtocolClient()
{
}

// 0x003B83E4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::MatchMakingProtocolClient::~MatchMakingProtocolClient()
{
}

// 0x003B7C5C slot 0x50 | fefates:bytes
void nn::nex::MatchMakingProtocolClient::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072D68C slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
void nn::nex::MatchMakingProtocolClient::CreateResponder() const
{
}

// 0x003B790C | fefates:bytes [tier B]
void nn::nex::MatchMakingProtocolClient::ProtoReturn_FindByOwner(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003B7E18 | fefates:bytes [tier B]
void nn::nex::MatchMakingProtocolClient::ProtoReturn_GetSessionURLs(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

} // namespace nex
} // namespace nn
