#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x13F78 in ModuleClub.cro, offset_to_top 0, 23 entries
class AcRobjPinLight : public ::UtlBase<Actor>
{
public:
    AcRobjPinLight(); // ctor address unknown
    virtual ~AcRobjPinLight(); // ModuleClub.cro +0x005584 slot 0x00
    virtual oml::framework::Result Calc(); // ModuleClub.cro +0x005464 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleClub.cro +0x00544C slot 0x30
};
