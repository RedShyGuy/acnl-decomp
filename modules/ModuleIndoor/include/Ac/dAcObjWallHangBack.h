#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x6687C in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcObjWallHangBack : public ::UtlBase<Actor>
{
public:
    AcObjWallHangBack(); // ctor address unknown
    virtual ~AcObjWallHangBack(); // ModuleIndoor.cro +0x024334 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x024224 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x0240DC slot 0x30
};
