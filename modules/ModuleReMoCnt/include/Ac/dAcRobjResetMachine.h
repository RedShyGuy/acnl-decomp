#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x6924 in ModuleReMoCnt.cro, offset_to_top 0, 23 entries
class AcRobjResetMachine : public ::UtlBase<Actor>
{
public:
    AcRobjResetMachine(); // ctor address unknown
    virtual ~AcRobjResetMachine(); // ModuleReMoCnt.cro +0x005464 slot 0x00
    virtual void Calc(); // ModuleReMoCnt.cro +0x00515C slot 0x24
    virtual void Draw(); // ModuleReMoCnt.cro +0x005130 slot 0x30
};
