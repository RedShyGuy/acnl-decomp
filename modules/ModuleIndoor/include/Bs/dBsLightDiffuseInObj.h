#pragma once

#include "decomp.h"
#include "Bs/dBsLightDiffuseBase.h"

// vtable +0x66BFC in ModuleIndoor.cro, offset_to_top 0, 26 entries
class BsLightDiffuseInObj : public ::BsLightDiffuseBase
{
public:
    BsLightDiffuseInObj(); // ctor address unknown
    virtual ~BsLightDiffuseInObj(); // ModuleIndoor.cro +0x038E44 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x038CAC slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleIndoor.cro +0x038E00 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x038DE0 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleIndoor.cro +0x038C9C slot 0x30
};
