#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21D2C in ModuleMusIns.cro, offset_to_top 0, 46 entries
// vtable +0x21DF0 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21E24 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21E5C in ModuleMusIns.cro, offset_to_top -768, 11 entries
class AcIsMuTigerBeetle : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuTigerBeetle>
{
public:
    AcIsMuTigerBeetle(); // ctor address unknown
    virtual ~AcIsMuTigerBeetle(); // ModuleMusIns.cro +0x00F3E0 slot 0x00
};
