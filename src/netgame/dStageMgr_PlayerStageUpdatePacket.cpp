#include "netgame/dStageMgr.h"

namespace netgame {
// 0x00626348 (name after libgarden)
void netgame::StageMgr::PlayerStageUpdatePacket::Set(PlayerNo player, bool updateStage, stage::Name stage, bool updateControl,
                                                     bool control, PlayerNo receiver)
{
    value = (player & 3) | (updateStage << 2) | (stage << 3) | (updateControl << 11) | (control << 12) | (receiver << 13);
}
} // namespace netgame
