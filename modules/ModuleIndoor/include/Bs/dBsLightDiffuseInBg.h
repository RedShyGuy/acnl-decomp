#pragma once

#include "decomp.h"
#include "Bs/dBsLightDiffuseBase.h"

// vtable +0x66B28 in ModuleIndoor.cro, offset_to_top 0, 26 entries
class BsLightDiffuseInBg : public ::BsLightDiffuseBase
{
public:
    BsLightDiffuseInBg(); // ctor address unknown
    virtual ~BsLightDiffuseInBg(); // ModuleIndoor.cro +0x038AD0 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x03890C slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x038A8C slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x038A6C slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x0388FC slot 0x30
};
