#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x652C in ModuleReMoCnt.cro, offset_to_top 0, 23 entries
class AcRobjResetTV : public ::UtlBase<Actor>
{
public:
    AcRobjResetTV(); // ctor address unknown
    virtual ~AcRobjResetTV(); // ModuleReMoCnt.cro +0x0011FC slot 0x00
    virtual void Calc(); // ModuleReMoCnt.cro +0x000ED0 slot 0x24
    virtual void Draw(); // ModuleReMoCnt.cro +0x000EB8 slot 0x30
};
