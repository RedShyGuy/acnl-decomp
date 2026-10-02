#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVisMat.h"

// vtable +0x98464 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x98554 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x98580 in ModuleOutdoor.cro, offset_to_top -568, 2 entries
// vtable +0x985B8 in ModuleOutdoor.cro, offset_to_top -592, 11 entries
class AcIsFdAnt : public ::AcInsectFieldBase, public ::ResourceGetSklVisMat, public ::ObjectState<AcIsFdAnt>
{
public:
    AcIsFdAnt(); // ctor address unknown
    virtual ~AcIsFdAnt(); // ModuleOutdoor.cro +0x079F50 slot 0x00
};
