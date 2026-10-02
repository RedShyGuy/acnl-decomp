#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldSwim.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x96E64 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x96F50 in ModuleOutdoor.cro, offset_to_top -480, 9 entries
// vtable +0x96F7C in ModuleOutdoor.cro, offset_to_top -552, 2 entries
// vtable +0x96FB4 in ModuleOutdoor.cro, offset_to_top -576, 11 entries
class AcIsFdWaterStrider : public ::AcInsectFieldSwim, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdWaterStrider>
{
public:
    AcIsFdWaterStrider(); // ctor address unknown
    virtual ~AcIsFdWaterStrider(); // ModuleOutdoor.cro +0x05787C slot 0x00
};
