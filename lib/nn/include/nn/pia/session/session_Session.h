#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Crypto.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationIdTable.h"

namespace nn {
namespace pia {
namespace transport {
class NetworkFactory;
} // namespace transport
namespace session {
class AutoMatchmakeJob;
class BrowseMatchmakeJob;
class ClearMatchmakeSystemPasswordJob;
class CloseParticipationJob;
class CommonMatchmakeSession;
class ConfigParticipationJobBase;
class CreateSessionJob;
class CreateSessionSetting;
class DestroySessionJob;
class GenerateMatchmakeSystemPasswordJob;
class ISessionInfoList;
class JoinSessionJob;
class JoinSessionSetting;
class JointSessionJob;
class LeaveSessionJob;
class MeshEventListenerForSession;
class MeshLayerController;
class ModifyAttributeJob;
class OpenParticipationJob;
class SessionProtocol;
class SessionSearchCriteria;
class SessionStatusCheckJob;
class StationIdStatusTable;
class UpdateApplicationDataJob;
class UpdateSessionSettingJob;

// RTTI N2nn3pia7session7SessionE @ 0x008D014C
// vtable 0x00901D3C (vptr 0x00901D44), offset_to_top 0, 1 entries
//
// The session on top of the mesh: the matchmaking (create, auto matchmake, browse, join,
// leave the sessions of the network through the jobs the NetworkFactory makes), the stations
// of the session by their station ids and, with the station id table, the joint sessions (two
// matchmake sessions at once). One instance (CreateInstance). Its destructor is not virtual (the
// vtable only has Trace). Layout from the constructor and CreateInstance; all names except
// IsUsingStationIdTable are ours.
class Session : public ::nn::pia::common::RootObject
{
public:
    // the argument of CreateInstance (passed by value)
    struct Setting
    {
        transport::NetworkFactory* m_pNetworkFactory; // 0x0
        u8 m_RelayMode;                               // 0x4 (Mesh::Setting)
        u8 m_SessionInfoNumMax;                       // 0x5, the capacity of the session info list
        bool m_IsBandwidthCheckEnabled;               // 0x6 (Mesh::Setting)
    };

    // the argument of Startup (most of it goes to MeshLayerController::Startup)
    struct StartupSetting
    {
        bool m_IsHostMigrationEnabled;                       // 0x00
        const u8* m_pIdentificationData;                     // 0x04
        common::Crypto::Mode m_CryptoMode;                   // 0x08 (only if the network can)
        u32 m_TimeoutMSec;                                   // 0x0C, 1000..30000
        u32 m_KeepAliveIntervalMSec;                         // 0x10
        s32 m_BandwidthCheckBandwidth;                       // 0x14
        u32 m_BandwidthCheckPacketSize;                      // 0x18
        s32 m_BandwidthCheckDurationMSec;                    // 0x1C
        bool m_IsBandwidthCheckOneWay;                       // 0x20
        const transport::Station::PlayerName* m_pPlayerName; // 0x24
    };

    // the setting of the next CreateInstance (0x00975A80)
    struct GlobalSetting
    {
        bool m_IsRelayRouteNetwork; // 0x0 (Mesh::GlobalSetting)
        bool m_Unknown0x1;          // 0x1 (SessionStatusCheckJob, MeshEventListenerForSession)
        bool m_IsTimeoutFree;       // 0x2 (Mesh::GlobalSetting; Startup does not check the timeout)
    };

    // the asynchronous operation of m_CallContext (m_AsyncType)
    enum AsyncType : u8
    {
        ASYNC_TYPE_NONE = 0,
        ASYNC_TYPE_AUTO_MATCHMAKE = 1,
        ASYNC_TYPE_BROWSE = 2,
        ASYNC_TYPE_CREATE = 3,
        ASYNC_TYPE_JOIN = 4,
        ASYNC_TYPE_LEAVE = 5, // or destroy
        ASYNC_TYPE_OPEN_PARTICIPATION = 6,
        ASYNC_TYPE_CLOSE_PARTICIPATION = 8,
        ASYNC_TYPE_MODIFY_ATTRIBUTE = 35,
    };

    // the events of m_EventCallback
    enum EventType : u8
    {
        EVENT_TYPE_JOIN = 0,
        EVENT_TYPE_LEAVE = 1,
        EVENT_TYPE_HOST_CHANGED = 2,
        EVENT_TYPE_3 = 3,
        EVENT_TYPE_JOINT_HOST_CHANGED = 4,
        // the joint session job (JointSessionJob): started and done in the phases 6 to 10, failed
        EVENT_TYPE_JOINT_SESSION_STARTED_6 = 11,
        EVENT_TYPE_JOINT_SESSION_DONE_6 = 12,
        EVENT_TYPE_JOINT_SESSION_STARTED_7 = 13,
        EVENT_TYPE_JOINT_SESSION_DONE_7 = 14,
        EVENT_TYPE_JOINT_SESSION_STARTED_8 = 15,
        EVENT_TYPE_JOINT_SESSION_DONE_8 = 16,
        EVENT_TYPE_JOINT_SESSION_STARTED_9 = 17,
        EVENT_TYPE_JOINT_SESSION_DONE_9 = 18,
        EVENT_TYPE_JOINT_SESSION_STARTED_10 = 19,
        EVENT_TYPE_JOINT_SESSION_DONE_10 = 20,
        EVENT_TYPE_JOINT_SESSION_FAILED = 21,
        // the string of the session was set / cleared (nex notifications 120 / 121)
        EVENT_TYPE_STRING_SET = 22,
        EVENT_TYPE_STRING_CLEARED = 23,
    };

    // the state of the session (GetStatus): m_State 2..4 give 1..3, m_DisconnectState 2, 3 give 4, 5
    enum Status : u8
    {
        STATUS_NONE = 0,
        STATUS_1 = 1,
        STATUS_2 = 2,
        STATUS_JOINT = 3,
        STATUS_DISCONNECTED_4 = 4,
        STATUS_DISCONNECTED_5 = 5,
    };

    typedef void (*EventCallback)(EventType type, StationId stationId);
    typedef bool (*JoinApprovalCallback)(const transport::Station::IdentificationInfo* pInfo);
    typedef s32 (*StationIdCallback)(const transport::StationIdTable::Entry* pEntry);

    explicit Session(Setting setting); // 0x0044CD34
    ~Session(); // 0x0044CE90
    virtual void Trace(u64 flag) const; // 0x007349A4 slot 0x00

    static nn::Result CreateInstance(Setting setting); // 0x0044A4D4
    static void DestroyInstance(); // 0x0044AE04
    nn::Result Startup(const StartupSetting& setting); // 0x0044CBD8
    void Cleanup(); // 0x0044C7DC

    // the asynchronous operations
    nn::Result CreateSessionAsync(const CreateSessionSetting* pSetting); // 0x0044B538
    nn::Result AutoMatchmakeAsync(const CreateSessionSetting* pCreateSetting, const SessionSearchCriteria* pCriteria, u32 criteriaNum); // 0x0044BCF0
    nn::Result BrowseSessionAsync(const SessionSearchCriteria* pCriteria); // 0x0044B41C
    nn::Result JoinSessionAsync(const JoinSessionSetting* pSetting); // 0x0044B0E8
    nn::Result LeaveSessionAsync(); // 0x0044B2AC
    nn::Result OpenParticipationAsync(); // 0x0044BDC8
    nn::Result CloseParticipationAsync(); // 0x0044BE58
    nn::Result ModifyAttributeAsync(u32 index, u32 value); // 0x0044B6EC
    DECOMP_NOINLINE nn::Result ModifyAttributeAsyncCore(u32 index, u32 value, u32 sessionId, CommonMatchmakeSession* pSession); // 0x0044BF94
    nn::Result GetAsyncResult(AsyncType type) const; // 0x00734714
    nn::Result GetJoinSessionAsyncResult() const; // 0x0073470C
    nn::Result GetLeaveSessionAsyncResult() const; // 0x00734744
    nn::Result GetBrowseSessionAsyncResult() const; // 0x0073475C
    nn::Result GetCreateSessionAsyncResult() const; // 0x00734764
    nn::Result GetModifyAttributeAsyncResult() const; // 0x007347CC
    nn::Result GetAutoMatchmakeAsyncResult() const; // 0x00734834
    nn::Result GetOpenParticipationAsyncResult() const; // 0x0073483C
    nn::Result GetCloseParticipationAsyncResult() const; // 0x00734874
    bool IsJoinSessionAsyncCompleted() const; // 0x0073476C
    bool IsLeaveSessionAsyncCompleted() const; // 0x0073479C
    bool IsBrowseSessionAsyncCompleted() const; // 0x007347D4
    bool IsCreateSessionAsyncCompleted() const; // 0x00734804
    bool IsModifyAttributeAsyncCompleted() const; // 0x00734844
    bool IsAutoMatchmakeAsyncCompleted() const; // 0x007348A8
    bool IsOpenParticipationAsyncCompleted() const; // 0x007348D8
    bool IsCloseParticipationAsyncCompleted() const; // 0x00734908

    // the stations
    bool AddStation(transport::StationIdTable::Entry entry, u32 sessionId); // 0x0044ACD0
    bool RemoveStation(transport::StationIdTable::Entry entry, u32 sessionId); // 0x0044AEC4
    void AddToStationIdList(const StationId& stationId); // 0x0044B3A0
    void RemoveFromStationIdList(const StationId& stationId); // 0x0044B650
    bool IsValidStation(StationId stationId) const; // 0x00734684
    DECOMP_NOINLINE u16 GetStationNum() const; // 0x007343DC
    bool ClearStationBit(u32 stationIndex); // 0x0044C4DC
    void SetupStationIdsAsHost(); // 0x0044C144
    bool SetupStationIdsAsClient(); // 0x0044C304

    // the events and the notifications of the server
    void NotifyJoinEvent(StationId stationId); // 0x0044A0F0
    DECOMP_NOINLINE void NotifyEvent(EventType type, StationId stationId); // 0x0044BEE8
    static void NotifyStationEvent(EventType type, StationIndex stationIndex); // 0x0044B5F0
    void OnParticipantDisconnected(u32 sessionId, u32 principalId); // 0x0044A114
    void UpdateSessionOwner(u32 sessionId, u32 ownerPrincipalId); // 0x0044B744
    void OnUnknownNotification(u32 value); // 0x0044B190
    void OnUnknownNotification2(u32 value1, u32 value2); // 0x0044B264
    nn::Result SetEventCallback(EventCallback callback); // 0x0044C294
    void ClearEventCallback(); // 0x0044C4A8
    void ClearJoinApprovalCallback(); // 0x0044C79C
    // the matchmake session that was joined: the other one in a joint session (name is ours)
    CommonMatchmakeSession* GetJoinedMatchmakeSession() const; // 0x0044C7A8

    // the callbacks of the mesh (MeshLayerController::StartupMeshCore) and of the transport
    static bool CheckJoinApproval(const transport::Station::IdentificationInfo* pInfo); // 0x0044C478
    static u32 GetHostCandidatePriority(StationIndex stationIndex, bool isFromConnectionInfo); // 0x0044C634
    static u32 GetCurrentSessionId(); // 0x0044B6C8
    StationIdCallback GetStationIdCallback() const; // 0x0044C2F8

    // the session ids
    void SetJoinable(u32 sessionId, bool isJoinable); // 0x0044AC2C
    // 0: whether the current session may be joined
    bool IsJoinable(u32 sessionId) const; // 0x00734418
    u32 GetJointSessionId() const; // 0x007344EC
    u32 GetJoinedSessionId() const; // 0x00734964

    // the state
    bool IsUsingStationIdTable() const; // 0x0073474C | fefates:bytes
    bool IsJoining() const; // 0x007344B0
    DECOMP_NOINLINE bool IsHost() const; // 0x007349A8
    DECOMP_NOINLINE bool IsHostOfBothSessions() const; // 0x00734528
    Status GetStatus() const; // 0x00734AB8
    nn::Result CheckStatus() const; // 0x00734604
    StationId GetJointHostStationId() const; // 0x0073487C
    u8 GetJoinSessionPhase() const; // 0x0044AE4C
    u8 GetAutoMatchmakePhase() const; // 0x0044C21C
    u8 GetJointSessionPhase() const; // 0x0044B0D8
    static u8 GetJoinMeshJobPhase(); // 0x0044B5E0
    DECOMP_NOINLINE u32 GetMeshLayerControllerValue() const; // 0x00734938
    ISessionInfoList* GetSessionInfoList() const; // 0x0044C0A8
    u8* GetUnknown0x101(); // 0x0044C0C0
    bool GetUnknown0x100() const; // 0x0044B738
    void CleanupConfigParticipationJob(); // 0x0044C134
    void SetString(const u16* pString, u32 length); // 0x0044C04C
    void ClearString(); // 0x0044C0F4
    DECOMP_NOINLINE void StopSessionStatusCheck(); // 0x0044C1CC
    void SetUnknownFlagOfJoiningJobs(); // 0x0044C2B0
    DECOMP_NOINLINE void SetDisconnected(); // 0x0044C524
    DECOMP_NOINLINE void SetDisconnectedByError(); // 0x0044C5AC
    void ClearStatus(bool keepStationIds); // 0x0044A26C
    static void SetSyncClockRequestInterval(s32 intervalMSec); // 0x0044C0CC
    static void SetTransportState(bool isAvailable); // 0x0044C4B4

    // 0x00975A80
    static GlobalSetting s_GlobalSetting;
    static Session* s_pInstance;

    bool m_IsHostMigrationEnabled;                         // 0x004
    bool m_Unknown0x5;                                     // 0x005 (NetworkFactory::vf_0xA0)
    MeshLayerController* m_pMeshLayerController;           // 0x008
    SessionStatusCheckJob* m_pSessionStatusCheckJob;       // 0x00C
    CreateSessionJob* m_pCreateSessionJob;                 // 0x010
    AutoMatchmakeJob* m_pAutoMatchmakeJob;                 // 0x014
    BrowseMatchmakeJob* m_pBrowseMatchmakeJob;             // 0x018
    JoinSessionJob* m_pJoinSessionJob;                     // 0x01C
    LeaveSessionJob* m_pLeaveSessionJob;                   // 0x020
    DestroySessionJob* m_pDestroySessionJob;               // 0x024
    OpenParticipationJob* m_pOpenParticipationJob;         // 0x028
    CloseParticipationJob* m_pCloseParticipationJob;       // 0x02C
    ConfigParticipationJobBase* m_pConfigParticipationJob; // 0x030
    GenerateMatchmakeSystemPasswordJob* m_pGenerateMatchmakeSystemPasswordJob; // 0x034
    ClearMatchmakeSystemPasswordJob* m_pClearMatchmakeSystemPasswordJob;       // 0x038
    u32 m_StringLength;                                    // 0x03C (NetworkFactory::GetStringBufferLength)
    u16* m_pString;                                        // 0x040
    JointSessionJob* m_pJointSessionJob;                   // 0x044
    ModifyAttributeJob* m_pModifyAttributeJob;             // 0x048
    UpdateSessionSettingJob* m_pUpdateSessionSettingJob;   // 0x04C
    UpdateApplicationDataJob* m_pUpdateApplicationDataJob; // 0x050
    u32 m_Unknown0x54;                                     // 0x054 (not set)
    u32 m_Unknown0x58;                                     // 0x058 (not set)
    EventCallback m_EventCallback;                         // 0x05C
    JoinApprovalCallback m_JoinApprovalCallback;           // 0x060
    StationIdStatusTable* m_pStationIdStatusTable;         // 0x064
    transport::ProtocolId m_SessionProtocolId;             // 0x068
    SessionProtocol* m_pSessionProtocol;                   // 0x06C
    bool m_IsStarted;                                      // 0x070
    u8 m_State;                                            // 0x071, 2, 3, 4 (4: joint session)
    u8 m_DisconnectState;                                  // 0x072, 2, 3
    StationId m_LocalStationId;                            // 0x074
    StationId m_HostStationId;                             // 0x07C
    StationId m_JointHostStationId;                        // 0x084
    u32 m_Unknown0x8C;                                     // 0x08C
    common::CallContext m_CallContext;                     // 0x090, of the *Async functions
    AsyncType m_AsyncType;                                 // 0x0A4
    MeshEventListenerForSession* m_pMeshEventListener;     // 0x0A8
    u32 m_StationBitmap;                                   // 0x0AC
    bool m_Unknown0xB0;                                    // 0x0B0
    u8 m_Unknown0xB1;                                      // 0x0B1
    bool m_Unknown0xB2;                                    // 0x0B2
    u8 m_StationIdEntryNumMax[2];                          // 0x0B3, of the matchmake sessions (StationIdTable::SetEntryNumMax)
    u8 m_CurrentIndex;                                     // 0x0B5, of the matchmake sessions
    CommonMatchmakeSession* m_pMatchmakeSessions[2];       // 0x0B8 (the second one only for joint sessions)
    ISessionInfoList* m_pSessionInfoList;                  // 0x0C0
    u32 m_SessionIds[2];                                   // 0x0C4, of the matchmake sessions
    common::ObjList<StationId> m_StationIdList;            // 0x0CC
    u64* m_pStationIdNodeBuffer;                           // 0x0F8
    u32 m_Unknown0xFC;                                     // 0x0FC, a session id
    bool m_Unknown0x100;                                   // 0x100
    u8 m_Unknown0x101[32];                                 // 0x101
    u32 m_UnjoinableSessionIds[4];                         // 0x124
    bool m_Unknown0x134;                                   // 0x134 (MeshLayerController::Startup)
};
ASSERT_OFFSET(Session, m_EventCallback, 0x5C);
ASSERT_OFFSET(Session, m_LocalStationId, 0x74);
ASSERT_OFFSET(Session, m_CallContext, 0x90);
ASSERT_OFFSET(Session, m_pMatchmakeSessions, 0xB8);
ASSERT_OFFSET(Session, m_StationIdList, 0xCC);
ASSERT_OFFSET(Session, m_UnjoinableSessionIds, 0x124);
ASSERT_SIZE(Session, 0x138);
} // namespace session
} // namespace pia
} // namespace nn
