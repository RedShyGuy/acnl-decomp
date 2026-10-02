#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x94344 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxAurora : public ::UtlBase<Base>
{
public:
    BsVrboxAurora(); // ctor address unknown
    virtual ~BsVrboxAurora(); // ModuleOutdoor.cro +0x0001B5 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x01DB04 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x01DAB8 slot 0x30
};
