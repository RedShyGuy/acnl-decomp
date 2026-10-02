#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVisMat.h"

// vtable +0x93CE4 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x93DD0 in ModuleOutdoor.cro, offset_to_top -468, 9 entries
// vtable +0x93DFC in ModuleOutdoor.cro, offset_to_top -572, 2 entries
// vtable +0x93E34 in ModuleOutdoor.cro, offset_to_top -616, 11 entries
class AcIsFdFeather : public ::AcInsectFieldFly, public ::ResourceGetSklVisMat, public ::ObjectState<AcIsFdFeather>
{
public:
    AcIsFdFeather(); // ctor address unknown
    virtual ~AcIsFdFeather(); // ModuleOutdoor.cro +0x0193FC slot 0x00
};
