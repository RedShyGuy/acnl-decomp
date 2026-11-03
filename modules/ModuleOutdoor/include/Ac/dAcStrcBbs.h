#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x987E4 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcBbs : public ::UtlBase<AcStrc>
{
public:
    AcStrcBbs(); // ctor address unknown
    virtual ~AcStrcBbs(); // ModuleOutdoor.cro +0x07B9C0 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleOutdoor.cro +0x0830C4 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleOutdoor.cro +0x083160 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x07B710 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x07B6DC slot 0x30
};
