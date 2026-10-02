#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x221EC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x222AC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x222E0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x22318 in ModuleMusIns.cro, offset_to_top -748, 11 entries
class AcIsMuWaterStrider : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuWaterStrider>
{
public:
    AcIsMuWaterStrider(); // ctor address unknown
    virtual ~AcIsMuWaterStrider(); // ModuleMusIns.cro +0x010A94 slot 0x00
};
