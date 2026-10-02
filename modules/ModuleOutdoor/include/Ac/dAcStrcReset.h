#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x92710 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcReset : public ::UtlBase<AcStrc>
{
public:
    AcStrcReset(); // ctor address unknown
    virtual ~AcStrcReset(); // ModuleOutdoor.cro +0x00BAC4 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x00B920 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x00B8A8 slot 0x30
};
