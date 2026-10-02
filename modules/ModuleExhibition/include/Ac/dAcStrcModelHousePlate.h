#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Utl/dUtlBase.h"
#include "script/dITalkRecept.h"

// vtable +0x8850 in ModuleExhibition.cro, offset_to_top 0, 43 entries
// vtable +0x8904 in ModuleExhibition.cro, offset_to_top -104, 63 entries
class AcStrcModelHousePlate : public ::UtlBase<DemoActor>, public ::script::ITalkRecept
{
public:
    AcStrcModelHousePlate(); // ctor address unknown
    virtual ~AcStrcModelHousePlate(); // ModuleExhibition.cro +0x0066F0 slot 0x00
    virtual void CanInitialize() const; // ModuleExhibition.cro +0x00661C slot 0x08
    virtual void Calc(); // ModuleExhibition.cro +0x0062CC slot 0x24
    virtual void Draw(); // ModuleExhibition.cro +0x006230 slot 0x30
};
