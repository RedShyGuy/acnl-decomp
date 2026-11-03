#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x141A0 in ModuleClub.cro, offset_to_top 0, 23 entries
class AcRobjLiveChair : public ::UtlBase<Actor>
{
public:
    AcRobjLiveChair(); // ctor address unknown
    virtual ~AcRobjLiveChair(); // ModuleClub.cro +0x008884 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleClub.cro +0x011BB8 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleClub.cro +0x011C54 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleClub.cro +0x0087A4 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleClub.cro +0x00878C slot 0x30
};
