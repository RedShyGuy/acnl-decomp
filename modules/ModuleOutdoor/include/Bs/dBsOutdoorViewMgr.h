#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x960B0 in ModuleOutdoor.cro, offset_to_top 0, 22 entries
class BsOutdoorViewMgr : public ::UtlBase<Base>
{
public:
    BsOutdoorViewMgr(); // ctor address unknown
    virtual ~BsOutdoorViewMgr(); // ModuleOutdoor.cro +0x04DEFC slot 0x00
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x04D048 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x04CECC slot 0x30
};
