#include "nn/nex/nex__DDL_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"

namespace nn {
namespace nex {
// 0x003C3034 slot 0x00 | fefates:callseq
void nn::nex::MatchmakeSessionSearchCriteria::vf_0x00()
{
}

// 0x003C2FC0 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_MatchmakeSessionSearchCriteria
void nn::nex::MatchmakeSessionSearchCriteria::vf_0x04()
{
}

// 0x003C2B8C | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::SetMaxParticipants(u16, u16)
{
}

// 0x003C2BAC | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::SetMinParticipants(u16, u16)
{
}

// 0x003C2A58 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetGameMode(u32)
{
}

// 0x003C2A6C (name is ours)
bool nn::nex::MatchmakeSessionSearchCriteria::SetAttribute(u32, const qVector<u32>&)
{
}

// 0x003C2B34 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetAttribute(u32, u32)
{
}

// 0x003C2B4C (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetVacantOnly(bool)
{
}

// 0x003C2B5C (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetAttributeRange(u32, u32, u32)
{
}

// 0x003C2B78 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetMatchmakeSystemType(u32)
{
}

// 0x00395238 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetMatchmakeParam(u32, const MatchmakeParam&)
{
}

// 0x003C2BCC | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::Reset()
{
}

// 0x003C2F18 | fefates:bytes [tier B]
nn::nex::MatchmakeSessionSearchCriteria::MatchmakeSessionSearchCriteria()
{
}

} // namespace nex
} // namespace nn
