#pragma once

#include "decomp.h"
#include "Object/dObjectState.h"
#include "Other/dActor.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x65438 in ModuleIndoor.cro, offset_to_top 0, 20 entries
// vtable +0x65494 in ModuleIndoor.cro, offset_to_top -72, 9 entries
// vtable +0x654C0 in ModuleIndoor.cro, offset_to_top -144, 2 entries
// vtable +0x654F8 in ModuleIndoor.cro, offset_to_top -308, 11 entries
class AcCockroach : public ::Actor, public ::ResourceGetSklVis, public ::ObjectState<AcCockroach>
{
public:
    AcCockroach(); // ctor address unknown
    virtual ~AcCockroach(); // ModuleIndoor.cro +0x035DAC slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x006FD0 slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x007268 slot 0x18
    virtual void CanCalc() const; // ModuleIndoor.cro +0x00603C slot 0x20
    virtual void Calc(); // ModuleIndoor.cro +0x007150 slot 0x24
    virtual void HandleCalcResult(oml::framework::Result); // ModuleIndoor.cro +0x00623C slot 0x28
    virtual void Draw(); // ModuleIndoor.cro +0x006FA8 slot 0x30
    virtual void Unk0(); // ModuleIndoor.cro +0x05AA5C slot 0x3C
};
