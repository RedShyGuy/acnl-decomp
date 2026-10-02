#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x97614 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcCarnivalStage : public ::UtlBase<AcStrc>
{
public:
    AcStrcCarnivalStage(); // ctor address unknown
    virtual ~AcStrcCarnivalStage(); // ModuleOutdoor.cro +0x05D1C0 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x05CF2C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05CEB4 slot 0x30
};
