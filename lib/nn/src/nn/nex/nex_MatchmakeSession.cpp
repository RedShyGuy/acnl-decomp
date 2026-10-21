#include "nn/nex/nex__DDL_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSession.h"

namespace nn {
namespace nex {
// 0x0038262C | fefates:bytes [tier B]
nn::nex::MatchmakeSession::MatchmakeSession()
{
    Reset();
}

// 0x00382704 slot 0x00
// 0x003826F4 (deleting dtor)
nn::nex::MatchmakeSession::~MatchmakeSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0038254C | fefates:bytes [tier B]
void nn::nex::MatchmakeSession::Reset()
{
    Gathering::Reset();
    m_GameMode = 0;
    m_MatchmakeSystemType = 0;
    m_Attributes.clear();
    m_Attributes.ResizeInline(DEFAULT_ATTRIBUTE_COUNT, 0);
    m_IsOpenParticipation = true;
    m_ApplicationBuffer.clear();
    m_ParticipationCount = 0;
    m_ProgressScore = DEFAULT_PROGRESS_SCORE;
    m_Unknown0x6C = 0;
    m_StartedTime = *DateTime::s_pNever;
    m_MatchmakeParam = MatchmakeParam();
    SetMatchmakeSystemType(static_cast<MatchmakeSystemType>(0), 0);
    m_ReferGatheringId = 0;
}

// 0x003D6A1C | fefates:bytes [tier B]
void nn::nex::MatchmakeSession::SetMatchmakeSystemType(nn::nex::MatchmakeSystemType type, unsigned int argument)
{
    m_MatchmakeSystemType = type;
    switch (type) {
    case 1:
        SetParticipationPolicy(PARTICIPATION_POLICY_SYSTEM_TYPE_1, argument);
        break;
    case 2:
    case 3:
        SetParticipationPolicy(PARTICIPATION_POLICY_SYSTEM_TYPE_2_3, argument);
        break;
    default:
        SetParticipationPolicy(PARTICIPATION_POLICY_DEFAULT, 0);
        break;
    }
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
