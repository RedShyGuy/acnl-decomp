#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// vtable +0x10778 in ModuleTour.cro, offset_to_top 0, 16 entries
// vtable +0x107C0 in ModuleTour.cro, offset_to_top -20, 3 entries
class BsMenuMapIsland : public ::Base, public ::state::Mode<BsMenuMapIsland>
{
public:
    BsMenuMapIsland(); // ctor address unknown
    virtual ~BsMenuMapIsland(); // ModuleTour.cro +0x005250 slot 0x00
    virtual void Initialize(); // ModuleTour.cro +0x004B88 slot 0x0C
    virtual void Finalize(); // ModuleTour.cro +0x005120 slot 0x18
    virtual void Calc(); // ModuleTour.cro +0x005028 slot 0x24
    virtual void Draw(); // ModuleTour.cro +0x004B4C slot 0x30
    virtual void Unk0(); // ModuleTour.cro +0x00D1DC slot 0x3C
};
