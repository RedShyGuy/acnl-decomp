#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94A58 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcDemoPier : public ::UtlBase<AcStrc>
{
public:
    AcStrcDemoPier(); // ctor address unknown
    virtual ~AcStrcDemoPier(); // ModuleOutdoor.cro +0x0241C0 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x024070 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x02404C slot 0x30
};
