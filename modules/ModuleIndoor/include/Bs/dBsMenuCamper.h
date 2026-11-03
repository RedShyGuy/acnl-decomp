#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x657D4 in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x65840 in ModuleIndoor.cro, offset_to_top -40, 3 entries
class BsMenuCamper : public ::MenuBase, public ::state::Mode<BsMenuCamper>
{
public:
    BsMenuCamper(); // ctor address unknown
    virtual ~BsMenuCamper(); // ModuleIndoor.cro +0x035124 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x00E1F0 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x00E8B8 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x00E648 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x00E1C0 slot 0x30
};
