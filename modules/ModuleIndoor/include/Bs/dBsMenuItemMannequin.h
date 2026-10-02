#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// vtable +0x66CC8 in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x66D34 in ModuleIndoor.cro, offset_to_top -40, 63 entries
// vtable +0x66E38 in ModuleIndoor.cro, offset_to_top -164, 3 entries
class BsMenuItemMannequin : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuItemMannequin>
{
public:
    BsMenuItemMannequin(); // ctor address unknown
    virtual ~BsMenuItemMannequin(); // ModuleIndoor.cro +0x030510 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x0399E4 slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x039E28 slot 0x18
    virtual void Calc(); // ModuleIndoor.cro +0x039C7C slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x0399A8 slot 0x30
    virtual void OnClose(); // ModuleIndoor.cro +0x039220 slot 0x44
};
