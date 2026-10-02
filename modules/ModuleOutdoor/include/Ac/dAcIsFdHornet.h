#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFlyPursue.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVisMat.h"

// vtable +0x93170 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x93260 in ModuleOutdoor.cro, offset_to_top -484, 9 entries
// vtable +0x9328C in ModuleOutdoor.cro, offset_to_top -588, 2 entries
// vtable +0x932C4 in ModuleOutdoor.cro, offset_to_top -668, 11 entries
class AcIsFdHornet : public ::AcInsectFieldFlyPursue, public ::ResourceGetSklVisMat, public ::ObjectState<AcIsFdHornet>
{
public:
    AcIsFdHornet(); // ctor address unknown
    virtual ~AcIsFdHornet(); // ModuleOutdoor.cro +0x01052C slot 0x00
};
