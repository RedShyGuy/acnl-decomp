#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x22528 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x225E8 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x2261C in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x22654 in ModuleMusIns.cro, offset_to_top -784, 11 entries
class AcIsMuFly : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuFly>
{
public:
    AcIsMuFly(); // ctor address unknown
    virtual ~AcIsMuFly(); // ModuleMusIns.cro +0x011978 slot 0x00
};
