#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x267DC in ModuleShop.cro, offset_to_top 0, 23 entries
class AcRobjHighTech : public ::UtlBase<Actor>
{
public:
    AcRobjHighTech(); // ctor address unknown
    virtual ~AcRobjHighTech(); // ModuleShop.cro +0x004650 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleShop.cro +0x024230 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleShop.cro +0x0242CC slot 0x18
    virtual oml::framework::Result Calc(); // ModuleShop.cro +0x004510 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleShop.cro +0x0044F8 slot 0x30
};
