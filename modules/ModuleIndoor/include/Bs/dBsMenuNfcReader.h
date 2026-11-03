#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x66504 in ModuleIndoor.cro, offset_to_top 0, 25 entries
// vtable +0x66570 in ModuleIndoor.cro, offset_to_top -40, 3 entries
class BsMenuNfcReader : public ::MenuBase, public ::state::Mode<BsMenuNfcReader>
{
public:
    BsMenuNfcReader(); // ctor address unknown
    virtual ~BsMenuNfcReader(); // ModuleIndoor.cro +0x0206B4 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x0204D8 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x02061C slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x020580 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x0204C4 slot 0x30
};
