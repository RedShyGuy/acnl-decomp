#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21364 in ModuleMusIns.cro, offset_to_top 0, 46 entries
// vtable +0x21428 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x2145C in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21494 in ModuleMusIns.cro, offset_to_top -748, 11 entries
class AcIsMuSeaSlater : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuSeaSlater>
{
public:
    AcIsMuSeaSlater(); // ctor address unknown
    virtual ~AcIsMuSeaSlater(); // ModuleMusIns.cro +0x00CA48 slot 0x00
};
