#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixBase.h"

// vtable +0x672A4 in ModuleIndoor.cro, offset_to_top 0, 21 entries
class BsLightDiffuseDarkroom : public ::BsLightFixBase
{
public:
    BsLightDiffuseDarkroom(); // ctor address unknown
    virtual ~BsLightDiffuseDarkroom(); // ModuleIndoor.cro +0x03E95C slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x03E850 slot 0x0C
    virtual void Calc(); // ModuleIndoor.cro +0x03E918 slot 0x24
    virtual void Unk0(); // ModuleIndoor.cro +0x05939C slot 0x3C
};
