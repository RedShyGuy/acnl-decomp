#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qVector.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"

namespace nn {
namespace nex {
class Gathering;
class JoinMatchmakeSessionParam;
class MatchmakeExtensionClient;
class MatchmakeSession;
class MatchmakeSessionSearchCriteria;
class NgsBridgeInterface;
class PlayingSession;
class ResultRange;
class StationURL;
class String;
class UpdateMatchmakeSessionParam;
} // namespace nex
namespace pia {
namespace inet {
class NexCreateSessionSetting;
class NexJoinSessionSetting;
class NexSessionSearchCriteria;
class NexUpdateSessionSetting;
// RTTI N2nn3pia4inet19NexMatchmakeSessionE @ 0x008CF93C
// vtable 0x0090024C (vptr 0x00900254), offset_to_top 0, 63 entries
//
// The matchmake session of the inet network: every *Async function starts one call of the
// matchmake extension client (or the matchmaking client) of nex with the own nex call context,
// the matching Is*Completed function signals the result to the call context of pia and maps the
// results of nex to the ones of pia. Only one call runs at a time. The layout is from the
// constructor; the member names and most slot names are ours.
class NexMatchmakeSession : public ::nn::pia::session::CommonMatchmakeSession
{
public:
    typedef nex::AnyObjectHolder<nex::Gathering, nex::String> GatheringHolder;
    // an entry of the list at 0x7C (only created, cleared and deleted; the type is not known)
    struct Unknown0x7CEntry
    {
        u8 m_Data[0x20];
    };

    // the timeout of the calls that wait for the server (ms)
    static const s64 CALL_TIMEOUT_MSEC = 10000;
    // the progress score is sent at most every that many seconds
    static const s64 PROGRESS_SCORE_INTERVAL_SEC = 30;

    NexMatchmakeSession(); // 0x003FF280
    // the values of pia for nex (NexSessionSearchCriteria; names are ours)
    // 0 -> 1, 1 -> 2, others -> 0
    static u32 ConvertMatchmakeSystemType(u8 type); // 0x003FED7C
    // 0 -> 0, 1 -> 4, 2 -> 5, others -> 0
    static u32 ConvertSelectionMethod(u8 method); // 0x003FEFCC
    // the nex search criteria of a search (an array of criteria objects); false if they are not
    // valid (names are ours)
    bool SetSearchCriteria(const NexSessionSearchCriteria* pCriteria, u32 criteriaNum); // 0x003FA72C
    // copies the setting into the update parameter (name is ours)
    void SetSessionSetting(const NexUpdateSessionSetting* pSetting); // 0x003F9E2C
    // the matchmaking client binds to the credentials of the bridge / unbinds (names are ours)
    nn::Result Bind(nn::nex::NgsBridgeInterface* pNgsBridge); // 0x003F92E8
    nn::Result Unbind(); // 0x003FF1A0
    // copies the create setting into the nex matchmake session (name is ours)
    void SetCreateSetting(const NexCreateSessionSetting* pSetting, bool isHostMigrationEnabled); // 0x003FDC04
    // copies the join setting into the join parameter (name is ours)
    void SetJoinSetting(const NexJoinSessionSetting* pSetting); // 0x003F9958
    // the search is done: its lists are cleared (name is ours)
    void FinishBrowse(); // 0x003FB718
    // the matchmake session is fetched again if it is the current one / whether that is done
    // (then the members of the session are updated; names are ours)
    nn::Result GetSessionStatusAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FC3E0
    bool IsGetSessionStatusCompleted(); // 0x003FDA14
    // whether the call of vf_0x78 is done; the fetched session (name is ours)
    bool IsGetMatchmakeSessionCompleted(nn::nex::MatchmakeSession** ppSession); // 0x003FEE6C

    virtual ~NexMatchmakeSession(); // 0x003FF8FC slot 0x00 | slot vf_0x00 of nn::pia::session::CommonMatchmakeSession
    // 0x003FF8EC slot 0x04 | slot vf_0x04 of nn::pia::session::CommonMatchmakeSession (deleting dtor)
    virtual void Cleanup(); // 0x003FEFF0 slot 0x08 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result AutoMatchmakeAsync(nn::pia::common::CallContext* pCallContext); // 0x003F9C1C slot 0x0C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsAutoMatchmakeCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x003FB734 slot 0x10 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result BrowseAsync(nn::pia::common::CallContext* pCallContext); // 0x003FA4E8 slot 0x14 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsBrowseCompleted(); // 0x003FC4C8 slot 0x18 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::pia::session::ISessionInfoList* GetSessionInfoList(); // 0x003FDBFC slot 0x1C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result CreateAsync(nn::pia::common::CallContext* pCallContext); // 0x003FA5D0 slot 0x20 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsCreateCompleted(u32* pSessionId); // 0x003FC8F0 slot 0x24 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result JoinAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003F9D90 slot 0x28 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsJoinCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x003FBB5C slot 0x2C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result LeaveAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FA3B4 slot 0x30 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsLeaveCompleted(); // 0x003FC080 slot 0x34 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result UnregisterAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FC25C slot 0x38 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsUnregisterCompleted(); // 0x003FD868 slot 0x3C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result OpenParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003F9564 slot 0x40 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsOpenParticipationCompleted(); // 0x003F9B1C slot 0x44 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result CloseParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003F95EC slot 0x48 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsCloseParticipationCompleted(); // 0x003FA2B8 slot 0x4C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result vf_0x50(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FB68C slot 0x50 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool vf_0x54(nn::pia::transport::StationConnectionInfo* pInfo); // 0x003FD524 slot 0x54 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result ModifyAttributeAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value); // 0x003F94C4 slot 0x58 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool IsModifyAttributeCompleted(); // 0x003F9A44 slot 0x5C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result vf_0x60(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003F9338 slot 0x60 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool vf_0x64(u32 index, u32* pValue); // 0x003F9674 slot 0x64 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result UpdateSessionSettingAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FA218 slot 0x6C
    virtual bool IsUpdateSessionSettingCompleted(); // 0x003FBF44 slot 0x70
    virtual nn::Result vf_0x74(u32 sessionId); // 0x003FC364 slot 0x74 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result vf_0x78(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FED98 slot 0x78 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual bool vf_0x7C(); // 0x003FCBF0 slot 0x7C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual nn::Result UpdateProgressScore(u32 sessionId, u8 score); // 0x003FB454 slot 0x98
    virtual bool IsProgressScoreUpdatable(u8 score); // 0x003FCF4C slot 0x9C
    // the matchmaking with stations that join together (names are ours): the principal ids of
    // the other stations, the session they have to be in (0: none) and whether the option is set
    virtual nn::Result AutoMatchmakeWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, const u32* pPrincipalIds, u32 principalNum,
                                                          u32 gatheringIdForParticipationCheck, bool isOptionSet); // 0x003FCD50 slot 0xA4
    virtual bool IsAutoMatchmakeWithParticipantsCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId, void* pApplicationData,
                                                          u32* pApplicationDataSize); // 0x003FE1D8 slot 0xA8
    virtual nn::Result CreateWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, const u32* pPrincipalIds, u32 principalNum,
                                                   u32 gatheringIdForParticipationCheck, bool isOptionSet); // 0x003FD650 slot 0xAC
    virtual bool IsCreateWithParticipantsCompleted(u32* pSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x003FEB50 slot 0xB0
    virtual nn::Result JoinWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const u32* pPrincipalIds, u32 principalNum,
                                                 u32 gatheringIdForParticipationCheck, bool isOptionSet); // 0x003FCFC8 slot 0xB4
    virtual bool IsJoinWithParticipantsCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x003FE5E8 slot 0xB8
    virtual nn::Result GenerateSystemPasswordAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FA6AC slot 0xBC
    virtual bool IsGenerateSystemPasswordCompleted(wchar_t* pPassword); // 0x003FCAD4 slot 0xC0
    virtual nn::Result ClearSystemPasswordAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003F99C8 slot 0xC4
    virtual bool IsClearSystemPasswordCompleted(); // 0x003FB4E4 slot 0xC8
    virtual nn::Result FindSessionByOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 ownerPrincipalId, u32 offset, u32 num); // 0x003F9788 slot 0xCC
    virtual bool IsFindSessionByOwnerCompleted(); // 0x003FAD68 slot 0xD0
    virtual nn::Result GetOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FB5BC slot 0xD4
    virtual bool IsGetOwnerCompleted(u32* pOwnerPrincipalId); // 0x003FD3AC slot 0xD8
    virtual nn::Result MigrateOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const u32* pPrincipalIds, u32 principalNum); // 0x003FD1D0 slot 0xDC
    virtual bool IsMigrateOwnerCompleted(); // 0x003FE9D8 slot 0xE0
    // the sessions a principal plays in
    virtual nn::Result GetJoinedSessionsAsync(nn::pia::common::CallContext* pCallContext, u32 principalId); // 0x003F9858 slot 0xE4
    virtual bool IsGetJoinedSessionsCompleted(u32 numMax, u32* pSessionIds, u32* pNum); // 0x003FB1DC slot 0xE8
    // the gathering of a session (the caller owns it then)
    virtual nn::Result FindGatheringAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x003FF1D0 slot 0xEC
    virtual bool IsFindGatheringCompleted(nn::nex::Gathering** ppGathering); // 0x003F93C4 slot 0xF0
    virtual nn::Result UpdateApplicationBufferAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const nn::nex::qVector<u8>& data); // 0x003FB32C slot 0xF4
    virtual bool IsUpdateApplicationBufferCompleted(); // 0x003FCC20 slot 0xF8

    // the network error code of the last failure (nex::ErrorCodeConverter; name is ours)
    static u32 s_NetworkErrorCode;

    nn::nex::MatchmakeExtensionClient* m_pClient;                            // 0x040
    // the session to create or to look for (SetCreateSetting)
    nn::nex::MatchmakeSession* m_pSourceSession;                             // 0x044
    nn::nex::qList<nn::nex::MatchmakeSessionSearchCriteria>* m_pSearchCriteria; // 0x048
    nn::nex::qList<nn::nex::ResultRange>* m_pResultRanges;                   // 0x04C
    // the found gatherings (BrowseAsync, FindSessionByOwnerAsync)
    nn::nex::qList<GatheringHolder>* m_pGatherings;                          // 0x050
    nn::pia::session::ISessionInfoList* m_pSessionInfoList;                  // 0x054, Session's
    GatheringHolder m_SourceGathering;                                       // 0x058
    GatheringHolder m_Gathering;                                             // 0x060
    nn::nex::UpdateMatchmakeSessionParam* m_pUpdateParam;                    // 0x068
    // the created or joined session
    nn::nex::MatchmakeSession* m_pMatchmakeSession;                          // 0x06C
    nn::nex::JoinMatchmakeSessionParam* m_pJoinParam;                        // 0x070
    nn::nex::qList<nn::nex::StationURL>* m_pSessionUrls;                     // 0x074
    nn::nex::String* m_pSystemPassword;                                      // 0x078
    nn::nex::qList<Unknown0x7CEntry>* m_pUnknown0x7C;                        // 0x07C
    nn::nex::qList<nn::nex::PlayingSession>* m_pPlayingSessions;             // 0x080
    nn::nex::qList<u32>* m_pPrincipalIds;                                    // 0x084
    nn::nex::NgsBridgeInterface* m_pNgsBridge;                               // 0x088
    u32 m_SessionId;                                                         // 0x08C, the current one
    nn::nex::ProtocolCallContext m_NexCallContext;                           // 0x090
    nn::pia::common::CallContext* m_pCallContext;                            // 0x108, of the running call
    bool m_IsHost;                                                           // 0x10C
    u32 m_Unknown0x110;                                                      // 0x110
    // the last send of the progress score
    nn::pia::common::Time m_ProgressScoreUpdateTime;                         // 0x118
    nn::pia::common::Time m_Deadline;                                        // 0x120, of the running call
    u16 m_MaxParticipants;                                                   // 0x128
    u16 m_MinParticipants;                                                   // 0x12A
    // of the search criteria (AutoMatchmakeAsync: 0 with, 2 without the extension protocol)
    u8 m_MatchmakeSystemType;                                                // 0x12C
};
ASSERT_OFFSET(NexMatchmakeSession, m_pClient, 0x40);
ASSERT_OFFSET(NexMatchmakeSession, m_SourceGathering, 0x58);
ASSERT_OFFSET(NexMatchmakeSession, m_pNgsBridge, 0x88);
ASSERT_OFFSET(NexMatchmakeSession, m_NexCallContext, 0x90);
ASSERT_OFFSET(NexMatchmakeSession, m_pCallContext, 0x108);
ASSERT_OFFSET(NexMatchmakeSession, m_ProgressScoreUpdateTime, 0x118);
ASSERT_OFFSET(NexMatchmakeSession, m_MatchmakeSystemType, 0x12C);
ASSERT_SIZE(NexMatchmakeSession, 0x130);
} // namespace inet
} // namespace pia
} // namespace nn
