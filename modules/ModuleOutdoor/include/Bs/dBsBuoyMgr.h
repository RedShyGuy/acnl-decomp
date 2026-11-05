#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x988DC in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsBuoyMgr : public ::UtlBase<Base>
{
public:
    BsBuoyMgr(); // ctor address unknown
    virtual ~BsBuoyMgr(); // ModuleOutdoor.cro +0x07D160 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleOutdoor.cro +0x082F30 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleOutdoor.cro +0x082FCC slot 0x18
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x07CC60 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x07C8D8 slot 0x30
    virtual bool Unk0(); // ModuleOutdoor.cro +0x07E638 slot 0x3C
};
