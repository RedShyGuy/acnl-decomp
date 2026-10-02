#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixBase.h"

// vtable +0x67300 in ModuleIndoor.cro, offset_to_top 0, 21 entries
class BsLightDiffuseShowroom : public ::BsLightFixBase
{
public:
    BsLightDiffuseShowroom(); // ctor address unknown
    virtual ~BsLightDiffuseShowroom(); // ModuleIndoor.cro +0x03EBD0 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x03EAB8 slot 0x0C
    virtual void Calc(); // ModuleIndoor.cro +0x03EB8C slot 0x24
};
