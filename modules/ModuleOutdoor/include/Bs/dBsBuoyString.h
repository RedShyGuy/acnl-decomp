#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x93AA8 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsBuoyString : public ::UtlBase<Base>
{
public:
    BsBuoyString(); // ctor address unknown
    virtual ~BsBuoyString(); // ModuleOutdoor.cro +0x017910 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x017818 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x0177B8 slot 0x30
};
