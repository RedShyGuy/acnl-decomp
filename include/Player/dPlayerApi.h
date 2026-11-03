#pragma once

#include "decomp.h"

// the save slot of a player (names after libgarden)
enum PlayerSaveNumber : u8
{
    PLAYER_SAVE_NUMBER_VILLAGE_PLAYER_0 = 0,
    PLAYER_SAVE_NUMBER_VILLAGE_PLAYER_1 = 1,
    PLAYER_SAVE_NUMBER_VILLAGE_PLAYER_2 = 2,
    PLAYER_SAVE_NUMBER_VILLAGE_PLAYER_3 = 3,
    PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_0 = 4,
    PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_1 = 5,
    PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_2 = 6,
    PLAYER_SAVE_NUMBER_NET_PLAYER_SLOT_3 = 7,
    PLAYER_SAVE_NUMBER_INVALID = 8,
};

// the access functions of the players (game code)
void PlayerGetPosition(long&, long&, netgame::PlayerNo, bool); // 0x005C13AC | libgarden [tier A]
void PlayerGetPositionForCamera(); // 0x005C3238 | libgarden [tier A]
void PlayerGetState(netgame::PlayerNo); // 0x005C3DDC | libgarden [tier A]
AcPlayer* PlayerGetActor(netgame::PlayerNo, bool); // 0x005C3EA0 | libgarden [tier A]
