#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Other/dObjTalkRecept.h"
#include "Utl/dUtlBase.h"

// vtable +0x6629C in ModuleIndoor.cro, offset_to_top 0, 38 entries
// vtable +0x6633C in ModuleIndoor.cro, offset_to_top -104, 72 entries
// vtable +0x66464 in ModuleIndoor.cro, offset_to_top -228, 14 entries
class AcRobjCoolerBox : public ::UtlBase<DemoActor>, public ::ObjTalkRecept
{
public:
    AcRobjCoolerBox(); // ctor address unknown
    virtual ~AcRobjCoolerBox(); // ModuleIndoor.cro +0x033B98 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x01ADF8 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x01ADE0 slot 0x30
};
