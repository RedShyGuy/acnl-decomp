#include "nn/nex/nex__DDL_UpdateMatchmakeSessionParam.h"
#include "nn/nex/nex_UpdateMatchmakeSessionParam.h"

namespace nn {
namespace nex {
namespace {
// the attributes of a session
const u32 ATTRIBUTE_COUNT = 6;
// the progress score of a new session
const u8 DEFAULT_PROGRESS_SCORE = 100;
} // namespace

// ctor address unknown
nn::nex::UpdateMatchmakeSessionParam::UpdateMatchmakeSessionParam()
{
}

// 0x003BD94C slot 0x00
// 0x003BD908 (deleting dtor)
nn::nex::UpdateMatchmakeSessionParam::~UpdateMatchmakeSessionParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BD65C (name is ours)
void nn::nex::UpdateMatchmakeSessionParam::SetApplicationBuffer(const qVector<u8>& buffer)
{
    m_ApplicationBuffer.AssignInline(buffer);
}

// 0x003BD81C (name is ours)
void nn::nex::UpdateMatchmakeSessionParam::Reset()
{
    m_GatheringId = 0;
    m_ModificationFlags = 0;
    u32 attributes[ATTRIBUTE_COUNT] = {};
    m_Attributes.assign(attributes, attributes + ATTRIBUTE_COUNT);
    m_IsOpenParticipation = true;
    SetApplicationBuffer(qVector<u8>());
    m_ProgressScore = DEFAULT_PROGRESS_SCORE;
    m_MatchmakeParam = MatchmakeParam();
    m_UserPassword = String();
    m_StartedTime = *DateTime::s_pNever;
    m_GameMode = 0;
    m_Description = String();
    m_MaxParticipants = 0;
    m_MinParticipants = 0;
}

} // namespace nex
} // namespace nn
