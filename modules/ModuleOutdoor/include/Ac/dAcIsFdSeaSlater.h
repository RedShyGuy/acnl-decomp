#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x95344 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x95434 in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x95460 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x95498 in ModuleOutdoor.cro, offset_to_top -588, 11 entries
class AcIsFdSeaSlater : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdSeaSlater>
{
public:
    AcIsFdSeaSlater(); // ctor address unknown
    virtual ~AcIsFdSeaSlater(); // ModuleOutdoor.cro +0x02FC0C slot 0x00
};
