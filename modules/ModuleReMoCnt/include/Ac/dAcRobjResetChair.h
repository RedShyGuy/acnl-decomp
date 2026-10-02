#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x65E8 in ModuleReMoCnt.cro, offset_to_top 0, 23 entries
class AcRobjResetChair : public ::UtlBase<Actor>
{
public:
    AcRobjResetChair(); // ctor address unknown
    virtual ~AcRobjResetChair(); // ModuleReMoCnt.cro +0x002354 slot 0x00
    virtual void Initialize(); // ModuleReMoCnt.cro +0x0058A8 slot 0x0C
    virtual void Finalize(); // ModuleReMoCnt.cro +0x005944 slot 0x18
    virtual void Calc(); // ModuleReMoCnt.cro +0x001F44 slot 0x24
    virtual void Draw(); // ModuleReMoCnt.cro +0x001F2C slot 0x30
};
