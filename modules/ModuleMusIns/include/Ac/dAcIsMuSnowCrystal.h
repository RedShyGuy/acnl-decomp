#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21B8C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x21C4C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21C80 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21CB8 in ModuleMusIns.cro, offset_to_top -720, 11 entries
class AcIsMuSnowCrystal : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuSnowCrystal>
{
public:
    AcIsMuSnowCrystal(); // ctor address unknown
    virtual ~AcIsMuSnowCrystal(); // ModuleMusIns.cro +0x00ED04 slot 0x00
};
