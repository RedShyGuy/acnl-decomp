#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_AnyObjectHolder.h"

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
bool nn::nex::MatchMakingClient::EndParticipation(nn::nex::ProtocolCallContext*, unsigned, const nn::nex::String&)
{
}

// 0x00387030 | fefates:bytes [tier B]
bool nn::nex::MatchMakingClient::UnregisterGathering(nn::nex::ProtocolCallContext*, unsigned int)
{
}

// 0x00386FBC (name after MatchMakingProtocolClient::ProtoReturn_GetSessionURLs)
bool nn::nex::MatchMakingClient::GetSessionURLs(nn::nex::ProtocolCallContext*, u32, nn::nex::qList<nn::nex::StationURL>*)
{
}

// 0x00387014 (name after pia::inet::NexProcessHostMigrationJob::CallUpdateSessionHost)
bool nn::nex::MatchMakingClient::UpdateSessionHost(nn::nex::ProtocolCallContext*, u32, bool)
{
}

// 0x00386F98 (name after the method name of the protocol)
bool nn::nex::MatchMakingClient::FindByOwner(nn::nex::ProtocolCallContext*, u32, const nn::nex::ResultRange&,
                                             nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >*)
{
}

// 0x0038704C (name after the method name of the protocol)
bool nn::nex::MatchMakingClient::MigrateGatheringOwnership(nn::nex::ProtocolCallContext*, u32, const nn::nex::qList<u32>&)
{
}

// 0x003870AC (name after the method name of the protocol)
bool nn::nex::MatchMakingClient::FindBySingleID(nn::nex::ProtocolCallContext*, u32, nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>*)
{
}

} // namespace nex
} // namespace nn
