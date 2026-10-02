#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Utl/dUtlBase.h"

// vtable +0x66238 in ModuleIndoor.cro, offset_to_top 0, 23 entries
class AcObjUnitCursor : public ::UtlBase<Actor>
{
public:
    AcObjUnitCursor(); // ctor address unknown
    virtual ~AcObjUnitCursor(); // ModuleIndoor.cro +0x01A538 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x05FA60 slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x05FAFC slot 0x18
    virtual void Calc(); // ModuleIndoor.cro +0x01A2C4 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x01A120 slot 0x30
};
