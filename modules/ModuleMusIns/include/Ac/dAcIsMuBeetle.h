#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1FCAC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1FD6C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1FDA0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1FDD8 in ModuleMusIns.cro, offset_to_top -796, 11 entries
class AcIsMuBeetle : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuBeetle>
{
public:
    AcIsMuBeetle(); // ctor address unknown
    virtual ~AcIsMuBeetle(); // ModuleMusIns.cro +0x003778 slot 0x00
};
