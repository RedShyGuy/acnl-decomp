#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereIn.h"

// vtable +0x67698 in ModuleIndoor.cro, offset_to_top 0, 21 entries
class BsLightHemiSphereDarkroom : public ::BsLightHemiSphereIn
{
public:
    BsLightHemiSphereDarkroom(); // ctor address unknown
    virtual ~BsLightHemiSphereDarkroom(); // ModuleIndoor.cro +0x0412AC slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleIndoor.cro +0x038FEC slot 0x0C
    virtual oml::framework::Result Calc(); // ModuleIndoor.cro +0x039168 slot 0x24
};
