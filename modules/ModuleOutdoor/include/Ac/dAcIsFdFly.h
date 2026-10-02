#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSkeletal.h"

// vtable +0x9862C in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x98718 in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x98744 in ModuleOutdoor.cro, offset_to_top -516, 2 entries
// vtable +0x9877C in ModuleOutdoor.cro, offset_to_top -600, 11 entries
class AcIsFdFly : public ::AcInsectFieldFly, public ::ResourceGetSkeletal, public ::ObjectState<AcIsFdFly>
{
public:
    AcIsFdFly(); // ctor address unknown
    virtual ~AcIsFdFly(); // ModuleOutdoor.cro +0x07AF78 slot 0x00
};
