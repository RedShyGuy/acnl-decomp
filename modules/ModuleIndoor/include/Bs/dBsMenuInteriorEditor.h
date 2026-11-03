#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x671DC in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x67248 in ModuleIndoor.cro, offset_to_top -40, 3 entries
class BsMenuInteriorEditor : public ::MenuBase, public ::state::Mode<BsMenuInteriorEditor>
{
public:
    BsMenuInteriorEditor(); // ctor address unknown
    virtual ~BsMenuInteriorEditor(); // ModuleIndoor.cro +0x02F624 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x03DED4 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x03E1B0 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x03E120 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x03DE78 slot 0x30
};
