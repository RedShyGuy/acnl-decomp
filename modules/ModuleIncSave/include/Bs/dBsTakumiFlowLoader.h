#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x1E244 in ModuleIncSave.cro, offset_to_top 0, 16 entries
class BsTakumiFlowLoader : public ::Base
{
public:
    BsTakumiFlowLoader(); // ctor address unknown
    virtual ~BsTakumiFlowLoader(); // ModuleIncSave.cro +0x006E3C slot 0x00
    virtual void Initialize(); // ModuleIncSave.cro +0x006DA4 slot 0x0C
    virtual void Finalize(); // ModuleIncSave.cro +0x006DF4 slot 0x18
    virtual void Calc(); // ModuleIncSave.cro +0x006DEC slot 0x24
    virtual void Draw(); // ModuleIncSave.cro +0x006D9C slot 0x30
};
