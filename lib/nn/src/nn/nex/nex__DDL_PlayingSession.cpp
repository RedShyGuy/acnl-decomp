#include "nn/nex/nex__DDL_PlayingSession.h"
#include "nn/nex/nex_MatchmakeSession.h"

namespace nn {
namespace nex {
// 0x0039535C slot 0x00
// 0x00395324 (deleting dtor)
nn::nex::_DDL_PlayingSession::~_DDL_PlayingSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B208 (name is ours)
nn::nex::MatchmakeSession* nn::nex::_DDL_PlayingSession::GetMatchmakeSession() const
{
    Gathering* pGathering = m_Gathering.m_pObject;
    if (pGathering != nullptr && pGathering->IsAKindOf(String(L"MatchmakeSession"))) {
        return static_cast<MatchmakeSession*>(m_Gathering.m_pObject);
    }
    return nullptr;
}

} // namespace nex
} // namespace nn
