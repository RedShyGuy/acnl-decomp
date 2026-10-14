#include "nn/nex/nex__DDL_GameSession.h"
#include "nn/nex/nex_GameSession.h"

namespace nn {
namespace nex {
// 0x0072B814 slot 0x08 | fefates:bytes
nn::nex::Gathering* nn::nex::_DDL_GameSession::Clone() const
{
    // (a new one, nothing is copied)
    return new GameSession();
}

// 0x0072B7CC slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_GameSession::GetGatheringType() const
{
    return String(L"GameSession");
}

// 0x0072B7EC slot 0x10
bool nn::nex::_DDL_GameSession::IsA(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"GameSession");
}

// 0x0072B8B4 slot 0x14
bool nn::nex::_DDL_GameSession::IsAKindOf(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"GameSession") || Gathering::IsAKindOf(typeName);
}

// 0x0072B844 slot 0x18 | fefates:bytes
void nn::nex::_DDL_GameSession::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x00383F88 slot 0x1C | fefates:bytes
void nn::nex::_DDL_GameSession::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

} // namespace nex
} // namespace nn
