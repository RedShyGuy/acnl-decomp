#pragma once

#include "decomp.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/netgame_Api.h"

namespace netgame {
// The players that still have to confirm a step (name after libgarden, the others are ours)
// RTTI N7netgame14ConfirmMachineE @ 0x008D3F50
// vtable 0x0090BD3C (vptr 0x0090BD44), offset_to_top 0, 2 entries
class ConfirmMachine : public ::netgame::MachineBitTable
{
public:
    ConfirmMachine() {}
    virtual ~ConfirmMachine(); // 0x00624A08 slot 0x00
    // the player answered: all the others that take part are still open
    void Update(PlayerNo player, bool isOpen); // 0x00624948
    bool IsComplete() const; // 0x0075F814
};
ASSERT_SIZE(ConfirmMachine, 8);
} // namespace netgame
