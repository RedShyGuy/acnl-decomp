#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x2101C in ModuleVillage.cro, offset_to_top 0, 23 entries
class AcBalloon : public ::UtlBase<Actor>
{
public:
    AcBalloon(); // ctor address unknown
    virtual ~AcBalloon(); // ModuleVillage.cro +0x01B9CC slot 0x00
    virtual void Initialize(); // ModuleVillage.cro +0x01CE08 slot 0x0C
    virtual void Finalize(); // ModuleVillage.cro +0x01CEA4 slot 0x18
    virtual void Calc(); // ModuleVillage.cro +0x01B494 slot 0x24
    virtual void Draw(); // ModuleVillage.cro +0x01AFF4 slot 0x30
    virtual void Unk0(); // ModuleVillage.cro +0x01BBD4 slot 0x3C
};
