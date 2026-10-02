#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x943A4 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxForest : public ::UtlBase<Base>
{
public:
    BsVrboxForest(); // ctor address unknown
    virtual ~BsVrboxForest(); // ModuleOutdoor.cro +0x01E1A8 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x01E11C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x01E0FC slot 0x30
};
