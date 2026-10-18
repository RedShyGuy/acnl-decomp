#include "nn/nex/nex_AutoMatchmakeParam.h"

namespace nn {
namespace nex {
// 0x00387440 (name is ours)
void nn::nex::AutoMatchmakeParam::SetSearchCriteria(const qList<MatchmakeSessionSearchCriteria>& criteria)
{
    m_SearchCriteria.AssignInline(criteria);
}

// 0x00387808
nn::nex::AutoMatchmakeParam::AutoMatchmakeParam()
{
    m_SourceMatchmakeSession = MatchmakeSession();
    m_AdditionalParticipants = qList<u32>();
    m_GatheringIdForParticipationCheck = 0;
    m_AutoMatchmakeOption = 0;
    m_JoinMessage = String();
    m_SearchCriteria = qList<MatchmakeSessionSearchCriteria>();
    m_TargetGatherings = qList<u32>();
    m_ParticipationCount = 1;
}

// 0x0038795C slot 0x00
// 0x0038794C (deleting dtor)
nn::nex::AutoMatchmakeParam::~AutoMatchmakeParam()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
