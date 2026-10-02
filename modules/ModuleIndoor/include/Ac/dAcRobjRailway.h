#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x65D40 in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcRobjRailway : public ::UtlBase<Actor>
{
public:
    AcRobjRailway(); // ctor address unknown
    virtual ~AcRobjRailway(); // ModuleIndoor.cro +0x0138E8 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x0137C4 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x0137AC slot 0x30
};
