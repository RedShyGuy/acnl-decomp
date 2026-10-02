#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21ED0 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x21F90 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21FC4 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21FFC in ModuleMusIns.cro, offset_to_top -756, 11 entries
class AcIsMuWaterBeetle : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuWaterBeetle>
{
public:
    AcIsMuWaterBeetle(); // ctor address unknown
    virtual ~AcIsMuWaterBeetle(); // ModuleMusIns.cro +0x00FA58 slot 0x00
};
