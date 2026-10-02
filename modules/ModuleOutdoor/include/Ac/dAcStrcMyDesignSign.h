#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x9711C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcMyDesignSign : public ::UtlBase<AcStrc>
{
public:
    AcStrcMyDesignSign(); // ctor address unknown
    virtual ~AcStrcMyDesignSign(); // ModuleOutdoor.cro +0x05A2C4 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x05A170 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05A0F0 slot 0x30
};
