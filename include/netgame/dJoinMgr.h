#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"

namespace netgame {
// the bytes of the first part of the save data in front of the save itself
extern u32 s_SaveHeaderSize;

// The entry of a new player (names after libgarden: JoinMgr, StartEntry, m_State; the others are ours)
class JoinMgr
{
public:
    JoinMgr(); // 0x00621940
    void Reset(); // 0x006218A4
    void Process(); // 0x006208E4
    void RemovePlayer(PlayerNo player); // 0x00621138
    DECOMP_NOINLINE void StartEntry(); // 0x006218F0 | libgarden
    void SetPlayerFlag(PlayerNo player, bool isSet); // 0x00621150
    // whether all the others that take part have the flag
    bool IsPlayerFlagSet() const; // 0x0075F298

private:
    // the data of the host for a new player of a town: player, flags, save block, NPCs, stage
    bool SendTownData(); // 0x00621160
    // the save data in parts; true when all of it is sent
    bool SendSaveData(PlayerNo target); // 0x00621364
    bool SendRandomMatchData(PlayerNo target); // 0x0062167C

public:
    u8 m_State;            // 0x00
    u8 unk_0x1[3];
    u32 m_SendOffset;      // 0x04 (of the save data)
    stage::Name m_Stage;   // 0x08
    bool m_PlayerFlags[4]; // 0x09
    u8 unk_0xD;
    u8 unk_0xE;
    u8 unk_0xF;
    u8 unk_0x10;
    u8 unk_0x11;
    u8 unk_0x12;
    u8 unk_0x13;
    u8 unk_0x14;
    u8 unk_0x15;
    u8 unk_0x16;
    u8 unk_0x17;
};
ASSERT_SIZE(JoinMgr, 0x18);
} // namespace netgame
