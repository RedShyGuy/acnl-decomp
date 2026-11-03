#include "Player/dPlayerApi.h"

// 0x005C13AC | libgarden [tier A]
void PlayerGetPosition(long&, long&, netgame::PlayerNo, bool)
{
}

// 0x005C3238 | libgarden [tier A]
void PlayerGetPositionForCamera()
{
}

// 0x005C3DDC | libgarden [tier A]
void PlayerGetState(netgame::PlayerNo)
{
}

// 0x005C3EA0 | libgarden [tier A]
AcPlayer* PlayerGetActor(netgame::PlayerNo, bool)
{
}

#include "stage/stage_Extern.h"

// the actions of the player (values of libgarden's PlayerAction::Name; names are ours)
// 0x00882B50
extern const u32 PLAYER_ACTION_IDLE = 6;
// 0x00882B60
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK = 47;
// 0x00882B64
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_WALK_SLOW = 48;
// 0x00882B68
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_RUN = 49;
// 0x00882B70
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_UP = 52;
// 0x00882B74
extern const u32 PLAYER_ACTION_EXIT_CONNECTION_STAIR_DOWN = 53;
// 0x00882B78
extern const u32 PLAYER_ACTION_DOOR_EXIT_WAIT = 70;
// 0x00882B7C
extern const u32 PLAYER_ACTION_DOOR_EXIT_BEGIN = 71;

// 0x00AB00E4
PlayerStartInfo g_PlayerStartInfo;
