#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x97788 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsVrboxCumulonimbus : public ::UtlBase<Base>
{
public:
    BsVrboxCumulonimbus(); // ctor address unknown
    virtual ~BsVrboxCumulonimbus(); // ModuleOutdoor.cro +0x05D9C4 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x05D640 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05D5F4 slot 0x30
};
