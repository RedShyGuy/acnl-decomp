#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldStraight.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x93348 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x93438 in ModuleOutdoor.cro, offset_to_top -496, 9 entries
// vtable +0x93464 in ModuleOutdoor.cro, offset_to_top -568, 2 entries
// vtable +0x9349C in ModuleOutdoor.cro, offset_to_top -628, 11 entries
class AcIsFdLocust : public ::AcInsectFieldStraight, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdLocust>
{
public:
    AcIsFdLocust(); // ctor address unknown
    virtual ~AcIsFdLocust(); // ModuleOutdoor.cro +0x0115E8 slot 0x00
};
