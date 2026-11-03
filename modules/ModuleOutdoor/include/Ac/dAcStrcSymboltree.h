#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x95FB8 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcSymboltree : public ::UtlBase<AcStrc>
{
public:
    AcStrcSymboltree(); // ctor address unknown
    virtual ~AcStrcSymboltree(); // ModuleOutdoor.cro +0x038D08 slot 0x00
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x038584 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x03847C slot 0x30
};
