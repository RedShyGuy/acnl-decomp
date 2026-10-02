#pragma once

#include "decomp.h"
#include "netgame/dStageMgr.h"

class netgame::StageMgr::PlayerStages
{
public:
    void GetStage(netgame::PlayerNo) const; // 0x0075F84C | libgarden [tier A]
    void GetPlayerCount(stage::Name) const; // 0x0075F874 | libgarden [tier A]
};
