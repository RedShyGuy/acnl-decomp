#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9479C in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x9488C in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x948B8 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x948F0 in ModuleOutdoor.cro, offset_to_top -612, 11 entries
class AcIsFdScorpion : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdScorpion>
{
public:
    AcIsFdScorpion(); // ctor address unknown
    virtual ~AcIsFdScorpion(); // ModuleOutdoor.cro +0x0221D8 slot 0x00
};
