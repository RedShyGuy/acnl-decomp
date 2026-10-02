#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x94F44 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxRainbow : public ::UtlBase<Base>
{
public:
    BsVrboxRainbow(); // ctor address unknown
    virtual ~BsVrboxRainbow(); // ModuleOutdoor.cro +0x02BD0C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x02BAE0 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x02BA94 slot 0x30
};
