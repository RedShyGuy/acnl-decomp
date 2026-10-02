#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x96D3C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcRecycleShop : public ::UtlBase<AcStrc>
{
public:
    AcStrcRecycleShop(); // ctor address unknown
    virtual ~AcStrcRecycleShop(); // ModuleOutdoor.cro +0x055F64 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x055858 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x055714 slot 0x30
};
