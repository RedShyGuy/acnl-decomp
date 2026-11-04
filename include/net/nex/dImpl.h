#pragma once

#include "decomp.h"
#include "net/dScanBufBase.h"
#include "net/dSearchDataResult.h"
#include "net/dTagWriter.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_MessagingNotificationHandler.h"
#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/pia/pia_Types.h"
#include "nn/nex/nex_ErrorCodeConverter.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_SystemError.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/session/session_Mesh.h"
#include "sead/seadExpHeap.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_Transport.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadPtrArrayImpl.h"

namespace nn {
namespace nex {
class MatchmakeExtensionClient;
class MessagingClient;
class IndependentServer;
class NgsFacade;
class DataStoreLogicServerClient;
template <typename T>
class DataStoreClientTemplate;
class DataStoreMetaInfo;
class DataStoreSearchParam;
class DataStoreSearchResult;
} // namespace nex
namespace pia {
namespace session {
class CreateSessionSetting;
class JoinSessionSetting;
class ISessionInfo;
class SessionSearchCriteria;
} // namespace session
} // namespace pia
} // namespace nn

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead

namespace net {
// the heap of the network layer (name is ours)
extern sead::Heap* g_pNetHeap;

namespace nex {
// a station of the network game (names after libgarden)
struct ParticipantInfo
{
    DECOMP_EMIT_COPY ParticipantInfo() : stationId(0xFFFFFFFD, 0xFFFFFFFF) {} // 0x0051B730 (the invalid station)

    nn::pia::StationId stationId; // 0x00
    u32 rtt;                      // 0x08 (in milliseconds; 0 for the local station)
    u32 unk_0xC;
};
ASSERT_SIZE(ParticipantInfo, 0x10);

// the state of the matchmaking that the game reads (names after libgarden)
struct MatchInfo
{
    MatchInfo() : localStation(0xFFFFFFFD, 0xFFFFFFFF), hostStation(0xFFFFFFFD, 0xFFFFFFFF) { Clear(); }

    // no session: invalid stations and zeros
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Clear() // 0x001386E0 (name is ours)
    {
        time = 0;
        for (s32 i = 0; i < 4; i++) {
            participants[i].stationId = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
            participants[i].rtt = 0;
        }
        localStation = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
        hostStation = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
        sessionId = 0;
        instanceId = 0;
        participantCount = 0;
        notif3Received = false;
    }

    s64 time;                         // 0x00
    ParticipantInfo participants[4];  // 0x08
    nn::pia::StationId localStation;  // 0x48
    nn::pia::StationId hostStation;   // 0x50
    s32 sessionId;                    // 0x58
    u32 instanceId;                   // 0x5C
    u8 participantCount;              // 0x60
    bool notif3Received;              // 0x61
    u32 unk_0x64;
};
ASSERT_SIZE(MatchInfo, 0x68);

// The connection to the nex servers (DataStore, matchmaking, messaging) and the pia sessions (local
// and over the internet). The names after libgarden are marked, the others are ours.
class Impl
{
public:
    // RTTI N3net3nex4Impl24NotificationEventHandlerE @ 0x008D0FE4
    // vtable 0x009046B8 (vptr 0x009046C0), offset_to_top 0, 3 entries
    //
    // passes the notification events of the server to the Impl
    class NotificationEventHandler : public ::nn::nex::NotificationEventHandler
    {
    public:
        NotificationEventHandler() {}
        virtual ~NotificationEventHandler(); // 0x005152C0 slot 0x00
        virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x0051526C slot 0x08
    };

    // RTTI N3net3nex4Impl18MessageRecvHandlerE @ 0x008D0FD8
    // vtable 0x009046A4 (vptr 0x009046AC), offset_to_top 0, 3 entries
    //
    // passes the text messages of the messaging service to the message callback of the Impl
    class MessageRecvHandler : public ::nn::nex::MessagingNotificationHandler
    {
    public:
        MessageRecvHandler() {}
        // (a nop in the original that falls into the destructor of MessagingNotificationHandler)
        virtual ~MessageRecvHandler(); // 0x003BDDF4 slot 0x00
        // 0x00513400 slot 0x04 (deleting dtor)
        virtual void DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage); // 0x00513328 slot 0x08
    };

    typedef nn::pia::session::SessionSearchCriteria SessionSearchCriteria;
    typedef nn::pia::session::ISessionInfo ISessionInfo;

    // the identification data of the local session (the text "Name")
    struct IdentificationData
    {
        char16 name[17];  // 0x00
        u8 length;        // 0x22
        u8 unk_0x23;
    };

    // 100 buffers of 524 bytes for received data (a free list and an array of the used ones; names
    // are ours)
    struct ReceiveBuffer
    {
        ReceiveBuffer() : m_Source(0), m_Param(0), m_Key(0) {}

        u32 m_Source;        // 0x00 (the principal id of the sender)
        u32 m_Param;         // 0x04
        u32 m_Key;           // 0x08
        char16 m_Text[256];  // 0x0C
    };
    struct ReceivePool
    {
        DECOMP_ALWAYS_INLINE ReceivePool();
        // the used buffer goes back into the free list
        void Erase(s32 index)
        {
            for (s32 i = index; i < index + 1 && i < m_Used.size(); i++) {
                void* buffer = m_Used.at(i);
                *reinterpret_cast<void**>(buffer) = m_pFree;
                m_pFree = buffer;
            }
            m_Used.erase(index, 1);
        }
        // a new buffer from the free list in the used array (NULL if the array is full)
        ReceiveBuffer* Alloc()
        {
            if (m_Used.size() >= m_Used.mPtrNumMax) {
                return NULL;
            }
            ReceiveBuffer* buffer = static_cast<ReceiveBuffer*>(m_pFree);
            if (buffer != NULL) {
                m_pFree = *reinterpret_cast<void**>(buffer);
                ::new (buffer) ReceiveBuffer;
            }
            if (m_Used.size() < m_Used.mPtrNumMax) {
                m_Used.mPtrs[m_Used.mPtrNum] = buffer;
                m_Used.mPtrNum++;
            }
            return buffer;
        }

        sead::PtrArray<ReceiveBuffer> m_Used; // 0x00
        void* m_pFree;              // 0x0C
        ReceiveBuffer* m_pBuffers;  // 0x10
        ReceiveBuffer m_Buffers[100];   // 0x14
        void* m_UsedPointers[100];  // 0xCC54
    };

    // fills the tags that an object must have
    typedef void (*TagFunction)(TagWriter* writer);
    // reports a failure: level 1 server error, 2 no connection to the access point
    typedef void (*ErrorCallback)(u8 level, u32 errorCode);
    // whether a station with the player name may join the session (that the Impl created)
    typedef bool (*JoinApprovalCallback)(const char16* name, u32 length);
    // a station joined (with its player name) or left the pia session
    typedef void (*StationCallback)(nn::pia::StationId stationId, bool isJoined, const char16* name, u32 length);
    // a notification of type 103 of a friend (also after it was kept by the server; type 111 with no data)
    typedef void (*NotificationCallback)(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                         bool isFromHandler);
    // data that a station sent with the reliable protocol
    typedef void (*ReceiveCallback)(nn::pia::StationId stationId, const void* data, u32 size);
    // a text message of the messaging service (the sender, its subject)
    typedef void (*MessageCallback)(const u32* principalId, const wchar_t* text, u32 length, u32 unknown);

    // an object to upload (names are ours)
    struct UploadInfo
    {
        // the name (NULL: none)
        DECOMP_EMIT_COPY const wchar_t* GetName() const { return m_Name[0] != 0 ? m_Name : NULL; } // 0x00747AE4

        wchar_t m_Name[19];          // 0x00 (none: "unknown@JohnDoe")
        u16 m_DataType;              // 0x26 (0xFFFF: none)
        TagFunction m_TagFunction;   // 0x28
        u32 m_RecipientId;           // 0x2C (0: public; otherwise only this principal may read and delete it)
        u32* m_pReferredCount;       // 0x30 (UploadData of an existing object: its referred count)
    };

    // the result of UpdateObject
    enum UpdateResult : u8
    {
        UPDATE_RESULT_SUCCESS = 0,
        UPDATE_RESULT_NOT_FOUND = 1, // (the object is posted again)
        UPDATE_RESULT_FAILURE = 2,
    };

    // the order of a search (SearchData)
    enum SearchOrder : u8
    {
        SEARCH_ORDER_NONE = 0,
        SEARCH_ORDER_1 = 1, // result order column 64, descending
        SEARCH_ORDER_2 = 2, // result order column 6, descending
    };

    // the memory of pia's common module
    static const u32 COMMON_MEMORY_SIZE = 0xE8000;
    // the game server of the game and its access key
    static const u32 GAME_SERVER_ID = 0x86201;
    static const s32 LOGIN_TIMEOUT = 30000;
    // the unique ids of the local communication of the two scan modes
    static const u32 LOCAL_COMMUNICATION_ID_0 = 0x198D;
    static const u32 LOCAL_COMMUNICATION_ID_1 = 0x198E;
    // the game mode of the sessions that the game creates
    static const u32 SESSION_GAME_MODE = 3002;
    static const u32 FRIEND_SESSION_GAME_MODE = 3001;
    static const u32 SOCKET_SHARED_MEMORY_SIZE = 0x10000;
    // results of RateObject and ResetPermission (not looked up)
    static const bit32 RESULT_DATASTORE_RATING_0 = 0x80690004;
    static const bit32 RESULT_DATASTORE_RATING_1 = 0x80690007;
    static const bit32 RESULT_DATASTORE_RATING_2 = 0x80690003;
    // results of pia that Send names (the names are from the strings of Send)
    static const bit32 RESULT_SEND_BUFFER_FULL = 0xD861482C; // (not named)
    static const bit32 RESULT_NOT_INITIALIZED = 0xE0A1482B;
    static const bit32 RESULT_NOT_IMPLEMENTED = 0xE161482A;
    static const bit32 RESULT_NOT_FOUND = 0xD8814829;
    static const bit32 RESULT_CANCELLED = 0xD9214824;
    static const bit32 RESULT_BROKEN_DATA = 0xD9614822;
    static const bit32 RESULT_BUFFER_SHORTAGE = 0xE0E14823;
    static const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A14821;
    static const bit32 RESULT_ALLOCATION_FAILED = 0xE0614820;
    static const s32 SOCKET_SESSION_NUM = 4;
    // results of the session startup after which ConnectLocal reports error type 0 (not looked up)
    static const bit32 RESULT_LOCAL_STARTUP_FAILED_0 = 0xD8A14868;
    static const bit32 RESULT_LOCAL_STARTUP_FAILED_1 = 0xD8A14825;

    // notification types of the game (meanings not known)
    static const u32 NOTIFICATION_TYPE_101 = 101;
    static const u32 NOTIFICATION_TYPE_102 = 102;
    static const u32 NOTIFICATION_TYPE_103 = 103;

    static const u32 SEARCH_DATA_TYPE_NUM = 10;
    static const u32 SEARCH_DATA_TYPE_MAX = 8;
    static const u32 SEARCH_RESULT_SIZE = 50;

    Impl(); // 0x00516B48
    ~Impl(); // 0x0013C598
    // destroys the NexHeap
    DECOMP_EMIT_COPY void DestroyNexHeap() // 0x0013C568
    {
        if (s_pNexHeap != NULL) {
            s_pNexHeap->destroy();
            s_pNexHeap = NULL;
        }
    }
    // the DataStore client and http (a nop in the original falls into nn::http::Finalize)
    DECOMP_NOINLINE void FinalizeDataStore(); // 0x0046FEC8
    // the commands STUB_0 to STUB_5 (empty; armlink removed their calls)
    void Stub0(u32 param) {}
    void Stub1(u32 param) {}
    void Stub2(u32 param) {}
    void Stub3(u32 param) {}
    void Stub4(u32 param) {}
    void Stub5(u32 param) {}
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsLoggedIn() const { return m_pNgsFacade != NULL; } // 0x00747C94
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsLocalHost() const // 0x00747CA4
    {
        nn::pia::local::LocalNetwork* network = nn::pia::local::LocalNetwork::s_pInstance;
        return network != NULL && network->IsHost();
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsInLocalNetwork() const // 0x00747CD4
    {
        nn::pia::local::LocalNetwork* network = nn::pia::local::LocalNetwork::s_pInstance;
        return network != NULL && (network->IsHost() || network->IsClient());
    }
    DECOMP_NOINLINE bool Initialize(); // 0x0050E858

    // pia's common module (memory from m_pHeap, the scheduler)
    DECOMP_NOINLINE bool InitializeCommon(); // 0x0050F4D4
    DECOMP_NOINLINE void FinalizeCommon(); // 0x0050F37C
    // (a nop in the original that falls into FinalizeCommon)
    DECOMP_NOINLINE void Finalize(); // 0x0050F378
    // the local (UDS) network with a pia session; 0 or (0x80000000 | error type)
    DECOMP_NOINLINE s32 ConnectLocal(); // 0x0050EE00
    DECOMP_NOINLINE void FinalizeLocal(); // 0x0050F68C
    DECOMP_NOINLINE bool InitializeLocalNetwork(); // 0x00511E30
    DECOMP_NOINLINE bool InitializeTransport(nn::pia::transport::Transport::Setting& setting); // 0x005137E8
    DECOMP_NOINLINE nn::Result StartupSession(nn::pia::session::Session::StartupSetting& setting); // 0x00512B40
    // the event callback of the pia session
    static void OnSessionEvent(nn::pia::session::Session::EventType type, nn::pia::StationId stationId); // 0x0051451C
    // nex: memory functions, sockets and the login to the game server
    DECOMP_NOINLINE bool InitializeNex(bool unused); // 0x0050F7A0
    DECOMP_NOINLINE void FinalizeNex(); // 0x00510F00
    DECOMP_NOINLINE void FinalizeIndependent(); // 0x00515738

    // the pia session over nex
    DECOMP_NOINLINE bool InitializeInet(); // 0x00512D1C
    DECOMP_NOINLINE void FinalizeInet(); // 0x00512FE0
    // (a nop in the original that falls into LeaveSession)
    DECOMP_NOINLINE void LeaveLocalSession(); // 0x005111EC
    DECOMP_NOINLINE void LeaveSession(); // 0x005111F0
    DECOMP_NOINLINE void LeaveNexSession(); // 0x00512FC8
    DECOMP_NOINLINE bool OpenNexSession(bool notifyFriends, bool setParam = false, u16 param = 0); // 0x005112B4 | libgarden
    DECOMP_NOINLINE bool CloseNexSession(); // 0x00511928 (libgarden name)
    DECOMP_NOINLINE bool OpenParticipation(); // 0x00511AF0
    DECOMP_NOINLINE bool InitializeNexCommon(); // 0x00511B8C
    // pia's scheduler and the received data of the reliable protocol
    DECOMP_NOINLINE void Update(); // 0x00511F5C
    DECOMP_NOINLINE bool CloseParticipation(); // 0x0051208C
    // deletes the objects of search target 10 of the DataStore
    DECOMP_NOINLINE bool DeleteOwnData(); // 0x00512124
    DECOMP_NOINLINE s32 JoinLocalSession(u32 sessionId); // 0x005123F8
    // keeps a notification of the server (one per key)
    DECOMP_NOINLINE void StoreNotification(const u32& source, u32 key, u32 param, const nn::nex::String& text); // 0x005127DC
    DECOMP_NOINLINE s32 CreateLocalSession(u8 mode, const void* data, u32 dataSize); // 0x00512930
    // the friend code of a station of the local session (0 if it is not known)
    DECOMP_NOINLINE void GetFriendCode(nn::pia::StationId stationId, u64* friendCode); // 0x00513410
    DECOMP_NOINLINE bool SendNotification102(const wchar_t* text); // 0x005134EC
    // a text message (its subject) to a principal over the messaging service
    DECOMP_NOINLINE bool SendTextMessage(const u32* principalId, const wchar_t* text); // 0x0051353C
    DECOMP_NOINLINE bool JoinRandomSession(u32 attribute); // 0x00513970 (libgarden name)
    // MatchmakeExtension method 25 with the principal, then method 28 with the principal and the text
    DECOMP_NOINLINE bool CallMatchmakeMethod25(const u32* principalId, const wchar_t* text); // 0x00511448
    // MatchmakeExtension method 26 with the principal
    DECOMP_NOINLINE bool CallMatchmakeMethod26(const u32* principalId); // 0x00513E0C
    // the application data of the beacon of the local network (at most 184 bytes)
    static void SetLocalApplicationData(const void* data, u32 size); // 0x005144F4
    // the player name of the pia session (at most 16 characters, not while in a session)
    DECOMP_NOINLINE bool SetPlayerName(const char16* name, u32 length); // 0x005150C0
    // attribute 2 of the session becomes 1
    DECOMP_NOINLINE bool ModifySessionAttribute(); // 0x005152C4
    // a session for friends with the text of its notification
    DECOMP_NOINLINE bool CreateNexSession(const char16* text, u32 length, u16 param); // 0x00515360
    // the sessions that friends announced (notifications of type 101)
    DECOMP_NOINLINE void GetSessionNotifications(ScanBufBase** results, u32 resultNum, u32* foundNum, bool withText); // 0x00515494
    // the independent server (with a login if logsIn)
    DECOMP_NOINLINE bool InitializeIndependent(bool logsIn); // 0x005155A8
    // leaves the session (and tells the friends that it is closed)
    DECOMP_NOINLINE void LeaveFriendSession(); // 0x005157D0
    // the text of the notification of the session (sent again to the friends)
    DECOMP_NOINLINE bool UpdateSessionText(const char16* text, u32 length); // 0x00515868
    // sends data to a station of the session with the reliable protocol
    DECOMP_NOINLINE bool Send(nn::pia::StationId stationId, const void* data, u32 size); // 0x00515A44
    // the work of the network thread: DataStore uploads, pia and the cached prints
    DECOMP_NOINLINE void Process(); // 0x00516364
    // rates an object (rating slots 0 and 1; 0: not rated)
    DECOMP_NOINLINE bool RateObject(u64 dataId, s32 rating0, s32 rating1); // 0x005167E4
    // http and the DataStore client
    DECOMP_NOINLINE bool InitializeDataStore(); // 0x00512BE8
    DECOMP_NOINLINE bool AutoMatchmake(const nn::pia::session::CreateSessionSetting& createSetting,
                                       const SessionSearchCriteria& criteria); // 0x00513120
    // searches the local sessions of the mode (sub id mode + 8) and fills the results
    DECOMP_NOINLINE void ScanLocal(u8 mode, ScanBufBase** results, u32 resultNum, u32* foundNum, bool withData); // 0x00511BA0
    // 0 or (0x80000000 | error type)
    DECOMP_NOINLINE s32 JoinSessionByOwner(const u32& ownerPid); // 0x00514090 | libgarden
    DECOMP_NOINLINE s32 JoinSessionById(u32 sessionId); // 0x00514BAC | libgarden
    DECOMP_NOINLINE bool CreateSession(u32 attribute); // 0x005149C4
    // the service token of the independent server
    DECOMP_NOINLINE bool RequestServiceToken(); // 0x00514AC4
    static void ResetSyncClockRequestInterval(); // 0x0051595C
    DECOMP_NOINLINE bool SendNotification103(u32 param1, u32 param2, const wchar_t* text); // 0x0051597C

    // DataStore
    DECOMP_NOINLINE bool SearchData(SearchDataResultList* results, const u16* dataTypes, TagFunction tagFunction,
                                    SearchOrder order); // 0x0050E8D0
    DECOMP_NOINLINE bool GetData(u64 dataId, SearchDataResultList* results, u16 dataType,
                                 TagFunction tagFunction); // 0x0050ED0C
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsConnectedToMatchmakeService() const // 0x00747D1C | libgarden
    {
        return m_pMatchmakeExtension != NULL && m_pMatchmakeExtension->IsConnected();
    }
    // the time of the pia session (0 outside of a session)
    DECOMP_EMIT_COPY DECOMP_NOINLINE s64 GetSessionTime() const // 0x00747D4C
    {
        nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
        if (session == NULL || session->GetStatus() != nn::pia::session::Session::STATUS_1) {
            return 0;
        }
        return nn::pia::session::Mesh::GetInstanceTime();
    }
    // the stations of the session (the rest of out is cleared); their number
    u8 GetInfo(ParticipantInfo* out, size_t count) const; // 0x00747D88 | libgarden (name)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasDataStore() const { return m_pDataStore != NULL; } // 0x00747EF4
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsNexSessionHost() const // 0x00747F04 (libgarden name)
    {
        return m_pMatchmakeExtension != NULL && nn::pia::session::Session::s_pInstance != NULL &&
               nn::pia::session::Session::s_pInstance->IsHost();
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE nn::pia::StationId GetLocalStationId() const // 0x00747F40
    {
        nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
        return session != NULL ? session->m_LocalStationId : s_InvalidStationId;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE nn::pia::StationId GetHostStationId() const // 0x00747F68
    {
        nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
        return session != NULL ? session->m_HostStationId : s_InvalidStationId;
    }
    // uploads an object (updates the object of dataId if it is one of ours, otherwise posts a new one)
    DECOMP_NOINLINE bool UploadData(const void* data, u32 dataSize, u64* dataId, const UploadInfo* info, const void* metaBinary,
                                    u32 metaBinarySize); // 0x0050EFC8
    DECOMP_NOINLINE bool UploadMetaBinary(u64* dataId, const UploadInfo* info, const void* metaBinary, u32 metaBinarySize); // 0x0050F278
    // the permission of an object becomes the default one; 0 or (0x80000000 | error type)
    DECOMP_NOINLINE s32 ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction); // 0x005164C0
    // the data of an object (size: its size)
    DECOMP_NOINLINE bool DownloadData(u64 dataId, void* buffer, u32 bufferSize, u32* size); // 0x00510AD8

    size_t SearchSessions(const SessionSearchCriteria& criteria, ISessionInfo** buffer, size_t bufferSize); // 0x0051173C | libgarden

    // the settings that the server sends (UtilityClient::GetIntegerSettings)
    static u32 GetServerSetting1(); // 0x0050D2E8
    static u32 GetServerSetting2(); // 0x0050D300

    // the memory functions of nex (from the NexHeap)
    static void* Allocate(u32 size); // 0x004DE29C
    static void Free(void* p); // 0x004DE280

private:
    // the global objects of nex and the pool of received data
    DECOMP_NOINLINE bool InitializeNexGlobals(); // 0x0050F3E4
    // (inline)
    void FinalizeNexGlobals();
    DECOMP_NOINLINE bool InitializeSocket(); // 0x005109E0
    // (inline) without a login and an independent server, the sockets and the access point go
    void FinalizeSocketIfUnused();
    DECOMP_NOINLINE bool Login(u32 gameServerId, const wchar_t* accessKey); // 0x00515F40
    DECOMP_NOINLINE void Logout(); // 0x005163A4
    // the meta data of an object, if it has the data type (0xFFFF: any) and the tags of tagFunction
    DECOMP_NOINLINE bool GetMetaChecked(u64 dataId, u16 dataType, TagFunction tagFunction,
                                        nn::nex::DataStoreMetaInfo* metaInfo); // 0x0050E638
    DECOMP_NOINLINE bool SearchObject(const nn::nex::DataStoreSearchParam& param,
                                      nn::nex::DataStoreSearchResult* result); // 0x0050F57C
    // resultOption 1: with the tags
    DECOMP_NOINLINE bool GetMeta(u64 dataId, nn::nex::DataStoreMetaInfo* metaInfo, u8 resultOption,
                                 bool reportsError); // 0x00510BF8
    // the next result of the list is set to the object; false when the list is full
    DECOMP_NOINLINE bool AddResult(const nn::nex::DataStoreMetaInfo& metaInfo, SearchDataResultList* results); // 0x00514144
    DECOMP_NOINLINE bool GetIntegerSettings(); // 0x00513BC8
    // reports a lost connection to the access point or to the server
    DECOMP_NOINLINE void CheckConnection(); // 0x0050FE28
    DECOMP_NOINLINE bool RateObjectSlot(u64 dataId, s8 slot, s32 ratingValue); // 0x005168B8
    // (inline) reports the last system error of nex (a call that could not be made) with the level
    void ReportSystemError(u8 level)
    {
        u32 error = nn::nex::SystemError::GetLastError();
        wchar_t description[128];
        nn::nex::SystemError::GetErrorDescription(error, description, 256);
        u32 errorCode = error != 0 ? nn::nex::ErrorCodeConverter::ConvertSystemErrorToNetworkErrorCode(error) : 0;
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(level, errorCode);
        }
        nn::nex::SystemError::ClearLastError();
    }
    DECOMP_NOINLINE bool GetObject(u64 dataId, void* buffer, u32 bufferSize); // 0x00510F80
    DECOMP_NOINLINE UpdateResult UpdateObject(u64 dataId, const void* data, u32 dataSize, const void* metaBinary, u32 metaBinarySize,
                                              const UploadInfo* info, const TagList* oldTags); // 0x0050F8A4
    // the new data id (0: failed)
    DECOMP_NOINLINE u32 PostObject(const void* data, u32 dataSize, const void* metaBinary, u32 metaBinarySize, const UploadInfo* info); // 0x0050FF7C
    DECOMP_NOINLINE s32 JoinSession(const nn::pia::session::JoinSessionSetting& setting); // 0x005148EC
    DECOMP_NOINLINE nn::Result JoinSessionImpl(const nn::pia::session::JoinSessionSetting& setting); // 0x00510DC0
    DECOMP_NOINLINE bool CreateSessionImpl(const nn::pia::session::CreateSessionSetting& setting); // 0x00515154
    DECOMP_NOINLINE bool CreateSessionAsync(const nn::pia::session::CreateSessionSetting& setting); // 0x00511A24
    // the join approval callback of the pia session
    static bool OnJoinApproval(const nn::pia::transport::Station::IdentificationInfo* info); // 0x005159CC
    // reports a failed pia call with the level
    DECOMP_NOINLINE void ReportPiaResult(u8 level, const nn::Result& result, u32 unused0, u32 unused1); // 0x00512704
    // the notifications of the friends that the server keeps (MatchmakeExtension method 10)
    DECOMP_NOINLINE void ReceiveNotifications(u32 type); // 0x00514634
    // a notification to the friends (MatchmakeExtension method 9)
    DECOMP_NOINLINE bool SendNotification(u32 type, u32 param1, u32 param2, const nn::nex::String& text); // 0x00514F2C
    // passes a notification event of the server on (also from the event handler)
    DECOMP_NOINLINE void OnNotificationEvent(const nn::nex::NotificationEvent& event, bool isFromHandler); // 0x00514BF8

public:
    sead::Heap* m_pHeap;                                // 0x000
    void* m_pCommonMemory;                              // 0x004 (of pia's common module)
    ISessionInfo** m_pSessionInfos;                     // 0x008 (16, of the local search)
    u8 m_ScanMode;                                      // 0x00C
    NotificationEventHandler m_NotificationEventHandler; // 0x010
    nn::nex::NgsFacade* m_pNgsFacade;                   // 0x014
    nn::nex::MatchmakeExtensionClient* m_pMatchmakeExtension; // 0x018
    u32 m_SessionId;                                    // 0x01C
    u32 m_OwnerPrincipalId;                             // 0x020 (of the session host)
    ReceivePool* m_pReceivePool;                        // 0x024
    bool m_HasNotification;                             // 0x028 (StoreNotification)
    MessageRecvHandler m_MessageRecvHandler;            // 0x02C
    nn::nex::MessagingClient* m_pMessaging;             // 0x030
    nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>* m_pDataStore; // 0x034
    nn::nex::IndependentServer* m_pIndependentServer;   // 0x038
    u32 m_GameServerId;                                 // 0x03C (of the login)
    bool m_IsSocketInitialized;                         // 0x040
    bool m_IsAcConnected;                               // 0x041
    bool m_IsLoggedIn;                                  // 0x042
    nn::pia::transport::ProtocolId m_ReliableProtocolId; // 0x044
    JoinApprovalCallback m_JoinApprovalCallback;        // 0x048
    StationCallback m_StationCallback;                  // 0x04C
    ReceiveCallback m_ReceiveCallback;                  // 0x050
    ErrorCallback m_ErrorCallback;                      // 0x054
    MessageCallback m_MessageCallback;                  // 0x058
    NotificationCallback m_NotificationCallback;        // 0x05C
    u32 unk_0x60;
    u32 m_LastSendSize;                                 // 0x064
    u8 m_SendSkipCount;                                 // 0x068 (Send fails this many times)
    bool m_IsSessionStarted;                            // 0x069
    bool m_IsSessionOpen;                               // 0x06A (open for participants)
    bool m_IsInSession;                                 // 0x06B
    char16 m_PlayerName[17];                            // 0x06C (the first 32 bytes are the pia player name)
    wchar_t m_SessionText[256];                         // 0x08E (the text of the notification of an opened session)
    u16 m_SessionParam;                                 // 0x28E
    sead::CriticalSection m_CriticalSection;            // 0x290

    static Impl* s_pInstance;
    static u32 s_ServerSetting0;
    static u32 s_ServerSetting1;
    static u32 s_ServerSetting2;
    static sead::ExpHeap* s_pNexHeap;
    // (0x00954698) the station id of index 253 (set by a static initializer)
    static nn::pia::StationId s_InvalidStationId;
    // (0x00954680) a value that CreateSession puts into attribute 3
    static u8 s_SessionAttribute3;
    // (0x00954681) RequestServiceToken got a token
    static bool s_HasServiceToken;
    // (0x00A24000) the data received with the reliable protocol
    static u8 s_ReceiveBuffer[0xE000];
    // (0x00A32000) the service token of the independent server
    static char s_ServiceToken[0x1000];
    // the memory of nn::socket
    static u64 s_SocketMemory[0x50000 / sizeof(u64)];
};
ASSERT_SIZE(Impl, 0x2AC);

// pia and nex get processing time (pia's scheduler and nex's), then the thread sleeps 8 ms (name is ours)
void Dispatch(); // 0x004DE2BC
} // namespace nex
} // namespace net
