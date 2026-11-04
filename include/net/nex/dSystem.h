#pragma once

#include "decomp.h"
#include "net/nex/dCommand.h"
#include "net/nex/dError.h"
#include "net/nex/dImpl.h"
#include "net/dSender.h"
#include "nn/os/os_CriticalSection.h"

namespace net {
class SystemCallback;

namespace nex {
// The network layer on the network thread: a state machine that works on the command of the
// Framework with the Impl and passes the events of the Impl to the SystemCallback of the game (names
// after libgarden: System, State and its values, m_Section, m_Impl, m_pCmd, m_Error, m_StateFn, m_State,
// m_pSender, m_pCallback, m_LocalStationId, m_Participants, m_ParticipantCount, CmdSuccess, CmdFailed;
// the others are ours)
class System
{
public:
    enum State : u8
    {
        NONE,
        IDLE,
        CONNECTED_LOCAL,
        LOCAL_SESSION,
        LOCAL_ERROR,
        INDEPENDENT,
        CONNECTED_NEX,
        NEX_SESSION,
        STATE_NUM,
    };

    typedef void (System::*StateFunction)();

    System(Sender* sender, SystemCallback* callback); // 0x0051897C
    DECOMP_EMIT_COPY DECOMP_NOINLINE ~System() // 0x0013881C
    {
        s_pInstance = NULL;
        s_FrameCount = 0;
    }
    // the callbacks of the Impl and the state IDLE
    bool Start(); // 0x00516C24
    // back to the state NONE (the NexHeap goes)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Stop() // 0x001387D4
    {
        if (m_State != NONE) {
            m_Impl.DestroyNexHeap();
            ChangeState(NONE);
            s_FrameCount = 0;
        }
    }
    // clears the error and the participants (and the requests of the sender)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Reset() // 0x00138740
    {
        m_Error.Clear();
        m_LocalStationId = Impl::s_InvalidStationId;
        for (s32 i = 0; i < 4; i++) {
            m_Participants[i].stationId = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
            m_Participants[i].rtt = 0;
        }
        m_ParticipantCount = 0;
        if (m_pSender != NULL) {
            m_pSender->Reset();
        }
    }
    // a command to work on (false while one runs or after an error)
    bool SetCommand(Command* command); // 0x00516CC8
    // the state of the matchmaking for the game; the error is passed on and cleared
    void GetMatchInfo(MatchInfo* info, Error* error); // 0x00516D30
    // the work of the network thread
    void Update(); // 0x005186B0

    State GetState() const { return m_State; }

    void CmdSuccess()
    {
        m_pCmd->SetDone();
        m_pCmd = NULL;
    }
    void CmdFailed()
    {
        m_pCmd->SetFailed();
        m_pCmd = NULL;
    }

private:
    // the state functions
    void StateNone(); // 0x00517258
    void StateIdle(); // 0x0051701C
    void StateConnectedLocal(); // 0x005172AC
    void StateLocalSession(); // 0x00518104
    void StateLocalError(); // 0x0051827C
    void StateIndependent(); // 0x005185B4
    void StateConnectedNex(); // 0x0051754C
    void StateNexSession(); // 0x005182D4

    void ChangeState(State state)
    {
        m_StateFn = s_StateFunctions[state];
        m_State = state;
    }
    // the command fails with an error of the type
    void FailCommand(Command::ID command, u8 type)
    {
        m_Section.Enter();
        m_Error.Set(command, type);
        m_Section.Exit();
        CmdFailed();
    }

    // the callbacks of the Impl
    static bool OnJoinApproval(const char16* name, u32 length); // 0x00516F68
    static void OnStationEvent(nn::pia::StationId stationId, bool isJoined, const char16* name, u32 length); // 0x00516ECC
    static void OnReceive(nn::pia::StationId stationId, const void* data, u32 size); // 0x00516FDC
    static void OnError(u8 level, u32 errorCode); // 0x00516E70
    static void OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification); // 0x00516F9C
    static void OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                               bool isFromHandler); // 0x005180A4
    // sends a request of the Sender
    static bool Send(const nn::pia::StationId* stationId, const void* data, u32 size); // 0x00516D04

public:
    nn::os::CriticalSection m_Section;         // 0x000
    Impl m_Impl;                               // 0x00C
    Command* m_pCmd;                           // 0x2B8
    Error m_Error;                             // 0x2BC
    StateFunction m_StateFn;                   // 0x2C8
    State m_State;                             // 0x2D0
    Sender* m_pSender;                         // 0x2D4
    SystemCallback* m_pCallback;               // 0x2D8
    nn::pia::StationId m_LocalStationId;       // 0x2DC
    u32 unk_0x2E4;
    ParticipantInfo m_Participants[4];         // 0x2E8
    u8 m_ParticipantCount;                     // 0x328
    bool m_NotifiesFriends;                    // 0x329 (the session is announced to the friends)
    u32 unk_0x32C;

    // (0x00954678) counts the calls of Update
    static u32 s_FrameCount;
    // (0x0095467C)
    static System* s_pInstance;
    static const StateFunction s_StateFunctions[STATE_NUM]; // 0x0087CDBC
};
ASSERT_SIZE(System, 0x330);
} // namespace nex
} // namespace net
