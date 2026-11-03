#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x13F14 in ModuleClub.cro, offset_to_top 0, 23 entries
class AcRobjSpeaker : public ::UtlBase<Actor>
{
public:
    AcRobjSpeaker(); // ctor address unknown
    virtual ~AcRobjSpeaker(); // ModuleClub.cro +0x0050E0 slot 0x00
    virtual oml::framework::Result Calc(); // ModuleClub.cro +0x004F1C slot 0x24
    virtual oml::framework::Result Draw(); // ModuleClub.cro +0x004F04 slot 0x30
};
