#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x27328 in ModuleShop.cro, offset_to_top 0, 23 entries
class AcRobjPermaMachine : public ::UtlBase<Actor>
{
public:
    AcRobjPermaMachine(); // ctor address unknown
    virtual ~AcRobjPermaMachine(); // ModuleShop.cro +0x017C28 slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x017714 slot 0x24
    virtual void Draw(); // ModuleShop.cro +0x0176FC slot 0x30
};
