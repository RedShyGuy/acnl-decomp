#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x96600 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x966EC in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x96718 in ModuleOutdoor.cro, offset_to_top -540, 2 entries
// vtable +0x96750 in ModuleOutdoor.cro, offset_to_top -580, 11 entries
class AcIsFdSnowCrystal : public ::AcInsectFieldFly, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdSnowCrystal>
{
public:
    AcIsFdSnowCrystal(); // ctor address unknown
    virtual ~AcIsFdSnowCrystal(); // ModuleOutdoor.cro +0x050AE4 slot 0x00
};
