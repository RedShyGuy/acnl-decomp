#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x30EF0 in ModuleMiniGame1.cro, offset_to_top 0, 22 entries
class BsMiniGame1Mgr : public ::UtlBase<Base>
{
public:
    BsMiniGame1Mgr(); // ctor address unknown
    virtual ~BsMiniGame1Mgr(); // ModuleMiniGame1.cro +0x012264 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleMiniGame1.cro +0x02E4F4 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleMiniGame1.cro +0x02E590 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleMiniGame1.cro +0x011E14 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleMiniGame1.cro +0x011D30 slot 0x30
    virtual void Unk0(); // ModuleMiniGame1.cro +0x02CCB4 slot 0x3C
};
