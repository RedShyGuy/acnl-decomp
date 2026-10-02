#pragma once

#include "decomp.h"

namespace netgame {
void IsMultiplePlayer(); // 0x002FC990 | libgarden [tier A]
void IsMultiPlayer(); // 0x00300838 | libgarden [tier A]
void GetNetgameType(); // 0x00305ED8 | libgarden [tier A]
void IsHost(); // 0x0030B824 | libgarden [tier A]
void InitNet(); // 0x00617BE8 | libgarden [tier A]
void IsStageControl(); // 0x0061B278 | libgarden [tier A]
void RequestOpenGateRandomMatch(netgame::GateMgr::Flag); // 0x0061B2E8 | libgarden [tier A]
void StageTransitionTourResult(); // 0x0061F204 | libgarden [tier A]
void RequestCloseGateRandomMatch(netgame::GateMgr::Flag); // 0x00620840 | libgarden [tier A]
void EnqueuePacket(unsigned char, unsigned char, void const*, unsigned int); // 0x00625488 | libgarden [tier A]
void GetMask(netgame::PlayerNo); // 0x00625750 | libgarden [tier A]
void ExistsPlayer(netgame::PlayerNo); // 0x006259D8 | libgarden [tier A]
void SetNetGameType(netgame::Type); // 0x00625B88 | libgarden [tier A]
void GetAvatar(netgame::PlayerNo); // 0x00626150 | libgarden [tier A]
void GetPlayerCount(); // 0x00626168 | libgarden [tier A]
void IsConnectedBestFriend(); // 0x00626218 | libgarden [tier A]
void StopNet(); // 0x00627368 | libgarden [tier A]
void StartConnectBestFriend(); // 0x0062739C | libgarden [tier A]
void GetIslandMap(); // 0x006A7988 | libgarden [tier A]
} // namespace netgame
