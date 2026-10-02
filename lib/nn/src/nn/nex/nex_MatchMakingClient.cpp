#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_MatchMakingClient.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::MatchMakingClient::MatchMakingClient()
{
}

// 0x0038735C slot 0x00 | fefates:bytes
nn::nex::MatchMakingClient::~MatchMakingClient()
{
}

// 0x00387088 slot 0x20 | fefates:bytes-fuzzy
void nn::nex::MatchMakingClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x00386FD4 | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::MatchMakingClient::EndParticipation(nn::nex::ProtocolCallContext*, unsigned, const nn::nex::String&)
{
}

// 0x00387030 | fefates:bytes [tier B]
void nn::nex::MatchMakingClient::UnregisterGathering(nn::nex::ProtocolCallContext*, unsigned int)
{
}

} // namespace nex
} // namespace nn
