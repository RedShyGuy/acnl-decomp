#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x92DF8 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x92EE4 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x92F10 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x92F48 in ModuleOutdoor.cro, offset_to_top -560, 11 entries
class AcIsFdCicada : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdCicada>
{
public:
    AcIsFdCicada(); // ctor address unknown
    virtual ~AcIsFdCicada(); // ModuleOutdoor.cro +0x00E9B8 slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x00E698 slot 0x30
};
