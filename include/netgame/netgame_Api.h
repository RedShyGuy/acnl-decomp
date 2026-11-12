#pragma once

#include "decomp.h"
#include "netgame/dGateMgr.h"

namespace netgame {
// a player of the network game (names after libgarden)
enum PlayerNo : u8
{
    PLAYER_NO_HOST = 0,
    PLAYER_NO_0 = 0,
    PLAYER_NO_1 = 1,
    PLAYER_NO_2 = 2,
    PLAYER_NO_3 = 3,
    PLAYER_NO_INVALID = 4,
    PLAYER_NO_MY_NO = 4,
    PLAYER_NO_TARGET_ALL = 4,
    PLAYER_NO_MAX = 4,
};

// the kind of the network game (names after libgarden)
enum Type : u8
{
    TYPE_NONE = 0,
    TYPE_FRIEND_MATCH = 1,
    TYPE_RANDOM_MATCH = 2,
    TYPE_DREAM = 3,
};

bool IsMultiplePlayer(); // 0x002FC990 | libgarden
DECOMP_NOINLINE bool IsMultiPlayer(); // 0x00300838 | libgarden
DECOMP_NOINLINE Type GetNetgameType(); // 0x00305ED8 | libgarden
DECOMP_NOINLINE bool IsHost(); // 0x0030B824 | libgarden
bool IsMyPlayerHost(); // 0x00625544
// the save number of the player of this console in the network game (name is ours)
u8 GetSaveNo(); // 0x0062644C
DECOMP_NOINLINE PlayerNo GetMyPlayerNo(); // 0x00305EF0 (name after libgarden)
bool InitNet(); // 0x00617BE8 | libgarden
bool IsStageControl(); // 0x0061B278 | libgarden
bool RequestOpenGateRandomMatch(GateMgr::Flag flag); // 0x0061B2E8 | libgarden
bool StageTransitionTourResult(); // 0x0061F204 | libgarden
bool RequestCloseGateRandomMatch(GateMgr::Flag flag); // 0x00620840 | libgarden
void EnqueuePacket(u8 type, PlayerNo target, const void* data = NULL, size_t size = 0); // 0x00625488 | libgarden
u8 GetMask(PlayerNo player); // 0x00625750 | libgarden
bool ExistsPlayer(PlayerNo player); // 0x006259D8 | libgarden
void SetNetGameType(Type type); // 0x00625B88 | libgarden
DECOMP_NOINLINE u8 GetPlayerCount(); // 0x00626168 | libgarden
bool IsConnectedBestFriend(); // 0x00626218 | libgarden
bool StopNet(); // 0x00627368 | libgarden
// the Mgr8 of NetGameMgr is done / the join is not finished (names are ours; netgame_Api.cpp)
bool IsMgr8Done();
bool IsJoinNotFinished();
PlayerNo GetLeavingPlayer();
bool IsLeavingPlayerGone();
bool ConfirmLeaving();
bool IsPastLeavingPoint();
bool IsJoinFlag15Set();
bool IsSaveConfirmed();
void ResetSaveDemo();
bool IsMySaveConfirmed();
bool StartLeaveExchange();
bool IsSaveDemoDone();
bool AreAllPlayersInSave();
void ConfirmSave();
void SendSessionOpen();
bool IsJoining();
void OnPlayerEntered();
bool IsSaveDemoWaiting();
bool StartConnectBestFriend(); // 0x0062739C | libgarden
bool StopConnectBestFriend(); // 0x00627408 (name is ours)
void* GetIslandMap(); // 0x006A7988 | libgarden
} // namespace netgame
