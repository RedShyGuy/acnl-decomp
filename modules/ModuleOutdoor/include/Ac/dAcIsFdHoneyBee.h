#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x94414 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x94500 in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x9452C in ModuleOutdoor.cro, offset_to_top -540, 2 entries
// vtable +0x94564 in ModuleOutdoor.cro, offset_to_top -624, 11 entries
class AcIsFdHoneyBee : public ::AcInsectFieldFly, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdHoneyBee>
{
public:
    AcIsFdHoneyBee(); // ctor address unknown
    virtual ~AcIsFdHoneyBee(); // ModuleOutdoor.cro +0x01FD64 slot 0x00
};
