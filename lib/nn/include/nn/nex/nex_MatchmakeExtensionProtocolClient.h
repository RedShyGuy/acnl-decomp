#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class AutoMatchmakeParam;
class CreateMatchmakeSessionParam;
class Gathering;
class JoinMatchmakeSessionParam;
class MatchmakeSession;
class MatchmakeSessionSearchCriteria;
class NotificationEvent;
class PlayingSession;
class ProtocolCallContext;
class ResultRange;
class String;
class UpdateMatchmakeSessionParam;

// The client of the matchmake extension protocol (protocol id 109). The protocol has no method
// names in the binary: the names are ours, after their use in pia::inet::NexMatchmakeSession and
// in MatchmakeExtensionClient; the methods without such a use are named by their ids.
// RTTI N2nn3nex32MatchmakeExtensionProtocolClientE @ 0x008CF290
// vtable 0x008FF57C (vptr 0x008FF584), offset_to_top 0, 23 entries
class MatchmakeExtensionProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol that are called
    enum Method
    {
        METHOD_CLOSE_PARTICIPATION = 1,
        METHOD_OPEN_PARTICIPATION = 2,
        METHOD_AUTO_MATCHMAKE_POSTPONE = 3,
        METHOD_BROWSE_MATCHMAKE_SESSION = 4,
        METHOD_MODIFY_CURRENT_GAME_ATTRIBUTE = 8,
        METHOD_9 = 9,
        METHOD_10 = 10,
        METHOD_UPDATE_APPLICATION_BUFFER = 11,
        METHOD_GET_PLAYING_SESSION = 16,
        METHOD_25 = 25,
        METHOD_26 = 26,
        METHOD_28 = 28,
        METHOD_UPDATE_PROGRESS_SCORE = 34,
        METHOD_GENERATE_MATCHMAKE_SESSION_SYSTEM_PASSWORD = 36,
        METHOD_CLEAR_MATCHMAKE_SESSION_SYSTEM_PASSWORD = 37,
        METHOD_CREATE_MATCHMAKE_SESSION = 38,
        METHOD_JOIN_MATCHMAKE_SESSION = 39,
        METHOD_AUTO_MATCHMAKE = 40,
        METHOD_GET_MATCHMAKE_SESSION = 41,
        METHOD_UPDATE_MATCHMAKE_SESSION = 44,
        METHOD_45 = 45,
        METHOD_COUNT = 47
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);
    typedef AnyObjectHolder<Gathering, String> GatheringHolder;

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE MatchmakeExtensionProtocolClient(unsigned short id); // 0x003CA4F8
    virtual ~MatchmakeExtensionProtocolClient(); // 0x003CA5F0 slot 0x00
    // 0x003CA5E0 slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C7E24 slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DEDC slot 0x54

    // (the return types are ours)
    bool CallMethod25(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds); // 0x003C6F00
    bool CallMethod28(nn::nex::ProtocolCallContext* pContext, const unsigned int& value1, const nn::nex::String& text, const unsigned int& value2); // 0x003C7044
    bool CallGetPlayingSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds, nn::nex::qList<nn::nex::PlayingSession>* pSessions); // 0x003C71B8
    bool CallOpenParticipation(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId); // 0x003C7318
    bool CallCloseParticipation(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId); // 0x003C7460
    bool CallMethod45(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param, u64* pResult); // 0x003C75A8
    bool CallMethod26(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds); // 0x003C7710
    bool CallUpdateProgressScore(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const u8& score); // 0x003C7854
    bool CallAutoMatchmakePostpone(nn::nex::ProtocolCallContext* pContext, const GatheringHolder& gathering, GatheringHolder* pJoinedGathering,
                                   const nn::nex::String& message); // 0x003C79B0
    bool CallBrowseMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::MatchmakeSessionSearchCriteria& criteria, const nn::nex::ResultRange& range,
                                    nn::nex::qList<GatheringHolder>* pGatherings); // 0x003C7B34
    bool CallMethod9(nn::nex::ProtocolCallContext* pContext, const unsigned int& value1, const unsigned int& value2, const unsigned int& value3,
                     const nn::nex::String& text); // 0x003C7CA4
    bool CallUpdateApplicationBuffer(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const nn::nex::qVector<u8>& buffer); // 0x003C7EAC
    bool CallMethod10(nn::nex::ProtocolCallContext* pContext, nn::nex::qList<nn::nex::NotificationEvent>* pEvents, const unsigned int& value); // 0x003C804C
    bool CallModifyCurrentGameAttribute(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const unsigned int& index,
                                        const unsigned int& value); // 0x003C88F4
    bool CallUpdateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::UpdateMatchmakeSessionParam& param); // 0x003C8A6C
    bool CallJoinMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::JoinMatchmakeSessionParam& param, nn::nex::MatchmakeSession* pSession); // 0x003C8CC8
    bool CallAutoMatchmake(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param, nn::nex::MatchmakeSession* pSession); // 0x003C94D0
    bool CallCreateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::CreateMatchmakeSessionParam& param, nn::nex::MatchmakeSession* pSession); // 0x003C9638
    bool CallClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId); // 0x003C9E78
    bool CallGenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, nn::nex::String* pPassword); // 0x003CA03C
    bool CallGetMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, nn::nex::MatchmakeSession* pSession); // 0x003CA1A0

    // (no results)
    static void ProtoReturn_Method25(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C7EA8
    static void ProtoReturn_Method28(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8048
    static void ProtoReturn_OpenParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C88F0
    static void ProtoReturn_CloseParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8BB0
    static void ProtoReturn_Method26(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8CB8
    static void ProtoReturn_UpdateProgressScore(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8CBC
    static void ProtoReturn_Method9(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C94C4
    static void ProtoReturn_UpdateApplicationBuffer(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C9884
    static void ProtoReturn_ModifyCurrentGameAttribute(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C9E70
    static void ProtoReturn_UpdateMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C9E74
    static void ProtoReturn_ClearMatchmakeSessionSystemPassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CA3FC
    // the results
    static void ProtoReturn_GetPlayingSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C81B0
    static void ProtoReturn_Method45(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8BB4
    // (symbols.json names it after the postponed auto matchmake with search criteria; the code is the
    // same for any method with a gathering as the result)
    static void ProtoReturn_AutoMatchmakePostpone(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8E28
    static void ProtoReturn_BrowseMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C8F10
    static void ProtoReturn_Method10(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C9888
    static void ProtoReturn_JoinMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C9FC0
    static void ProtoReturn_AutoMatchmake(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CA304
    static void ProtoReturn_CreateMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CA380
    static void ProtoReturn_GenerateMatchmakeSessionSystemPassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CA400
    static void ProtoReturn_GetMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003CA47C

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(MatchmakeExtensionProtocolClient, 0x44);
} // namespace nex
} // namespace nn
