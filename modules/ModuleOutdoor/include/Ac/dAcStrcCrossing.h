#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94960 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcCrossing : public ::UtlBase<AcStrc>
{
public:
    AcStrcCrossing(); // ctor address unknown
    virtual ~AcStrcCrossing(); // ModuleOutdoor.cro +0x023578 slot 0x00
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x022FF8 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x022FC4 slot 0x30
};
