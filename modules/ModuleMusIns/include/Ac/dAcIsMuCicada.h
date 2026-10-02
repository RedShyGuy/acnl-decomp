#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1FE4C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1FF0C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1FF40 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1FF78 in ModuleMusIns.cro, offset_to_top -732, 11 entries
class AcIsMuCicada : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuCicada>
{
public:
    AcIsMuCicada(); // ctor address unknown
    virtual ~AcIsMuCicada(); // ModuleMusIns.cro +0x003D44 slot 0x00
};
