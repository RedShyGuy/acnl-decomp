#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x91EE4 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x91FD0 in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x91FFC in ModuleOutdoor.cro, offset_to_top -540, 2 entries
// vtable +0x92034 in ModuleOutdoor.cro, offset_to_top -616, 11 entries
class AcIsFdMoth : public ::AcInsectFieldFly, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdMoth>
{
public:
    AcIsFdMoth(); // ctor address unknown
    virtual ~AcIsFdMoth(); // ModuleOutdoor.cro +0x0055E8 slot 0x00
};
