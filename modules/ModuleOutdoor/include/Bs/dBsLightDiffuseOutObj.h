#pragma once

#include "decomp.h"
#include "Bs/dBsLightDiffuseBase.h"

// vtable +0x97950 in ModuleOutdoor.cro, offset_to_top 0, 26 entries
class BsLightDiffuseOutObj : public ::BsLightDiffuseBase
{
public:
    BsLightDiffuseOutObj(); // ctor address unknown
    virtual ~BsLightDiffuseOutObj(); // ModuleOutdoor.cro +0x05DDC4 slot 0x00
    virtual void Initialize(); // ModuleOutdoor.cro +0x05DCB0 slot 0x0C
    virtual void Finalize(); // ModuleOutdoor.cro +0x05DD80 slot 0x18
    virtual void Calc(); // ModuleOutdoor.cro +0x05DD60 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05DCA0 slot 0x30
};
