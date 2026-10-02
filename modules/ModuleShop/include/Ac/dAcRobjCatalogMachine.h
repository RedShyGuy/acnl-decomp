#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Other/dObjTalkRecept.h"
#include "Utl/dUtlBase.h"

// vtable +0x27D80 in ModuleShop.cro, offset_to_top 0, 39 entries
// vtable +0x27E24 in ModuleShop.cro, offset_to_top -104, 72 entries
// vtable +0x27F4C in ModuleShop.cro, offset_to_top -228, 14 entries
class AcRobjCatalogMachine : public ::UtlBase<DemoActor>, public ::ObjTalkRecept
{
public:
    AcRobjCatalogMachine(); // ctor address unknown
    virtual ~AcRobjCatalogMachine(); // ModuleShop.cro +0x022268 slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x022044 slot 0x24
    virtual void Draw(); // ModuleShop.cro +0x02203C slot 0x30
};
