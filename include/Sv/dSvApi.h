#pragma once

#include "decomp.h"

// the access functions of the save data (game code)
void SvGetBestFriendList(); // 0x001DF8B4 | libgarden [tier A]
void SvGetCurrentPlayer(); // 0x002FBA40 | libgarden [tier A]
void SvGetPlayer(PlayerSaveNumber); // 0x002FBA60 | libgarden [tier A]
void SvGetNetPlayer(netgame::PlayerNo); // 0x002FEB2C | libgarden [tier A]
