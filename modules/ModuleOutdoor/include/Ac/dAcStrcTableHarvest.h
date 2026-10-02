#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x9730C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcTableHarvest : public ::UtlBase<AcStrc>
{
public:
    AcStrcTableHarvest(); // ctor address unknown
    virtual ~AcStrcTableHarvest(); // ModuleOutdoor.cro +0x05BA4C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x05B824 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05B7AC slot 0x30
};
