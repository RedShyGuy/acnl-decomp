#include "netgame/dConfirmMachine.h"

#include "netgame/dNetGameMgr.h"

namespace netgame {
// 0x00624948 (name is ours)
void netgame::ConfirmMachine::Update(PlayerNo player, bool isOpen)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == player) {
            if (isOpen) {
                Add(player);
            } else {
                Remove(player);
            }
        } else if (mgr->IsMultiPlayer() && mgr->IsAdmitted(no)) {
            Remove(no);
        } else {
            Add(no);
        }
    }
}

// 0x00624A08 slot 0x00 (name after libgarden)
// 0x00624A04 (deleting dtor)
netgame::ConfirmMachine::~ConfirmMachine()
{
    // empty (in the original too)
}

// 0x0075F814 (name is ours)
bool netgame::ConfirmMachine::IsComplete() const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (!IsSet(static_cast<PlayerNo>(i))) {
            return false;
        }
    }
    return true;
}
} // namespace netgame
