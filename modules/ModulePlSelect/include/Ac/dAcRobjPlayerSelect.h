#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x66C0 in ModulePlSelect.cro, offset_to_top 0, 23 entries
class AcRobjPlayerSelect : public ::UtlBase<Actor>
{
public:
    AcRobjPlayerSelect(); // ctor address unknown
    virtual ~AcRobjPlayerSelect(); // ModulePlSelect.cro +0x004840 slot 0x00
    virtual oml::framework::Result Initialize(); // ModulePlSelect.cro +0x004E14 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModulePlSelect.cro +0x004EB0 slot 0x18
    virtual oml::framework::Result Calc(); // ModulePlSelect.cro +0x004648 slot 0x24
    virtual oml::framework::Result Draw(); // ModulePlSelect.cro +0x004630 slot 0x30
};
