#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// vtable +0x66C6C in ModuleIndoor.cro, offset_to_top 0, 21 entries
class BsLightHemiSphereIn : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereIn(); // ctor address unknown
    virtual ~BsLightHemiSphereIn(); // ModuleIndoor.cro +0x039200 slot 0x00
};
