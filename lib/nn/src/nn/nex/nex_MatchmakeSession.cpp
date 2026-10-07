#include "nn/nex/nex__DDL_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSession.h"

namespace nn {
namespace nex {
// 0x00382704 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
nn::nex::MatchmakeSession::~MatchmakeSession()
{
}

// 0x0038254C | fefates:bytes [tier B]
void nn::nex::MatchmakeSession::Reset()
{
}

// 0x0038262C | fefates:bytes [tier B]
nn::nex::MatchmakeSession::MatchmakeSession()
{
}

// 0x003D6A1C | fefates:bytes [tier B]
void nn::nex::MatchmakeSession::SetMatchmakeSystemType(nn::nex::MatchmakeSystemType, unsigned int)
{
}

// 0x0038251C (name is ours)
void nn::nex::MatchmakeSession::SetAttribute(u32 index, u32 value)
{
    m_Attributes[index] = value;
}

// 0x00382528 (name is ours)
bool nn::nex::MatchmakeSession::SetProgressScore(u8 score)
{
    if (score > 100) {
        return false;
    }
    m_ProgressScore = score;
    return true;
}

// 0x0038253C (name is ours)
void nn::nex::MatchmakeSession::SetApplicationBuffer(const qVector<u8>& buffer)
{
    m_ApplicationBuffer = buffer;
}

} // namespace nex
} // namespace nn
