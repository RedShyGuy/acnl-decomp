#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Other/dObjTalkRecept.h"
#include "Utl/dUtlBase.h"

// vtable +0x97404 in ModuleOutdoor.cro, offset_to_top 0, 40 entries
// vtable +0x974AC in ModuleOutdoor.cro, offset_to_top -104, 72 entries
// vtable +0x975D4 in ModuleOutdoor.cro, offset_to_top -228, 14 entries
class AcHomeTicketMachine : public ::UtlBase<DemoActor>, public ::ObjTalkRecept
{
public:
    AcHomeTicketMachine(); // ctor address unknown
    virtual ~AcHomeTicketMachine(); // ModuleOutdoor.cro +0x05C5B4 slot 0x00
    virtual void Initialize(); // ModuleOutdoor.cro +0x083258 slot 0x0C
    virtual void Finalize(); // ModuleOutdoor.cro +0x0832F4 slot 0x18
    virtual void Calc(); // ModuleOutdoor.cro +0x05C380 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05C34C slot 0x30
    virtual void Unk0(); // ModuleOutdoor.cro +0x07E670 slot 0x3C
};
