#include "nn/nex/nex_CreateMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// 0x003BCBF0
nn::nex::CreateMatchmakeSessionParam::CreateMatchmakeSessionParam()
{
    m_SourceMatchmakeSession = MatchmakeSession();
    m_AdditionalParticipants = qList<u32>();
    m_GatheringIdForParticipationCheck = 0;
    m_CreateMatchmakeSessionOption = 0;
    m_JoinMessage = String();
    m_ParticipationCount = 1;
}

// 0x003BCD08 slot 0x00
// 0x003BCCF8 (deleting dtor)
nn::nex::CreateMatchmakeSessionParam::~CreateMatchmakeSessionParam()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
