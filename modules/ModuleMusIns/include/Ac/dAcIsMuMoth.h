#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1F79C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1F85C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1F890 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1F8C8 in ModuleMusIns.cro, offset_to_top -760, 11 entries
class AcIsMuMoth : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuMoth>
{
public:
    AcIsMuMoth(); // ctor address unknown
    virtual ~AcIsMuMoth(); // ModuleMusIns.cro +0x00178C slot 0x00
};
