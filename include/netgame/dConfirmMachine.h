#pragma once

#include "decomp.h"
#include "netgame/dMachineBitTable.h"

namespace netgame {
// RTTI N7netgame14ConfirmMachineE @ 0x008D3F50
// vtable 0x0090BD3C (vptr 0x0090BD44), offset_to_top 0, 2 entries
class ConfirmMachine : public ::netgame::MachineBitTable
{
public:
    ConfirmMachine(); // ctor candidate(s) 0x0061FF98, 0x00622BD0, 0x0062468C, 0x006253F8 (unverified)
    virtual void vf_0x00(); // 0x00624A08 slot 0x00 | virtual slot, introduced by netgame::ConfirmMachine
    virtual void vf_0x04(); // 0x00624A04 slot 0x04 | virtual slot, introduced by netgame::ConfirmMachine
};
} // namespace netgame
