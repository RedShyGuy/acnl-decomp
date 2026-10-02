#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// vtable +0x143A4 in ModuleClub.cro, offset_to_top 0, 21 entries
class BsLightHemiSphereClub : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereClub(); // ctor address unknown
    virtual ~BsLightHemiSphereClub(); // ModuleClub.cro +0x0109C4 slot 0x00
    virtual void Initialize(); // ModuleClub.cro +0x01061C slot 0x0C
    virtual void Finalize(); // ModuleClub.cro +0x01098C slot 0x18
    virtual void Calc(); // ModuleClub.cro +0x010964 slot 0x24
};
