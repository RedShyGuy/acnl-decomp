#pragma once

#include "decomp.h"
#include "Player/dPlayerApi.h"
#include "Sv/dSvBestFriendList.h"
#include "Sv/dSvGardenPlus.h"
#include "netgame/netgame_Api.h"

// The access functions of the save data (game code). The save file garden_plus.dat lies in memory behind
// g_SvSaveMemory; the players of other towns in a network game lie in g_SvNetPlayerList.

// the save file in memory (names are ours; the save code of the game allocates the copy)
struct SvSaveMemory
{
    SvGardenPlus* pData;    // 0x0
    SvGardenPlus* pCopy;    // 0x4 a copy of 0x89B00 bytes
    u32 unk_0x8;            // 0x8
};
extern SvSaveMemory g_SvSaveMemory;

// the players of the network game (names are ours): the copies of the players of other towns (save numbers 4..7)
// and the save number of each netgame::PlayerNo
struct SvNetPlayerList
{
    SvPlayerData players;               // 0x00000
    PlayerSaveNumber saveNumbers[4];    // 0x29200
};
extern SvNetPlayerList g_SvNetPlayerList;
// the player of this console (NULL until the save code sets it)
extern SvPlayer* g_pSvCurrentPlayer;

// the save I/O (object 0x00952490, names are ours): whether it is working on these parts of the save file. The
// access functions call them; only the player functions use the result.
bool IsSaveIoOnMainData(); // 0x006086B4 (offset 0, size 0x89A60)
bool IsSaveIoOnWholeFile(); // 0x00607F4C
bool IsSaveIoOnNpcData(); // 0x00607F8C

// the save file and its parts (names are ours)
SvGardenPlus* GetGardenPlus(); // 0x002FB344
SvGardenData* GetGardenData(); // 0x002FAEAC
SvSharedData* GetSharedData(); // 0x002FBA0C
SvCensus* GetCensus(); // 0x002FD00C

SvPlayer* SvGetCurrentPlayer(); // 0x002FBA40 | libgarden [tier A]
PlayerSaveNumber SvGetPlayerSaveNo(netgame::PlayerNo player = netgame::PLAYER_NO_MY_NO); // 0x002FF6CC (name after libgarden's sv.hpp)
// a player of the town (0..3)
bool SvIsValidPlayerSaveNo(PlayerSaveNumber save); // 0x0027B0FC (name after libgarden's sv.hpp)
// a player of another town (4..7)
bool SvIsNetPlayerSaveNo(PlayerSaveNumber save); // 0x0027B0E0 (name is ours)
// any player (0..7)
bool SvIsAnyPlayerSaveNo(PlayerSaveNumber save); // 0x002FDE1C (name is ours)
// the slot of a player of another town (-1 for another save number)
s32 SvGetNetPlayerSlot(PlayerSaveNumber save); // 0x0027AB98 (name is ours)
SvPlayer* SvGetPlayer(PlayerSaveNumber save); // 0x002FBA60 | libgarden [tier A]
SvPlayer* SvGetNetPlayer(netgame::PlayerNo player = netgame::PLAYER_NO_MY_NO); // 0x002FEB2C | libgarden [tier A]
SvBestFriendList* SvGetBestFriendList(); // 0x001DF8B4 | libgarden [tier A]
