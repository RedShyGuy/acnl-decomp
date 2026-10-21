#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_MatchmakeExtensionProtocolClient.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
class AutoMatchmakeParam;
class CreateMatchmakeSessionParam;
class JoinMatchmakeSessionParam;
class MatchmakeSession;
class MatchmakeSessionSearchCriteria;
class PlayingSession;
class UpdateMatchmakeSessionParam;
template <typename T0> class qVector;
} // namespace nex
} // namespace nn

namespace nn {
namespace nex {
// RTTI N2nn3nex24MatchmakeExtensionClientE @ 0x008CEDF0
// vtable 0x008FE868 (vptr 0x008FE870), offset_to_top 0, 10 entries
class MatchmakeExtensionClient : public ::nn::nex::MatchMakingClient
{
public:
    // the id of the matchmake extension protocol
    static const u16 EXTENSION_PROTOCOL_ID = 109;
    // the shortest time between two updates of the progress score (ms)
    static const s64 PROGRESS_SCORE_INTERVAL = 30000;
    // the highest progress score
    static const u8 MAX_PROGRESS_SCORE = 100;

    MatchmakeExtensionClient(); // 0x003B4B94 | fefates:bytes [tier B]
    virtual ~MatchmakeExtensionClient(); // 0x003B4C1C slot 0x00 | fefates:bytes
    // 0x003B4BE0 slot 0x04 (deleting dtor)
    virtual bool Bind(nn::nex::Credentials* pCredentials); // 0x003B4A88 slot 0x0C | fefates:bytes
    virtual bool Unbind(); // 0x003B4B5C slot 0x10 | fefates:bytes

    // The calls of the protocol (the names are ours after their use in pia::inet::
    // NexMatchmakeSession, the others after the method ids). armlink merged the tail calls of the
    // ones at 0x3C...: they are "add r0, #0x9C; nop" in front of the protocol functions.
    bool AutoMatchmakePostpone(nn::nex::ProtocolCallContext* pContext, const nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>& gathering,
                               nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pJoinedGathering, const nn::nex::String& message); // 0x003B47FC
    bool Method28(nn::nex::ProtocolCallContext* pContext, u32 value1, const nn::nex::String& text, u32 value2); // 0x003B4814
    bool OpenParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B483C
    bool CloseParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B4854
    // without a call context; at most every 30 seconds (else false)
    bool UpdateProgressScore(u32 gatheringId, u8 score); // 0x003B486C
    bool BrowseMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::MatchmakeSessionSearchCriteria& criteria,
                                const nn::nex::ResultRange& range,
                                nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings); // 0x003B49A0
    bool Method9(nn::nex::ProtocolCallContext* pContext, u32 value1, u32 value2, u32 value3, const nn::nex::String& text); // 0x003B49B8
    bool UpdateApplicationBuffer(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qVector<u8>& buffer); // 0x003B49E4
    bool Method10(nn::nex::ProtocolCallContext* pContext, nn::nex::qList<nn::nex::NotificationEvent>* pEvents, u32 value); // 0x003B49FC
    // the index of pia counts from 0, the one of the protocol from 1
    bool ModifyCurrentGameAttribute(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, u32 index, u32 value); // 0x003B4A14
    bool ClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B4A40
    bool GenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::String* pPassword); // 0x003B4A58
    bool GetMatchmakeSession(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::MatchmakeSession* pSession); // 0x003B4A70
    bool Method25(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds); // 0x003C6EF8
    bool GetPlayingSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds,
                           nn::nex::qList<nn::nex::PlayingSession>* pSessions); // 0x003C71B0
    bool Method26(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds); // 0x003C7708
    bool UpdateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::UpdateMatchmakeSessionParam& param); // 0x003C8A64
    bool JoinMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::JoinMatchmakeSessionParam& param,
                              nn::nex::MatchmakeSession* pSession); // 0x003C8CC0
    bool AutoMatchmake(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param,
                       nn::nex::MatchmakeSession* pSession); // 0x003C94C8
    bool CreateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::CreateMatchmakeSessionParam& param,
                                nn::nex::MatchmakeSession* pSession); // 0x003C9630

    bool m_IsExtensionRegistered;                  // 0x98 (the protocol and the prototypes)
    MatchmakeExtensionProtocolClient m_Extension;  // 0x9C
    Time m_LastProgressScoreTime;                  // 0xE0
};
ASSERT_OFFSET(MatchmakeExtensionClient, m_Extension, 0x9C);
ASSERT_OFFSET(MatchmakeExtensionClient, m_LastProgressScoreTime, 0xE0);
ASSERT_SIZE(MatchmakeExtensionClient, 0xE8);
} // namespace nex
} // namespace nn
