#pragma once

#include "decomp.h"
#include "Object/dObjectState.h"
#include "Other/dActor.h"
#include "Resource/dResourceGetSkeletal.h"

// vtable +0x10510 in ModuleTour.cro, offset_to_top 0, 22 entries
// vtable +0x10574 in ModuleTour.cro, offset_to_top -72, 9 entries
// vtable +0x105A0 in ModuleTour.cro, offset_to_top -120, 2 entries
// vtable +0x105D8 in ModuleTour.cro, offset_to_top -820, 11 entries
class AcPikoHanObj : public ::Actor, public ::ResourceGetSkeletal, public ::ObjectState<AcPikoHanObj>
{
public:
    AcPikoHanObj(); // ctor address unknown
    virtual ~AcPikoHanObj(); // ModuleTour.cro +0x007250 slot 0x00
    virtual void Initialize(); // ModuleTour.cro +0x00205C slot 0x0C
    virtual void Finalize(); // ModuleTour.cro +0x002480 slot 0x18
    virtual void CanCalc() const; // ModuleTour.cro +0x001368 slot 0x20
    virtual void Calc(); // ModuleTour.cro +0x002224 slot 0x24
    virtual void HandleCalcResult(oml::framework::Result); // ModuleTour.cro +0x001428 slot 0x28
    virtual void Draw(); // ModuleTour.cro +0x002008 slot 0x30
};
