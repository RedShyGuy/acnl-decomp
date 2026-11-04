#pragma once

#include "decomp.h"
#include "netgame/dAvatarId.h"
#include "netgame/dAvatarMgr.h"
#include "netgame/dConnectMgr.h"
#include "netgame/dFriendListMgr.h"
#include "netgame/dGateMgr.h"
#include "netgame/dIslandMgr.h"
#include "netgame/dJoinMgr.h"
#include "netgame/dLeaveMgr.h"
#include "netgame/dLockMgr.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/dNetGamePayload.h"
#include "netgame/dSaveBlock.h"
#include "Sv/dSvPersonalId.h"
#include "nn/os/os_CriticalSection.h"
#include "script/dWordFix.h"
#include "sead/seadSafeStringBase.h"
#include "netgame/dPacketQueue.h"
#include "netgame/dStageMgr.h"
#include "netgame/netgame_Api.h"
#include "sead/seadCriticalSection.h"

struct SvPlayer;

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// The network game: the managers of the session and the player tables (layout and most names after libgarden).
class NetGameMgr
{
public:
    class SingletonDisposer_;
    // (names after libgarden: Mgr8, FUN_00617b70; the others are ours)
    class Mgr8
    {
    public:
        void Clear(PlayerNo no); // 0x00617B60
        void FUN_00617b70(); // 0x00617B70 | libgarden
        void Reset(); // 0x00617BCC
        DECOMP_NOINLINE void ResetFlag(); // 0x00617BDC
        bool IsDone() const; // 0x0075ED9C

        u8 m_Data[4]; // 0x0
        u8 m_Flag;    // 0x4
        u8 unk_0x5[3];
    };
    ASSERT_SIZE(Mgr8, 8);

    // (names after libgarden: SaveDemoMgr, OnStartSave, m_State; the others are ours)
    class SaveDemoMgr
    {
    public:
        SaveDemoMgr(); // 0x006253F8
        void Process(); // 0x00624B28
        void RemovePlayer(PlayerNo player); // 0x006250C4
        // the next state after the packets of the frame
        void AdvanceAfterPackets(); // 0x006250D8
        void Reset(); // 0x00625264
        void OnStartSave(bool endSession); // 0x006252EC | libgarden

        // (for the admitted players)
        static void NotifyLeaveFinished(); // 0x00625114
        static bool CheckLeave(); // 0x00625164
        static void NotifySessionEnd(); // 0x006251C4
        static void NotifyEvent(); // 0x00625214

        u8 m_State;              // 0x0
        ConfirmMachine m_Confirm; // 0x4
        MachineBitTable m_Targets; // 0xC
        u8 m_CanDisconnect;      // 0xE
        u8 m_IsDone;             // 0xF
    };
    ASSERT_SIZE(SaveDemoMgr, 0x10);

    // (names are ours)
    class TransitionMgr
    {
    public:
        TransitionMgr(); // 0x0062A380
        void Reset(); // 0x0062A2E0
        void AllocBuffer(sead::Heap* heap); // 0x0062A300
        // appends the data to the buffer, if it fits
        void Add(const void* data, u32 size); // 0x0062A344

        static const u32 BUFFER_SIZE = 0x16800;

        u8* m_pBuffer;          // 0x0
        u32 m_BufferSize;       // 0x4
        PlayerNo m_EnteringPlayer; // 0x8
        PlayerNo m_ExitingPlayer;  // 0x9
        u8 m_State;             // 0xA
    };
    ASSERT_SIZE(TransitionMgr, 0xC);

    // the packets of the frame for one station (names are ours)
    class SendMgr
    {
    public:
        SendMgr(); // 0x00624914
        void ResetAll(); // 0x006248A4
        void AllocBuffers(sead::Heap* heap); // 0x006248D4
        bool IsBusy(PlayerNo no) const; // 0x0075F7C8
        void Reset(PlayerNo no); // 0x00624714
        // the packets on their way to the station, changed by delta
        void AddPending(PlayerNo no, s32 delta); // 0x00624764
        bool Commit(PlayerNo no, u32 size); // 0x006247D4
        void Prepare(PlayerNo no); // 0x00624808
        void AssignBuffers(PlayerNo myNo); // 0x00624820
        u8* GetBuffer(PlayerNo no); // 0x0075F7A8

        static const u32 BUFFER_SIZE = 0xD002;

        u8* m_pBuffer;      // 0x00
        u8* m_pBuffers[4];  // 0x04
        u32 m_Sizes[4];     // 0x14
        u8 m_Pending[4];    // 0x24
        bool m_IsAssigned;  // 0x28
    };
    ASSERT_SIZE(SendMgr, 0x2C);

    // the data that the host hands to a new player: blocks that functions of the game fill (names are ours)
    class EntryData
    {
    public:
        static const u32 BUFFER_SIZE = 0x89D;
        static const s32 SECTION_NUM = 2;
        static const s32 DATA_NUM = 3;

        typedef void (*FlagBuildFunction)(void* data, u32 no, u32 size);
        typedef void (*FlagApplyFunction)(u32 no, void* data, u32 size);
        typedef bool (*SectionBuildFunction)(void* data, u32 no, const void* param, u32 size);
        typedef void (*SectionApplyFunction)(u32 sender, const void* data, u32 size);
        typedef void (*DataSaveFunction)(void* data, u32 no, u32 size);
        typedef void (*DataLoadFunction)(u32 no, void* data, u32 size);

        EntryData(); // 0x0062613C
        void ClearSize(PlayerNo no); // 0x00625C78
        // packet 22 to the admitted guests
        void SendToGuests(const u16* param); // 0x00625C90
        void Save(u32 no); // 0x00625E24
        u8* GetFlag(PlayerNo no); // 0x00625E9C
        // sends the flag of the player to the host (inline in the original)
        DECOMP_ALWAYS_INLINE void SendFlag(PlayerNo no);
        void Load(u32 no); // 0x00625EC4
        void Receive(u32 sender, const u8* data, u32 size); // 0x00625F34
        void BuildFlag(PlayerNo no); // 0x00625FD0
        void ApplyFlag(PlayerNo no); // 0x0062602C
        void Reset(); // 0x00626090
        void AllocBuffers(sead::Heap* heap); // 0x006260A8

        u8* GetSendBuffer(PlayerNo no) { return m_pSendBuffers + (no - 1) * 6; }

        static const u32 s_BufferSize;
        static const u32 s_PlayerFlagSize;
        static const u32 s_FlagSize;
        static const u32 s_FlagOffset;
        static const FlagBuildFunction s_FlagBuildFunction;
        static const FlagApplyFunction s_FlagApplyFunction;
        static const u32 s_SectionSizes[SECTION_NUM];
        static const SectionBuildFunction s_SectionBuildFunctions[SECTION_NUM];
        static const SectionApplyFunction s_SectionApplyFunctions[SECTION_NUM];
        static const u32 s_DataSizes[DATA_NUM];
        static const u32 s_DataOffsets[DATA_NUM];
        static const DataSaveFunction s_DataSaveFunctions[DATA_NUM];
        static const DataLoadFunction s_DataLoadFunctions[DATA_NUM];

        u8* m_pBuffer;      // 0x00
        u8* m_pFlags;       // 0x04
        u8* m_pSendBuffers; // 0x08
        u32 m_Sizes[4];     // 0x0C
    };
    ASSERT_SIZE(EntryData, 0x1C);

    // the connection of the best friends and their texts (the name after libgarden: m_BestFriendMgr; the others are ours)
    class BestFriendMgr
    {
    public:
        enum SendState : u8
        {
            SEND_STATE_NONE,
            SEND_STATE_IDLE,
            SEND_STATE_SEND,
        };

        // a text of a best friend
        struct Message
        {
            Message(); // 0x0062BB64

            u32 principalId;   // 0x00
            SvPlayerId player; // 0x04
            u16 townId;        // 0x1A
            char16 townName[9]; // 0x1C
            char16 text[33];   // 0x2E
            u32 time[2];       // 0x70
            u8 isNotification; // 0x78
            bool isOwn;        // 0x79
        };
        ASSERT_SIZE(Message, 0x7C);

        // a ring of messages
        struct Queue
        {
            Queue() : m_pBuffer(NULL), m_Capacity(0), m_Read(0), m_Count(0) {}

            void Set(Message* buffer, s32 capacity)
            {
                m_pBuffer = buffer;
                m_Capacity = capacity;
                m_Read = 0;
                m_Count = 0;
            }
            void Clear()
            {
                m_Count = 0;
                m_Read = 0;
            }
            void Reset()
            {
                m_pBuffer = NULL;
                m_Capacity = 0;
                m_Count = 0;
                m_Read = 0;
            }
            s32 GetIndex(s32 index) const { return index >= m_Capacity ? index - m_Capacity : index; }
            const Message* Peek() const { return m_Count > 0 ? &m_pBuffer[GetIndex(m_Read)] : m_pBuffer; }
            const Message* At(s32 index) const { return index < m_Count ? &m_pBuffer[GetIndex(m_Read + index)] : m_pBuffer; }
            void Pop(Message* message)
            {
                if (m_Count > 0) {
                    *message = m_pBuffer[GetIndex(m_Read)];
                    m_Read++;
                    if (m_Read >= m_Capacity) {
                        m_Read = 0;
                    }
                    m_Count--;
                }
            }
            void PushBack(const Message* message)
            {
                if (m_Count < m_Capacity) {
                    s32 index = GetIndex(m_Count + m_Read);
                    m_Count++;
                    m_pBuffer[index] = *message;
                }
            }
            void PushFront(const Message* message)
            {
                if (m_Count >= m_Capacity && m_Count > 0) {
                    m_Count--;
                }
                if (m_Count < m_Capacity) {
                    m_Read--;
                    if (m_Read < 0) {
                        m_Read = m_Capacity - 1;
                    }
                    m_Count++;
                    m_pBuffer[GetIndex(m_Read)] = *message;
                }
            }

            Message* m_pBuffer; // 0x0
            s32 m_Capacity;     // 0x4
            s32 m_Read;         // 0x8
            s32 m_Count;        // 0xC
        };

        BestFriendMgr(); // 0x0062D340
        ~BestFriendMgr(); // 0x0062D3A4
        void Process(); // 0x0062BE08
        void ProcessConnect(); // 0x0062BBB4
        void Stop(); // 0x0062C7B8
        void AllocBuffers(sead::Heap* heap); // 0x0062BCD8
        void Reset(bool isForced); // 0x0062C740
        void OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification); // 0x0062C0E8
        void OnBestFriendOnline(const u32* principalId, const SvPersonalId* personalId); // 0x0062C964
        void ClearReceived(); // 0x0062C5E4
        bool PopMessage(Message* message); // 0x0062C61C
        bool PopToHistory(Message* message); // 0x0062C7D0
        DECOMP_NOINLINE void AddToHistory(const Message* message); // 0x0062C8B8
        void Enqueue(Message* message); // 0x0062CAF0
        void RequestDisconnect(bool disconnects); // 0x0062CBA4
        bool SendText(const sead::SafeStringBase<wchar_t>& text, const Principal* principal); // 0x0062CCF0
        void ClearAll(); // 0x0062CD58
        DECOMP_NOINLINE bool SendText(const sead::SafeStringBase<wchar_t>& text, const u32* principalId, const u16* playerId); // 0x0062CD94
        u32 GetHistory(Message* messages, u32 maxNum, u32 start, s32* totalNum, bool clearsNew); // 0x0062D268
        bool IsConnectPending() const; // 0x00760744
        s32 GetHistoryCount(); // 0x0076079C

    private:
        bool IsFromBestFriend(const Message* message);

    public:
        u32 m_ConnectTimer;   // 0x000
        u32 m_RetryCount;     // 0x004
        Queue m_Received;     // 0x008
        Queue m_History;      // 0x018
        nn::os::CriticalSection m_CriticalSection; // 0x028
        char16 m_HeaderText[214]; // 0x034
        char16 m_Text[256];   // 0x1E2
        u8 unk_0x3E2[2];
        u32 m_TargetPrincipalId; // 0x3E4
        SendState m_SendState;   // 0x3E8
        bool m_HasNewMessage;    // 0x3E9
        bool m_IsHistoryCut;     // 0x3EA
        bool m_IsConnectRequested; // 0x3EB
        bool m_IsDisconnecting;  // 0x3EC
        bool m_IsPaused;         // 0x3ED
        u8 unk_0x3EE[2];
    };
    ASSERT_SIZE(BestFriendMgr, 0x3F0);

    // the profiles that the players of a session exchange as friends (names are ours)
    class Mgr8B8
    {
    public:
        static const u32 PROFILE_SIZE = 0x200;

        struct Player
        {
            DECOMP_NOINLINE Player(); // 0x0062BAE0

            u8 flags;
        };

        Mgr8B8(); // 0x0062BAEC
        ~Mgr8B8(); // 0x0062BB3C
        void Add(PlayerNo no); // 0x0062B848
        void Process(); // 0x0062B8B0
        bool SendProfile(PlayerNo no); // 0x0062BA50

        u8 m_Profiles[4][PROFILE_SIZE]; // 0x000
        script::WordFix<9> m_Names[4];  // 0x800
        u8 m_Current;                   // 0x8B0
        Player m_Players[4];            // 0x8B1
    };
    ASSERT_SIZE(Mgr8B8, 0x8B8);

    // the packets of the other stations, one buffer each (names after libgarden: Receiver, GetBuffer, GetPacket,
    // SetIsEnabled, SetWasHandled, RECEIVE_BUFFER_SIZE, m_pBuffer; the others are ours)
    class Receiver
    {
    public:
        static const u32 RECEIVE_BUFFER_SIZE = 0x16802;

        Receiver(); // 0x00625B38
        void AllocBuffer(sead::Heap* heap); // 0x00625AF8
        void Reset(); // 0x00625AAC
        // the buffers of the three other players
        void AssignBuffers(PlayerNo myNo); // 0x00625A08
        void Clear(PlayerNo no); // 0x006259EC
        void SetWasHandled(u8 no, bool set); // 0x00625A8C | libgarden
        void SetIsEnabled(u8 no, bool set); // 0x00625A9C | libgarden
        DECOMP_EMIT_COPY DECOMP_NOINLINE void* GetBuffer(u8 no) // 0x0075FA00 | libgarden
        {
            if (no < PLAYER_NO_MAX) {
                return m_pBuffers[no];
            }
            return NULL;
        }
        DECOMP_EMIT_COPY DECOMP_NOINLINE bool WasHandled(u8 no) const // 0x0075FA14
        {
            if (no < PLAYER_NO_MAX) {
                return m_WasHandled[no];
            }
            return false;
        }
        // the packet behind the first two bytes of the buffer, if there is one
        DECOMP_EMIT_COPY DECOMP_NOINLINE const void* GetPacket(u8 from) // 0x0075FA28 | libgarden
        {
            if (from < PLAYER_NO_MAX && m_IsEnabled[from] && m_pBuffers[from] != NULL) {
                return m_pBuffers[from] + 2;
            }
            return NULL;
        }

        u8* m_pBuffer;       // 0x00
        u8* m_pBuffers[4];   // 0x04
        bool m_IsEnabled[4]; // 0x14
        bool m_WasHandled[4]; // 0x18
        bool m_IsAssigned;   // 0x1C
    };
    ASSERT_SIZE(Receiver, 0x20);

    // (names are ours)
    struct MatchingState
    {
        bool Invite(const u32* principalId, const wchar_t* text); // 0x006200E0
        void Add(const u32* principalId); // 0x00620178
        void Reset(); // 0x006202A4
        bool IsInvited(const u32* principalId) const; // 0x0075F224

        u32 m_Invited[3]; // 0x0
    };
    ASSERT_SIZE(MatchingState, 0xC);

    // the two bytes in front of a packet to a station (names are ours)
    struct PacketHead
    {
        u8 handled : 2;
        u8 isSystem : 1;
        u8 kind : 5;
        u8 counter;
    };
    ASSERT_SIZE(PacketHead, 2);

    enum Flag : u8
    {
        FLAG_PACKET_OVERFLOW = 1 << 0, // the packet buffer is full
        FLAG_STALLED = 1 << 1,         // nothing arrived from a station for a long time
        FLAG_COUNTER_MISMATCH = 1 << 3, // a packet counter jumped
        FLAG_STATION_LOST = 1 << 4,    // a station of the game is gone
    };

    NetGameMgr(); // 0x00619B1C
    ~NetGameMgr(); // 0x00619D70

    void OnConnectBestFriend(); // 0x0062C5D8
    bool PopBestFriendMessage(BestFriendMgr::Message* message); // 0x0062C610
    bool SendBestFriendText(const sead::SafeStringBase<wchar_t>& text, const Principal* principal); // 0x0062CCE4
    void ClearBestFriendMessages(); // 0x0062CD4C
    void StartEntry(); // 0x006218E8
    void StartLeave(); // 0x00624538
    void StartSave(bool endSession); // 0x00625284
    DECOMP_NOINLINE SaveBlock* GetSaveBlock(PlayerNo player); // 0x0075EEF8
    void BuildPayloadA(); // 0x00626794
    void BuildPayloadD(u32 param); // 0x00626A80
    void BuildPayloadE(u32 param); // 0x00626FD4

    bool IsEntering(PlayerNo player); // 0x00617BF8
    bool InitNet(); // 0x00617C88 | libgarden (the function netgame::InitNet)
    void CheckError(); // 0x00617D20
    bool StopNetAndConnection(); // 0x00617ED8
    void FiniNet(); // 0x00617FD4 | libgarden
    void Update(); // 0x00618024
    bool InitNetForConnect(); // 0x006181FC
    bool SendPacketBatch(PlayerNo target, const void* const* parts, const size_t* partSizes, size_t partCount, u8 type, u32 unknown1,
                         u32 unknown2, u32 unknown3); // 0x00618318 | libgarden
    bool ProcessLeave(); // 0x006184A8
    DECOMP_NOINLINE bool SendPacket(PlayerNo player, const void* const* parts, const size_t* partSizes, size_t partCount, u8 type, u32 unknown1,
                    u32 unknown2, u32 unknown3); // 0x006184E0
    bool ConnectFriendMatch(); // 0x0061875C
    void ProcessEnd(); // 0x006187E0
    bool DisconnectCurrent(); // 0x0061886C
    bool ConnectLocal(); // 0x00618978
    void OnSessionStarted(u32 unknown, bool isRestart); // 0x00618A68
    void RemovePlayer(PlayerNo player); // 0x00618C3C
    void CheckReceiveCounter(PlayerNo player, u8 counter, bool checks); // 0x00618DCC
    bool CanSend(PlayerNo player, bool hasData, u32 unknown); // 0x006199A4
    void InitEntryOrExit(bool isExit); // 0x006196D8 | libgarden
    bool ConnectRandomMatch(); // 0x0061937C | libgarden

    u8 GetPlayerCount() const; // 0x0075EFF8 | libgarden
    DECOMP_NOINLINE PlayerNo GetMyPlayerNo() const; // 0x0075F004 | libgarden
    bool IsAdmitted(PlayerNo player) const; // 0x0075F02C | libgarden
    bool IsMultiPlayer() const; // 0x0075F064 | libgarden
    bool IsInSession(PlayerNo player, bool skipSelf) const; // 0x00760658 | libgarden
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsExisting(PlayerNo player) const { return m_Bits[1].IsSet(player); } // 0x0075EFD0
    bool IsStageControl() const; // 0x0075EE90
    DECOMP_NOINLINE bool IsStageControl(stage::Name stage) const; // 0x0075EE14
    bool WritePacketHead(u8** buffer, PlayerNo player, bool isSystem, u32 kind); // 0x0075EF1C
    u32 GetBestFriendHistory(BestFriendMgr::Message* messages, u32 maxNum, u32 start, s32* totalNum, bool clearsNew); // 0x006181D4
    bool SendBestFriendTextToAll(const sead::SafeStringBase<wchar_t>& text); // 0x006182FC
    DECOMP_NOINLINE void OnPlayerEntered(); // 0x006274A8
    DECOMP_NOINLINE void LoadEntry(); // 0x00627574
    bool StopNet(); // 0x00619408
    void ResetSaveBlock(); // 0x00618E5C
    bool CollectGuests(MachineBitTable* targets, bool hasData, u32 unknown); // 0x00618E90
    bool CollectPlayers(MachineBitTable* targets, bool hasData, u32 unknown); // 0x00618F28
    s32 GetLockResult(); // 0x00618FB0
    bool RequestLock0(); // 0x0061900C
    bool IsJoinDone() const; // 0x0061903C
    void AssignPlayer(bool keepsPlayers); // 0x00619050
    bool RequestLock1(); // 0x0061912C
    bool CreateOrJoinRandomSession(u32 param); // 0x0061915C
    void SendSessionClosed(u8 isClosed); // 0x00619190
    bool CreateSession(const char16* text, u32 length, u16 param); // 0x00619200
    bool RequestLock2(); // 0x00619310
    bool IsJoining() const; // 0x0061933C
    bool RequestLock3(); // 0x0061954C
    bool RequestLock4(); // 0x006194A8
    bool JoinSession(u32 sessionId); // 0x006194F4
    bool JoinSessionByOwner(const u32* principalId); // 0x0061957C
    bool RequestLock5(); // 0x006195E0
    bool RequestLock6(); // 0x00619674
    bool RequestLock7(); // 0x006195B0
    bool CreateRandomSession(u32 param, bool isRestart); // 0x00619610
    bool IsJoinStarted() const; // 0x006196C4
    void AllocBuffers(sead::Heap* heap); // 0x006198AC
    void ResetSession(); // 0x00619A88

    // whether the packet queue is closed
    DECOMP_EMIT_COPY static s8 IsPacketQueueClosed() { return s_pInstance->m_PacketQueue.m_IsClosed; } // 0x00625568

    static NetGameMgr* createInstance(sead::Heap* heap); // 0x00618294

    static NetGameMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u8 unk_0x0[0x10];
    PacketQueue m_PacketQueue;   // 0x00010
    Receiver m_Receiver;         // 0x00058
    SendMgr m_SendMgr;           // 0x00078
    TransitionMgr m_TransitionMgr; // 0x000A4
    AvatarMgr m_AvatarMgr;       // 0x000B0
    StageMgr m_StageMgr;         // 0x001C0
    LockMgr m_LockMgr;           // 0x001E4
    JoinMgr m_JoinMgr;           // 0x00214
    SaveDemoMgr m_SaveDemoMgr;   // 0x0022C
    LeaveMgr m_LeaveMgr;         // 0x0023C
    Mgr8 m_Mgr8;                 // 0x0277C
    EntryData m_EntryData;       // 0x02784
    IslandMgr m_IslandMgr;       // 0x027A0
    BestFriendMgr m_BestFriendMgr; // 0x028EC
    Mgr8B8 m_Mgr8B8;             // 0x02CDC
    FriendListMgr m_FriendListMgr; // 0x03594
    PayloadA m_PayloadA;         // 0x07840
    PayloadB m_PayloadB;         // 0x0784C
    PayloadC m_PayloadC;         // 0x07858
    PayloadD m_PayloadD;         // 0x07864
    PayloadE m_PayloadE;         // 0x0786C
    ConnectMgr m_ConnectMgr;     // 0x07874
    GateMgr m_GateMgr;           // 0x07880
    MatchingState m_MatchingState; // 0x07888
    SaveBlock m_SaveBlock;       // 0x07894
    SaveBlock m_SaveBlocks[4];   // 0x09DB4
    NpcInfo m_NpcInfo;           // 0x13234
    ConnectMgr::Connection m_CurrentConnection; // 0x13266
    ConnectMgr::Connection m_LastConnection;    // 0x13267
    PlayerNo m_MyPlayerNo;       // 0x13268
    u8 unk_0x13269;
    u8 m_Flags;                  // 0x1326A
    u8 m_ErrorFlags;             // 0x1326B
    MachineBitTable m_Bits[7];   // 0x1326C
    MachineBitTable m_Entry;     // 0x1327A
    MachineBitTable m_ExistingPlayers; // 0x1327C
    MachineBitTable m_Entry2;    // 0x1327E
    sead::CriticalSection m_CriticalSection; // 0x13280
    u8 m_SaveNo;                 // 0x1329C
    Type m_Type;                 // 0x1329D
    u8 unk_0x1329E[2];
    s32 m_NetRefCount;           // 0x132A0
    u8 m_SendCounters[4];        // 0x132A4
    u8 m_ReceiveCounters[4];     // 0x132A8
    bool m_IsInNetSession;       // 0x132AC
    bool m_IsInNetgame;          // 0x132AD
    bool m_IsPlayerAssigned;     // 0x132AE
    bool unk_0x132AF;
    bool m_IsConnectRandomMatch; // 0x132B0
    bool m_IsSessionClosedForIsland; // 0x132B1
    u8 unk_0x132B2;
    bool m_NewPlayerJoined;      // 0x132B3
    bool unk_0x132B4;
    u8 unk_0x132B5;
    bool unk_0x132B6;
    bool unk_0x132B7;
};
ASSERT_SIZE(NetGameMgr, 0x132B8);

// whether the game is saving (function of the game that is not decompiled yet; name is ours)
bool IsSaving(); // 0x005CF1AC
extern s32 g_FrameCounter;
bool ConnectBestFriendIfAllowed(); // 0x0062CC24
void MakeParam(u32* param, u16 low, u32 high); // 0x00618E44
s32 GetLockResult(); // 0x00626D24
void BuildSessionData(const SvPersonalId* players); // 0x006295E4
void ReadSessionPlayers(const void* data, SvPersonalId* players); // 0x00629024
void CopyNpcInfo(void* npcs, void* info, u32 unknown); // 0x00119184 (Thumb)
void LoadNpcInfo(void* info, PlayerNo player); // 0x0051DDBC
void SetPlayerActive(u32 unknown, u32 saveNo, SvPlayer* player); // 0x005C441C
void ClearPlayer(u32 no); // 0x005C4ED0
u8 GetDefaultSaveNo(u32 unknown); // 0x002FF6CC

// the session ends for the player (table = the others; names are ours)
void OnPlayerLeaveFinished(PlayerNo player, const MachineBitTable* others); // 0x006276B0
void OnSessionEnd(PlayerNo player, const MachineBitTable* others); // 0x00627704
// whether the player is in the session
DECOMP_NOINLINE bool IsInSession(PlayerNo player, bool skipSelf); // 0x00624B10

DECOMP_ALWAYS_INLINE void NetGameMgr::EntryData::SendFlag(PlayerNo no)
{
    BuildFlag(no);
    u8* flag = GetFlag(no);
    if (flag != NULL) {
        PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(flag, s_PlayerFlagSize);
        queue.EndPacket(23, PLAYER_NO_HOST);
    }
    Reset();
}
} // namespace netgame
