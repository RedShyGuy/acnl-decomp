#include "nn/nex/nex__DDL_GameSession.h"
#include "nn/nex/nex_GameSession.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::GameSession::GameSession()
{
}

// 0x003576DC slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
nn::nex::GameSession::~GameSession()
{
}

// 0x0072B814 slot 0x08 | fefates:bytes
void nn::nex::GameSession::Clone() const
{
}

// 0x0072B7CC slot 0x0C | mk7dlp:bytes
void nn::nex::GameSession::GetGatheringType() const
{
}

// 0x0072B7EC slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
void nn::nex::GameSession::vf_0x10()
{
}

// 0x0072B8B4 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
void nn::nex::GameSession::vf_0x14()
{
}

// 0x0072B844 slot 0x18 | fefates:bytes
void nn::nex::GameSession::StreamIn(nn::nex::Message*) const
{
}

// 0x00383F88 slot 0x1C | fefates:bytes
void nn::nex::GameSession::StreamOut(nn::nex::Message*)
{
}

} // namespace nex
} // namespace nn
