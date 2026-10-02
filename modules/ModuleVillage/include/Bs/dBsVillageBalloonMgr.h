#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x20BAC in ModuleVillage.cro, offset_to_top 0, 22 entries
class BsVillageBalloonMgr : public ::UtlBase<Base>
{
public:
    BsVillageBalloonMgr(); // ctor address unknown
    virtual ~BsVillageBalloonMgr(); // ModuleVillage.cro +0x001AF8 slot 0x00
    virtual void Initialize(); // ModuleVillage.cro +0x01CC74 slot 0x0C
    virtual void Finalize(); // ModuleVillage.cro +0x01CD10 slot 0x18
    virtual void Calc(); // ModuleVillage.cro +0x0019A4 slot 0x24
    virtual void Draw(); // ModuleVillage.cro +0x00199C slot 0x30
};
