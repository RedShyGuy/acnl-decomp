#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_PersistentGathering.h"
#include "nn/nex/nex_ProtocolCallContext.h"

namespace nn {
namespace nex {
namespace {
// the progress score is above 100
const int RESULT_INVALID_PROGRESS_SCORE = static_cast<int>(0x8001000A);
// the progress score was updated less than 30 seconds ago
const int RESULT_PROGRESS_SCORE_TOO_EARLY = static_cast<int>(0x80010011);

// (inline) the prototype is deleted if the adapter does not take it
inline void RegisterPrototypeInline(Gathering* pPrototype)
{
    if (!MatchMakingClient::RegisterPrototype(pPrototype) && pPrototype != nullptr) {
        delete pPrototype;
    }
}
} // namespace

// 0x003B47FC (name is ours)
bool nn::nex::MatchmakeExtensionClient::AutoMatchmakePostpone(nn::nex::ProtocolCallContext* pContext, const nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>& gathering,
                                                             nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pJoinedGathering, const nn::nex::String& message)
{
    return m_Extension.CallAutoMatchmakePostpone(pContext, gathering, pJoinedGathering, message);
}

// 0x003B4814 (name is ours)
bool nn::nex::MatchmakeExtensionClient::Method28(nn::nex::ProtocolCallContext* pContext, u32 value1, const nn::nex::String& text, u32 value2)
{
    return m_Extension.CallMethod28(pContext, value1, text, value2);
}

// 0x003B483C (name is ours)
bool nn::nex::MatchmakeExtensionClient::OpenParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId)
{
    return m_Extension.CallOpenParticipation(pContext, gatheringId);
}

// 0x003B4854 (name is ours)
bool nn::nex::MatchmakeExtensionClient::CloseParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId)
{
    return m_Extension.CallCloseParticipation(pContext, gatheringId);
}

// 0x003B486C (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateProgressScore(u32 gatheringId, u8 score)
{
    ProtocolCallContext* pContext = new ProtocolCallContext();
    pContext->SetFlag(CallContext::FLAG_RELEASE_ON_COMPLETION);
    u32 id = gatheringId;
    u8 value = score;
    Time now = Time::GetTime();
    if (value > MAX_PROGRESS_SCORE) {
        pContext->SetCallInProgressState();
        pContext->SignalFailure(qResult(RESULT_INVALID_PROGRESS_SCORE));
        return false;
    }
    if (value != 0 && value != MAX_PROGRESS_SCORE && m_LastProgressScoreTime.m_Value != 0
        && static_cast<s64>(now.m_Value - m_LastProgressScoreTime.m_Value) < PROGRESS_SCORE_INTERVAL) {
        pContext->SetCallInProgressState();
        pContext->SignalFailure(qResult(RESULT_PROGRESS_SCORE_TOO_EARLY));
        return false;
    }
    m_LastProgressScoreTime = now;
    return m_Extension.CallUpdateProgressScore(pContext, id, value);
}

// 0x003B49A0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::BrowseMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::MatchmakeSessionSearchCriteria& criteria,
                                                              const nn::nex::ResultRange& range,
                                                              nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings)
{
    return m_Extension.CallBrowseMatchmakeSession(pContext, criteria, range, pGatherings);
}

// 0x003B49B8 (name is ours)
bool nn::nex::MatchmakeExtensionClient::Method9(nn::nex::ProtocolCallContext* pContext, u32 value1, u32 value2, u32 value3, const nn::nex::String& text)
{
    return m_Extension.CallMethod9(pContext, value1, value2, value3, text);
}

// 0x003B49E4 (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateApplicationBuffer(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qVector<u8>& buffer)
{
    return m_Extension.CallUpdateApplicationBuffer(pContext, gatheringId, buffer);
}

// 0x003B49FC (name is ours)
bool nn::nex::MatchmakeExtensionClient::Method10(nn::nex::ProtocolCallContext* pContext, nn::nex::qList<nn::nex::NotificationEvent>* pEvents, u32 value)
{
    return m_Extension.CallMethod10(pContext, pEvents, value);
}

// 0x003B4A14 (name is ours)
bool nn::nex::MatchmakeExtensionClient::ModifyCurrentGameAttribute(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, u32 index, u32 value)
{
    return m_Extension.CallModifyCurrentGameAttribute(pContext, gatheringId, index + 1, value);
}

// 0x003B4A40 (name is ours)
bool nn::nex::MatchmakeExtensionClient::ClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId)
{
    return m_Extension.CallClearMatchmakeSessionSystemPassword(pContext, gatheringId);
}

// 0x003B4A58 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::String* pPassword)
{
    return m_Extension.CallGenerateMatchmakeSessionSystemPassword(pContext, gatheringId, pPassword);
}

// 0x003B4A70 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GetMatchmakeSession(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::MatchmakeSession* pSession)
{
    return m_Extension.CallGetMatchmakeSession(pContext, gatheringId, pSession);
}

// 0x003B4A88 slot 0x0C | fefates:bytes
bool nn::nex::MatchmakeExtensionClient::Bind(nn::nex::Credentials* pCredentials)
{
    bool result = ServiceClient::Bind(pCredentials);
    if (result && !m_IsExtensionRegistered) {
        RegisterProtocol(&m_Extension);
        RegisterPrototypeInline(new MatchmakeSession());
        RegisterPrototypeInline(new PersistentGathering());
        m_IsExtensionRegistered = true;
    }
    return result;
}

// 0x003B4B5C slot 0x10 | fefates:bytes
bool nn::nex::MatchmakeExtensionClient::Unbind()
{
    bool result = ServiceClient::Unbind();
    if (result && m_IsExtensionRegistered) {
        UnregisterProtocol(&m_Extension);
    }
    m_IsExtensionRegistered = false;
    return result;
}

// 0x003B4B94 | fefates:bytes [tier B]
nn::nex::MatchmakeExtensionClient::MatchmakeExtensionClient() : m_IsExtensionRegistered(false), m_Extension(0), m_LastProgressScoreTime(0)
{
    m_Extension.m_ProtocolId = EXTENSION_PROTOCOL_ID;
}

// 0x003B4C1C slot 0x00 | fefates:bytes
// 0x003B4BE0 (deleting dtor)
nn::nex::MatchmakeExtensionClient::~MatchmakeExtensionClient()
{
    if (m_IsExtensionRegistered) {
        UnregisterProtocol(&m_Extension);
    }
}

// 0x003C6EF8 (name is ours)
bool nn::nex::MatchmakeExtensionClient::Method25(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds)
{
    return m_Extension.CallMethod25(pContext, principalIds);
}

// 0x003C71B0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::GetPlayingSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds,
                                                         nn::nex::qList<nn::nex::PlayingSession>* pSessions)
{
    return m_Extension.CallGetPlayingSession(pContext, principalIds, pSessions);
}

// 0x003C7708 (name is ours)
bool nn::nex::MatchmakeExtensionClient::Method26(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds)
{
    return m_Extension.CallMethod26(pContext, principalIds);
}

// 0x003C8A64 (name is ours)
bool nn::nex::MatchmakeExtensionClient::UpdateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::UpdateMatchmakeSessionParam& param)
{
    return m_Extension.CallUpdateMatchmakeSession(pContext, param);
}

// 0x003C8CC0 (name is ours)
bool nn::nex::MatchmakeExtensionClient::JoinMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::JoinMatchmakeSessionParam& param,
                                                            nn::nex::MatchmakeSession* pSession)
{
    return m_Extension.CallJoinMatchmakeSession(pContext, param, pSession);
}

// 0x003C94C8 (name is ours)
bool nn::nex::MatchmakeExtensionClient::AutoMatchmake(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param, nn::nex::MatchmakeSession* pSession)
{
    return m_Extension.CallAutoMatchmake(pContext, param, pSession);
}

// 0x003C9630 (name is ours)
bool nn::nex::MatchmakeExtensionClient::CreateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::CreateMatchmakeSessionParam& param,
                                                              nn::nex::MatchmakeSession* pSession)
{
    return m_Extension.CallCreateMatchmakeSession(pContext, param, pSession);
}

} // namespace nex
} // namespace nn
