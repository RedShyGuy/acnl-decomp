#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldSwim.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9698C in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x96A78 in ModuleOutdoor.cro, offset_to_top -480, 9 entries
// vtable +0x96AA4 in ModuleOutdoor.cro, offset_to_top -552, 2 entries
// vtable +0x96ADC in ModuleOutdoor.cro, offset_to_top -588, 11 entries
class AcIsFdWaterBeetle : public ::AcInsectFieldSwim, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdWaterBeetle>
{
public:
    AcIsFdWaterBeetle(); // ctor address unknown
    virtual ~AcIsFdWaterBeetle(); // ModuleOutdoor.cro +0x052998 slot 0x00
};
