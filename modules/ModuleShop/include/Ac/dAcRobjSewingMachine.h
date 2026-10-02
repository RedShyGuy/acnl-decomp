#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x27524 in ModuleShop.cro, offset_to_top 0, 23 entries
class AcRobjSewingMachine : public ::UtlBase<Actor>
{
public:
    AcRobjSewingMachine(); // ctor address unknown
    virtual ~AcRobjSewingMachine(); // ModuleShop.cro +0x01BB90 slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x01B824 slot 0x24
    virtual void Draw(); // ModuleShop.cro +0x01B7FC slot 0x30
};
