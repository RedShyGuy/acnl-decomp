#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x95A98 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcIslandHut : public ::UtlBase<AcStrc>
{
public:
    AcStrcIslandHut(); // ctor address unknown
    virtual ~AcStrcIslandHut(); // ModuleOutdoor.cro +0x03516C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x03501C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x034F4C slot 0x30
};
