#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_AnyObjectHolder.h"

namespace nn {
namespace nex {
// 0x003B4C1C slot 0x00 | fefates:bytes
nn::nex::MatchmakeExtensionClient::~MatchmakeExtensionClient()
{
}

// 0x003B4A88 slot 0x0C | fefates:bytes
bool nn::nex::MatchmakeExtensionClient::Bind(nn::nex::Credentials*)
{
}

// 0x003B4B5C slot 0x10 | fefates:bytes
void nn::nex::MatchmakeExtensionClient::Unbind()
{
}

// 0x003B4B94 | fefates:bytes [tier B]
nn::nex::MatchmakeExtensionClient::MatchmakeExtensionClient()
{
}

// 0x003B47FC (name is ours)
bool nn::nex::MatchmakeExtensionClient::AutoMatchmakePostpone(nn::nex::ProtocolCallContext*, const nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>&, nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>*, const nn::nex::String&)
{
}

// 0x003B483C (name is ours)
bool nn::nex::MatchmakeExtensionClient::OpenParticipation(nn::nex::ProtocolCallContext*, u32)
{
}

// 0x003B4854 (name is ours)
bool nn::nex::MatchmakeExtensionClient::CloseParticipation(nn::nex::ProtocolCallContext*, u32)
{
}

// 0x003B486C (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateProgressScore(u32, u8)
{
}

// 0x003B49A0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::BrowseMatchmakeSession(nn::nex::ProtocolCallContext*, const nn::nex::MatchmakeSessionSearchCriteria&, const nn::nex::ResultRange&, nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >*)
{
}

// 0x003B49E4 (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateApplicationBuffer(nn::nex::ProtocolCallContext*, u32, const nn::nex::qVector<u8>&)
{
}

// 0x003B4A14 (name is ours)
bool nn::nex::MatchmakeExtensionClient::ModifyCurrentGameAttribute(nn::nex::ProtocolCallContext*, u32, u32, u32)
{
}

// 0x003B4A40 (name is ours)
bool nn::nex::MatchmakeExtensionClient::ClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext*, u32)
{
}

// 0x003B4A58 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext*, u32, nn::nex::String*)
{
}

// 0x003B4A70 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GetMatchmakeSession(nn::nex::ProtocolCallContext*, u32, nn::nex::MatchmakeSession*)
{
}

// 0x003C71B0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GetPlayingSession(nn::nex::ProtocolCallContext*, const nn::nex::qList<u32>&, nn::nex::qList<nn::nex::PlayingSession>*)
{
}

// 0x003C8A64 (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateMatchmakeSession(nn::nex::ProtocolCallContext*, const nn::nex::UpdateMatchmakeSessionParam&)
{
}

// 0x003C8CC0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::JoinMatchmakeSession(nn::nex::ProtocolCallContext*, const nn::nex::JoinMatchmakeSessionParam&, nn::nex::MatchmakeSession*)
{
}

// 0x003C94C8 (name is ours)
bool nn::nex::MatchmakeExtensionClient::AutoMatchmake(nn::nex::ProtocolCallContext*, const nn::nex::AutoMatchmakeParam&, nn::nex::MatchmakeSession*)
{
}

// 0x003C9630 (name is ours)
bool nn::nex::MatchmakeExtensionClient::CreateMatchmakeSession(nn::nex::ProtocolCallContext*, const nn::nex::CreateMatchmakeSessionParam&, nn::nex::MatchmakeSession*)
{
}


} // namespace nex
} // namespace nn
