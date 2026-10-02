#pragma once

#include "decomp.h"
#include "netgame/dStageMgr.h"

class netgame::StageMgr::PlayerStageUpdatePacket
{
public:
    void Set(netgame::PlayerNo, bool, stage::Name, bool, bool, netgame::PlayerNo); // 0x00626348 | libgarden [tier A]
};
