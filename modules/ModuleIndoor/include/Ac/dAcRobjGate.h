#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x653C4 in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcRobjGate : public ::UtlBase<Actor>
{
public:
    AcRobjGate(); // ctor address unknown
    virtual ~AcRobjGate(); // ModuleIndoor.cro +0x005FBC slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x005DA0 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x005CE0 slot 0x30
};
