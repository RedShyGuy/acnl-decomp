#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x2018C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x2024C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20280 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x202B8 in ModuleMusIns.cro, offset_to_top -776, 11 entries
class AcIsMuHornet : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuHornet>
{
public:
    AcIsMuHornet(); // ctor address unknown
    virtual ~AcIsMuHornet(); // ModuleMusIns.cro +0x005088 slot 0x00
};
