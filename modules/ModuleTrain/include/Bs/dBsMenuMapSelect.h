#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// vtable +0xF3E0 in ModuleTrain.cro, offset_to_top 0, 16 entries
// vtable +0xF428 in ModuleTrain.cro, offset_to_top -20, 3 entries
class BsMenuMapSelect : public ::Base, public ::state::Mode<BsMenuMapSelect>
{
public:
    BsMenuMapSelect(); // ctor address unknown
    virtual ~BsMenuMapSelect(); // ModuleTrain.cro +0x009308 slot 0x00
    virtual void Initialize(); // ModuleTrain.cro +0x008D94 slot 0x0C
    virtual void Finalize(); // ModuleTrain.cro +0x00918C slot 0x18
    virtual void Calc(); // ModuleTrain.cro +0x0090C0 slot 0x24
    virtual void Draw(); // ModuleTrain.cro +0x008D5C slot 0x30
};
