#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// vtable +0x676F4 in ModuleIndoor.cro, offset_to_top 0, 21 entries
class BsLightHemiSphereShowroom : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereShowroom(); // ctor address unknown
    virtual ~BsLightHemiSphereShowroom(); // ModuleIndoor.cro +0x041504 slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x041404 slot 0x0C
    virtual void Calc(); // ModuleIndoor.cro +0x0414C0 slot 0x24
};
