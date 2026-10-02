#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x6735C in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcObjWallHangUnitCursor : public ::UtlBase<Actor>
{
public:
    AcObjWallHangUnitCursor(); // ctor address unknown
    virtual ~AcObjWallHangUnitCursor(); // ModuleIndoor.cro +0x03F0C0 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x03EF48 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x03EEC8 slot 0x30
};
