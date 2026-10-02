#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x94FB8 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x950A4 in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x950D0 in ModuleOutdoor.cro, offset_to_top -540, 2 entries
// vtable +0x95108 in ModuleOutdoor.cro, offset_to_top -632, 11 entries
class AcIsFdButterfly : public ::AcInsectFieldFly, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdButterfly>
{
public:
    AcIsFdButterfly(); // ctor address unknown
    virtual ~AcIsFdButterfly(); // ModuleOutdoor.cro +0x02DC7C slot 0x00
};
