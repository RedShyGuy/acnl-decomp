#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceNone.h"

// vtable +0x93EB0 in ModuleOutdoor.cro, offset_to_top 0, 58 entries
// vtable +0x93FA4 in ModuleOutdoor.cro, offset_to_top -468, 7 entries
// vtable +0x93FC8 in ModuleOutdoor.cro, offset_to_top -516, 2 entries
// vtable +0x94000 in ModuleOutdoor.cro, offset_to_top -580, 11 entries
class AcIsFdFirefly : public ::AcInsectFieldFly, public ::ResourceNone, public ::ObjectState<AcIsFdFirefly>
{
public:
    AcIsFdFirefly(); // ctor address unknown
    virtual ~AcIsFdFirefly(); // ModuleOutdoor.cro +0x01A100 slot 0x00
};
