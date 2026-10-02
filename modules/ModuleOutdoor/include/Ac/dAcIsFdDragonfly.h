#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldStraight.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9517C in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x9526C in ModuleOutdoor.cro, offset_to_top -496, 9 entries
// vtable +0x95298 in ModuleOutdoor.cro, offset_to_top -568, 2 entries
// vtable +0x952D0 in ModuleOutdoor.cro, offset_to_top -648, 11 entries
class AcIsFdDragonfly : public ::AcInsectFieldStraight, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdDragonfly>
{
public:
    AcIsFdDragonfly(); // ctor address unknown
    virtual ~AcIsFdDragonfly(); // ModuleOutdoor.cro +0x02EFB8 slot 0x00
};
