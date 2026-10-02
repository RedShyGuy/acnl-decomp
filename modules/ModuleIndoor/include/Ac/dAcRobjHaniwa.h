#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x65770 in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcRobjHaniwa : public ::UtlBase<Actor>
{
public:
    AcRobjHaniwa(); // ctor address unknown
    virtual ~AcRobjHaniwa(); // ModuleIndoor.cro +0x00D3AC slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x00D13C slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x00D124 slot 0x30
};
