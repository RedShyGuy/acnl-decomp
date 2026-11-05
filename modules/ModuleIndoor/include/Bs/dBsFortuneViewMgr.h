#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x66654 in ModuleIndoor.cro, offset_to_top 0, 22 entries
class BsFortuneViewMgr : public ::UtlBase<Base>
{
public:
    BsFortuneViewMgr(); // ctor address unknown
    virtual ~BsFortuneViewMgr(); // ModuleIndoor.cro +0x021EEC slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x05F8CC slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x05F968 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x021CBC slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x021C04 slot 0x30
    virtual bool Unk0(); // ModuleIndoor.cro +0x05A838 slot 0x3C
};
