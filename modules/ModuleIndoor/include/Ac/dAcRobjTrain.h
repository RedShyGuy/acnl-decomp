#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x656B4 in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcRobjTrain : public ::UtlBase<Actor>
{
public:
    AcRobjTrain(); // ctor address unknown
    virtual ~AcRobjTrain(); // ModuleIndoor.cro +0x00AA64 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x00A17C slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x00A158 slot 0x30
};
