#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x14204 in ModuleClub.cro, offset_to_top 0, 23 entries
class AcRobjProjector : public ::UtlBase<Actor>
{
public:
    AcRobjProjector(); // ctor address unknown
    virtual ~AcRobjProjector(); // ModuleClub.cro +0x00CD4C slot 0x00
    virtual void Calc(); // ModuleClub.cro +0x00C9DC slot 0x24
    virtual void Draw(); // ModuleClub.cro +0x00C850 slot 0x30
};
