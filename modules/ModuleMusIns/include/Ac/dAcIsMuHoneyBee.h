#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x20B44 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x20C04 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20C38 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x20C70 in ModuleMusIns.cro, offset_to_top -796, 11 entries
class AcIsMuHoneyBee : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuHoneyBee>
{
public:
    AcIsMuHoneyBee(); // ctor address unknown
    virtual ~AcIsMuHoneyBee(); // ModuleMusIns.cro +0x007934 slot 0x00
};
