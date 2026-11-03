#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixBase.h"

// vtable +0x142F0 in ModuleClub.cro, offset_to_top 0, 21 entries
class BsLightDiffuseClub : public ::BsLightFixBase
{
public:
    BsLightDiffuseClub(); // ctor address unknown
    virtual ~BsLightDiffuseClub(); // ModuleClub.cro +0x01012C slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleClub.cro +0x00FC88 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleClub.cro +0x0100E8 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleClub.cro +0x010028 slot 0x24
    virtual void Unk0(); // ModuleClub.cro +0x010FF4 slot 0x3C
};
