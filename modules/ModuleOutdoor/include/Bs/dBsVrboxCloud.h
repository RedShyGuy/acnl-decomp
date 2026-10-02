#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x93B08 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxCloud : public ::UtlBase<Base>
{
public:
    BsVrboxCloud(); // ctor address unknown
    virtual ~BsVrboxCloud(); // ModuleOutdoor.cro +0x018210 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x017C88 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x017C30 slot 0x30
};
