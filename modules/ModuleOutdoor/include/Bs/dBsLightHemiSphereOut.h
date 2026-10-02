#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// vtable +0x979C0 in ModuleOutdoor.cro, offset_to_top 0, 21 entries
class BsLightHemiSphereOut : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereOut(); // ctor address unknown
    virtual ~BsLightHemiSphereOut(); // ModuleOutdoor.cro +0x05E124 slot 0x00
    virtual void Initialize(); // ModuleOutdoor.cro +0x05DF1C slot 0x0C
    virtual void Calc(); // ModuleOutdoor.cro +0x05E070 slot 0x24
};
