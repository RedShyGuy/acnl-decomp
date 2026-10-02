#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFlyPursue.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSkeletal.h"

// vtable +0x945D8 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x946C4 in ModuleOutdoor.cro, offset_to_top -484, 9 entries
// vtable +0x946F0 in ModuleOutdoor.cro, offset_to_top -532, 2 entries
// vtable +0x94728 in ModuleOutdoor.cro, offset_to_top -592, 11 entries
class AcIsFdMosquito : public ::AcInsectFieldFlyPursue, public ::ResourceGetSkeletal, public ::ObjectState<AcIsFdMosquito>
{
public:
    AcIsFdMosquito(); // ctor address unknown
    virtual ~AcIsFdMosquito(); // ModuleOutdoor.cro +0x02077C slot 0x00
};
