#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// vtable +0x664A4 in ModuleIndoor.cro, offset_to_top 0, 22 entries
class BsIndoorViewMgr : public ::UtlBase<Base>
{
public:
    BsIndoorViewMgr(); // ctor address unknown
    virtual ~BsIndoorViewMgr(); // ModuleIndoor.cro +0x020214 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x01F840 slot 0x24
    virtual void Draw(); // ModuleIndoor.cro +0x01F69C slot 0x30
};
