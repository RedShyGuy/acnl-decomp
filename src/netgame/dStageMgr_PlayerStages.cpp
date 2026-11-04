#include "netgame/dStageMgr.h"

#include "netgame/dNetGameMgr.h"

namespace netgame {
// 0x0062542C (name is ours)
void netgame::StageMgr::PlayerStages::SetStage(PlayerNo player, stage::Name stage)
{
    if (player < PLAYER_NO_MAX && stage <= stage::NAME_NONE) {
        stages[player] = stage;
    }
}

// 0x00625440 (name is ours)
void netgame::StageMgr::PlayerStages::SetControl(PlayerNo player, bool control)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    if (control) {
        ctrl |= 1 << player;
    } else {
        ctrl &= ~(1 << player);
    }
}

// 0x00625468 (name is ours)
netgame::StageMgr::PlayerStages::PlayerStages()
{
    ctrl = 0;
}

// 0x0075F84C (name after libgarden)
stage::Name netgame::StageMgr::PlayerStages::GetStage(PlayerNo player) const
{
    return player < PLAYER_NO_MAX ? stages[player] : stage::NAME_NONE;
}

// 0x0075F85C (name is ours)
bool netgame::StageMgr::PlayerStages::HasControl(PlayerNo player) const
{
    return player < PLAYER_NO_MAX ? (ctrl >> player) & 1 : false;
}

// 0x0075F874 (name after libgarden)
u8 netgame::StageMgr::PlayerStages::GetPlayerCount(stage::Name stage) const
{
    u8 count = 0;
    if (stage < stage::NAME_MAX) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (NetGameMgr::s_pInstance->IsAdmitted(no) && GetStage(no) == stage) {
                count++;
            }
        }
    }
    return count;
}

// 0x0075F8DC (name is ours)
netgame::PlayerNo netgame::StageMgr::PlayerStages::FindController(stage::Name stage) const
{
    if (stage < stage::NAME_MAX) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (GetStage(no) == stage && HasControl(no)) {
                return no;
            }
        }
    }
    return PLAYER_NO_INVALID;
}

// 0x0075F938 (name is ours)
netgame::PlayerNo netgame::StageMgr::PlayerStages::FindPlayerWithoutControl(stage::Name stage) const
{
    if (stage < stage::NAME_MAX) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (GetStage(no) == stage && !HasControl(no)) {
                return no;
            }
        }
    }
    return PLAYER_NO_INVALID;
}
} // namespace netgame
