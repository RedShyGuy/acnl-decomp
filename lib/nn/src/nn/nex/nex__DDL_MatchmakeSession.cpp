#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex__DDL_MatchmakeSession.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::_DDL_MatchmakeSession::_DDL_MatchmakeSession()
{
}

// 0x0039BC30 slot 0x00 | fefates:bytes
nn::nex::_DDL_MatchmakeSession::~_DDL_MatchmakeSession()
{
}

// 0x0072D008 slot 0x08 | fefates:callseq
void nn::nex::_DDL_MatchmakeSession::Clone() const
{
}

// 0x0072CFA8 slot 0x0C | mk7dlp:bytes
void nn::nex::_DDL_MatchmakeSession::GetGatheringType() const
{
}

// 0x0072CFD4 slot 0x10
bool nn::nex::_DDL_MatchmakeSession::IsA(const String&) const
{
}

// 0x0072D028 slot 0x14
bool nn::nex::_DDL_MatchmakeSession::IsAKindOf(const String&) const
{
}

// 0x0039B71C slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
void nn::nex::_DDL_MatchmakeSession::StreamIn(nn::nex::Message*) const
{
}

// 0x0039B9B8 slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
void nn::nex::_DDL_MatchmakeSession::StreamOut(nn::nex::Message*)
{
}

// 0x0039B9C8 | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeSession::Extract(nn::nex::Message*, nn::nex::_DDL_MatchmakeSession*)
{
}

// 0x0039BC74 (name is ours)
void nn::nex::_DDL_MatchmakeSession::operator=(const _DDL_MatchmakeSession&)
{
}

// 0x0072B750 (name is ours)
u32 nn::nex::_DDL_MatchmakeSession::GetAttribute(u32 index) const
{
    return m_Attributes[index];
}

} // namespace nex
} // namespace nn
