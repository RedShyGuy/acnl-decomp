#pragma once

#include "decomp.h"

namespace netgame {
class StageMgr
{
public:
    class PlayerStageUpdatePacket;
    class PlayerStages;
    void UpdateStageControl(netgame::PlayerNo, bool); // 0x0062287C | libgarden [tier A]
    void SetStage(netgame::PlayerNo, stage::Name); // 0x006229E4 | libgarden [tier A]
    void GetTransitioningPlayer() const; // 0x0075F318 | libgarden [tier A]
    void FUN_0075f464() const; // 0x0075F464 | libgarden [tier A]
};
} // namespace netgame
