#pragma once

#include "decomp.h"
#include "net/nex/dCommand.h"
#include "net/nex/dError.h"
#include "net/nex/dImpl.h"
#include "net/nex/dSystem.h"
#include "nn/nex/nex_Pacer.h"
#include "nn/os/os_CriticalSection.h"
#include "sead/seadDelegate2.h"
#include "sead/seadDelegateThread.h"

namespace net {
class Sender;
class SearchDataResultList;
class ScanBufBase;
class SystemCallback;
class TagWriter;

namespace nex {
// The interface of the game to the network: each function hands one command to the network thread
// ("NetCommandThread", 30 frames per second) and is called again every frame until the command is done
// (true) or failed (false, the error is kept). The thread runs the System and publishes its state
// (MatchInfo, Error), Update takes it over for the game. (Names after libgarden: Framework,
// ProcessCommand, CreateOrJoinRandomSession, m_Section, m_pSystem, m_pCommandThread, m_pPacer, m_State,
// m_Command, m_MatchInfo, m_Error; the others are ours.)
class Framework
{
public:
    enum State : u8
    {
        UNINITIALIZED = 0,
        READY = 1,     // the thread runs
        LOGGED_IN = 2, // connected (local or internet)
        STATE_3 = 3,   // hosts a session
        STATE_4 = 4,   // in a local session of another station
        STATE_5 = 5,   // in a session over the internet
    };

    // the connections (m_ConnectFlags)
    enum ConnectFlag
    {
        CONNECT_FLAG_SESSION = 1 << 0,     // local or matchmaking (CONNECT_LOCAL/CONNECT_NEX)
        CONNECT_FLAG_NEX = 1 << 1,         // matchmaking over the internet
        CONNECT_FLAG_DATASTORE = 1 << 2,   // CONNECT_DATASTORE
        CONNECT_FLAG_INDEPENDENT = 1 << 3, // CONNECT_INDEPENDENT
    };

    typedef void (*TagFunction)(TagWriter* writer);

    Framework(); // 0x00519D54
    ~Framework(); // 0x00519E64

    // the System and the network thread
    bool Initialize(Sender* sender, SystemCallback* callback); // 0x00518A68
    // stops the thread and deletes the System (not while the independent server is connected)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Finalize() // 0x001329E4
    {
        if ((m_ConnectFlags & CONNECT_FLAG_INDEPENDENT) || m_State == UNINITIALIZED) {
            return;
        }
        if (m_pCommandThread != NULL) {
            m_pCommandThread->quitAndWaitDoneSingleThread(false);
            delete m_pCommandThread;
            m_pCommandThread = NULL;
        }
        if (m_pPacer != NULL) {
            ::delete m_pPacer;
            m_pPacer = NULL;
        }
        m_pSystem->Stop();
        delete m_pSystem;
        m_pSystem = NULL;
        m_State = UNINITIALIZED;
    }
    // clears the published state and the errors
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Reset() // 0x00132994
    {
        m_Section.Enter();
        m_MatchInfoThread.Clear();
        m_MatchInfo.Clear();
        m_ErrorThread.Clear();
        m_Error.Clear();
        m_pSystem->Reset();
        m_Section.Exit();
        m_IsInternet = false;
    }
    // takes over the state of the network thread (every frame)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Update() // 0x001364B4
    {
        m_Section.Enter();
        m_MatchInfo = m_MatchInfoThread;
        if (m_ErrorThread.m_Level != 0) {
            m_Error.Merge(m_ErrorThread);
            m_ErrorThread.Clear();
        }
        if (m_Command.id != Command::NONE && m_Command.processState == 2) {
            m_Command.processState = 0;
        }
        m_Section.Exit();
    }
    // hands the command to the thread or checks the running one: true once it is done
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool ProcessCommand(Command::ID id, const void* param = NULL) // 0x00132AC8 | libgarden
    {
        if (m_Command.id == Command::NONE) {
            m_Error.ClearRecoverable();
            if (IsError()) {
                return false;
            }
            m_Command.id = id;
            if (param != NULL) {
                m_Command.param = *static_cast<const Command::Param*>(param);
            }
            m_Command.processState = 1;
            m_Command.status = Command::NOT_DONE;
            m_pCommandThread->sendMessage(1, sead::MessageQueue::BLOCK_TYPE_BLOCK);
        }
        if (m_Command.id == id && m_Command.processState == 0) {
            u8 status = m_Command.status;
            m_Command.id = Command::NONE;
            if (status == Command::DONE) {
                return true;
            }
        }
        return false;
    }
    // the recoverable error is cleared
    DECOMP_EMIT_COPY DECOMP_NOINLINE void ClearError() { m_Error.ClearRecoverable(); } // 0x00138844
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool ResetSyncClock() // 0x00132A90
    {
        if (m_Command.id == Command::NONE && (m_IsBlocked || m_State <= LOGGED_IN)) {
            return true;
        }
        return ProcessCommand(Command::RESET_SYNC_CLOCK);
    }
    // leaves the session and closes all connections, then stops the thread
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool DisconnectAll() // 0x0012C0E8
    {
        Command::ID id = m_Command.id;
        if (id != Command::NONE && id != Command::LEAVE_SESSION && id != Command::LEAVE_SESSION_14 &&
            id != Command::DISCONNECT && id != Command::DISCONNECT_DATASTORE) {
            if (!CheckCommand()) {
                return false;
            }
        }
        if (!m_IsBlocked && m_State > LOGGED_IN) {
            bool isDone = m_State == STATE_3 ? ProcessCommand(Command::LEAVE_SESSION_14) : ProcessCommand(Command::LEAVE_SESSION);
            if (!isDone) {
                return false;
            }
            m_State = LOGGED_IN;
        }
        if (m_ConnectFlags & CONNECT_FLAG_INDEPENDENT) {
            if (!ProcessCommand(Command::DISCONNECT_INDEPENDENT)) {
                return false;
            }
            m_ConnectFlags &= ~CONNECT_FLAG_INDEPENDENT;
        }
        if (m_ConnectFlags & CONNECT_FLAG_DATASTORE) {
            if (!ProcessCommand(Command::DISCONNECT_DATASTORE)) {
                return false;
            }
            m_ConnectFlags &= ~CONNECT_FLAG_DATASTORE;
            if (!(m_ConnectFlags & CONNECT_FLAG_SESSION) && !(m_ConnectFlags & CONNECT_FLAG_NEX) &&
                !(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
                m_State = READY;
                Reset();
            }
        }
        if (m_ConnectFlags & CONNECT_FLAG_SESSION) {
            if (!ProcessCommand(Command::DISCONNECT)) {
                return false;
            }
            if (m_IsInternet) {
                m_ConnectFlags &= ~CONNECT_FLAG_NEX;
            }
            m_ConnectFlags &= ~CONNECT_FLAG_SESSION;
            m_State = READY;
            Reset();
        }
        Finalize();
        return true;
    }
    DECOMP_EMIT_COPY bool IsLocal() const // 0x0012E378
    {
        return !m_IsBlocked && m_State > READY && !m_IsInternet;
    }

    // the commands (true when done, false while running or after a failure)
    bool RateObject(u64 dataId, s8 rating0, s8 rating1); // 0x00518AF0
    bool SearchSessions(ScanBufBase** results, u32 resultNum, u32* foundNum, s8 withData, u8 mode); // 0x00518B78
    bool DisconnectIndependent(); // 0x00518CB4
    bool CreateSession(const char16* text, u32 length, u16 param, u8 mode); // 0x00518D14
    bool SearchData(SearchDataResultList* results, const u16* dataTypes, u32 dataTypeNum, TagFunction tagFunction,
                    u8 order); // 0x00518DD8
    bool GetData(u64 dataId, SearchDataResultList* results, u16 dataType, TagFunction tagFunction); // 0x00518ED0
    bool ConnectIndependent(u32 param); // 0x00518F7C
    bool UploadData(const void* data, u32 size, void* work, u64* dataId, const void* info); // 0x0051906C
    bool CallMatchmakeMethod25(const u32* param, const wchar_t* text); // 0x005190DC
    bool DisconnectDataStore(); // 0x00519164
    bool ConnectDataStore(u32 param); // 0x005191C4
    bool UploadData2(const void* data, u32 size, void* work, const void* info); // 0x005192C4
    bool DownloadData(u64 dataId, void* buffer, u32 size, void* work, u32* length); // 0x00519330
    bool JoinSessionByOwner(const u32* principalId); // 0x005193AC
    bool CreateOrJoinRandomSession(u32 param); // 0x00519444 | libgarden
    bool CreateRandomSession(u32 param); // 0x005194D4
    bool ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction); // 0x00519564
    bool SetSessionData(const char16* data, u32 length); // 0x00519608
    // the friend code of a station of the session, directly
    void GetFriendCodeDirect(const nn::pia::StationId* stationId, u64* friendCode); // 0x005196A0
    bool SendNotification102(const wchar_t* text); // 0x005196D0
    bool SendTextMessage(const u32* principalId, const wchar_t* text); // 0x0051975C
    // ends the command that is done (also when the state of the game was not waiting for it)
    bool CheckCommand(); // 0x005197F0
    void SetPlayerName(const char16* name, u32 length); // 0x00519870
    bool GetFriendCode(const nn::pia::StationId* stationId, u64* friendCode); // 0x00519888
    bool SendNotification103(u32 param1, u32 param2, const wchar_t* text); // 0x00519928
    bool OpenSession(s8 setParam, u16 param); // 0x005199A0
    bool LeaveSession(); // 0x00519A2C
    bool CloseSession(); // 0x00519AA0
    bool Disconnect(); // 0x00519AF4
    bool JoinSession(u32 sessionId); // 0x00519B64
    bool Connect(bool isLocal); // 0x00519BF4 | libgarden (Net::Connect)
    bool ModifySessionAttribute(const nn::pia::StationId* stationId); // 0x00519D08

    // the state for the game
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetFrameCount() const { return m_MatchInfo.instanceId; } // 0x00747F90
    DECOMP_EMIT_COPY DECOMP_NOINLINE const nn::pia::StationId& GetLocalStationId() const { return m_MatchInfo.localStation; } // 0x00747F98
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsParticipant(const nn::pia::StationId& stationId) const // 0x00747FA0
    {
        if (stationId == nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF)) {
            return false;
        }
        for (s32 i = 0; i < 4; i++) {
            if (m_MatchInfo.participants[i].stationId == stationId) {
                return true;
            }
        }
        return false;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetErrorCode() const // 0x00748030
    {
        if (m_Error.m_Level != 0 && m_Command.id == Command::NONE) {
            return m_Error.m_ErrorCode;
        }
        return 0;
    }
    // the kind of the error for the game (3: none)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetErrorKind() const // 0x00748054
    {
        if (m_Error.m_Level == 0 || m_Command.id != Command::NONE) {
            return 3;
        }
        if (m_Error.m_Level == 4) {
            return 0;
        }
        if (m_Error.m_Level == 2 || m_Error.m_Level == 3) {
            return 1;
        }
        switch (m_Error.m_Type) {
        case 0:
            return 2;
        case 1:
            return 4;
        case 2:
            return 5;
        case 3:
            return 6;
        case 4:
            return 7;
        case 5:
            return 8;
        default:
            return 3;
        }
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetErrorLevel() const { return m_Error.m_Level; } // 0x007480E8
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsInitialized(bool ignoresBlock) const // 0x007480F0
    {
        if (m_IsBlocked && !ignoresBlock) {
            return false;
        }
        return m_State != UNINITIALIZED;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE s64 GetSessionTime() const { return m_MatchInfo.time; } // 0x00748118
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsBlocked() const { return m_IsBlocked; } // 0x00748128
    DECOMP_EMIT_COPY DECOMP_NOINLINE const nn::pia::StationId& GetHostStationId() const { return m_MatchInfo.hostStation; } // 0x00748134
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool HasNotification(u8 mask) const { return (m_MatchInfo.notif3Received & mask) != 0; } // 0x0074813C
    // the round trip time to the station (0 if it is not in the session)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetRtt(const nn::pia::StationId& stationId) const // 0x0074814C
    {
        for (s32 i = 0; i < 4; i++) {
            if (m_MatchInfo.participants[i].stationId == stationId) {
                return m_MatchInfo.participants[i].rtt;
            }
        }
        return 0;
    }
    // an error and no command
    DECOMP_EMIT_COPY bool IsError() const { return m_Error.m_Level != 0 && m_Command.id == Command::NONE; } // 0x007481C8
    DECOMP_EMIT_COPY bool IsLoggedIn(bool ignoresBlock) const // 0x007481EC
    {
        if (m_IsBlocked && !ignoresBlock) {
            return false;
        }
        return m_State > READY;
    }
    DECOMP_EMIT_COPY bool IsHost() const // 0x00748218
    {
        return m_State == STATE_3 || (m_State == STATE_5 && m_MatchInfo.localStation == m_MatchInfo.hostStation);
    }
    DECOMP_EMIT_COPY bool IsInternet(bool ignoresBlock) const // 0x0074825C
    {
        if (m_IsBlocked && !ignoresBlock) {
            return false;
        }
        return m_State > READY && m_IsInternet;
    }

private:
    // starts the pacer and the network thread
    void CreateThread(); // 0x00518BFC
    // the network thread: a new command on message 1, then one frame of the System
    void ThreadFunc(sead::Thread* thread, s32 message); // 0x003CE9D0

public:
    sead::Delegate2<Framework, sead::Thread*, s32> m_Delegate; // 0x000
    nn::os::CriticalSection m_Section;                        // 0x010
    System* m_pSystem;                                        // 0x01C
    sead::DelegateThread* m_pCommandThread;                   // 0x020
    nn::nex::Pacer* m_pPacer;                                 // 0x024
    State m_State;                                            // 0x028
    u32 unk_0x2C;
    Command m_Command;                                        // 0x030
    MatchInfo m_MatchInfoThread;                              // 0x068 (written by the thread)
    MatchInfo m_MatchInfo;                                    // 0x0D0
    Error m_ErrorThread;                                      // 0x138 (written by the thread)
    Error m_Error;                                            // 0x144
    u32 m_ConnectFlags;                                       // 0x150
    bool m_IsInternet;                                        // 0x154 (connected over the internet, not locally)
    bool m_IsBlocked;                                         // 0x155 (never set: it would refuse new connections)
};
ASSERT_SIZE(Framework, 0x158);
} // namespace nex
} // namespace net
