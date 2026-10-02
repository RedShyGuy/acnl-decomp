#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x13794 in ModuleDream.cro, offset_to_top 0, 25 entries
// vtable +0x13800 in ModuleDream.cro, offset_to_top -40, 3 entries
class BsMenuAreaSelect : public ::MenuBase, public ::state::Mode<BsMenuAreaSelect>
{
public:
    BsMenuAreaSelect(); // ctor address unknown
    virtual ~BsMenuAreaSelect(); // ModuleDream.cro +0x004C00 slot 0x00
    virtual void Initialize(); // ModuleDream.cro +0x0082AC slot 0x0C
    virtual void Finalize(); // ModuleDream.cro +0x008710 slot 0x18
    virtual void Calc(); // ModuleDream.cro +0x0085B8 slot 0x24
    virtual void Draw(); // ModuleDream.cro +0x008270 slot 0x30
};
