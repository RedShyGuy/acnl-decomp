#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x92FB4 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x930A0 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x930CC in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x93104 in ModuleOutdoor.cro, offset_to_top -592, 11 entries
class AcIsFdFlower : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdFlower>
{
public:
    AcIsFdFlower(); // ctor address unknown
    virtual ~AcIsFdFlower(); // ModuleOutdoor.cro +0x00F9E4 slot 0x00
};
