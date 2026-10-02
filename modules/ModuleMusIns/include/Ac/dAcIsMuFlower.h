#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1FFEC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x200AC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x200E0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x20118 in ModuleMusIns.cro, offset_to_top -748, 11 entries
class AcIsMuFlower : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuFlower>
{
public:
    AcIsMuFlower(); // ctor address unknown
    virtual ~AcIsMuFlower(); // ModuleMusIns.cro +0x0046F4 slot 0x00
};
