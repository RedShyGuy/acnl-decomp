#pragma once

#include "decomp.h"

namespace netgame {
// The connections of the network layer (names after libgarden)
class ConnectMgr
{
public:
    enum Connection : u8
    {
        CONNECTION_LOCAL,
        CONNECTION_FRIEND_MATCH,
        CONNECTION_RANDOM_MATCH,
        CONNECTION_BEST_FRIEND,
        CONNECTION_DATASTORE,
        CONNECTION_INDEPENDENT,
        CONNECTION_NONE,
    };

    ConnectMgr(); // 0x0062A2C8
    void Process(); // 0x006297E0
    void Clear(); // 0x006297BC
    bool Connect(Connection connection); // 0x00629F10 | libgarden
    DECOMP_NOINLINE bool Disconnect(Connection connection); // 0x0062A0B0 | libgarden
    void SetPlayerName(); // 0x0062A280
    bool IsConnected(Connection connection) const; // 0x007601E0 | libgarden
    DECOMP_NOINLINE bool IsConnectedOrConnecting(Connection connection) const; // 0x007600CC
    bool IsConnectingOrDisconnecting(Connection connection) const; // 0x0076003C | libgarden
    // whether the running command is for the connection
    bool IsCurrentCmd(Connection connection, bool isDisconnect) const; // 0x00760198

private:
    // the bit of the connection, bit 7 = disconnect
    bool Enqueue(u8 cmd); // 0x0062A168
    void EraseCmd(s32 index);
    void EraseBestFriendDisconnect();
    void FinishCmd();

public:
    u8 m_CmdQueue[8];   // 0x0
    u8 m_CurrentCmd;    // 0x8
    u8 m_ConnectedFlags; // 0x9
    u8 m_Step;          // 0xA
    bool m_HasNetRef;   // 0xB
};
ASSERT_SIZE(ConnectMgr, 0xC);
} // namespace netgame
