#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21508 in ModuleMusIns.cro, offset_to_top 0, 46 entries
// vtable +0x215CC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21600 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21638 in ModuleMusIns.cro, offset_to_top -1180, 11 entries
class AcIsMuTumblebug : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuTumblebug>
{
public:
    AcIsMuTumblebug(); // ctor address unknown
    virtual ~AcIsMuTumblebug(); // ModuleMusIns.cro +0x00D600 slot 0x00
    virtual void Draw(); // ModuleMusIns.cro +0x00D178 slot 0x30
};
