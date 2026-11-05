#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollAmiiboCameraBase.h"

// vtable +0x156C8 in ModuleAmiiboCamera.cro, offset_to_top 0, 103 entries
class AcNpcDemoDollAmiiboCamera : public ::AcNpcDemoDollAmiiboCameraBase
{
public:
    AcNpcDemoDollAmiiboCamera(); // ctor address unknown
    virtual ~AcNpcDemoDollAmiiboCamera(); // ModuleAmiiboCamera.cro +0x00E388 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleAmiiboCamera.cro +0x012F90 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleAmiiboCamera.cro +0x01302C slot 0x18
    virtual oml::framework::Result Calc(); // ModuleAmiiboCamera.cro +0x00F590 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleAmiiboCamera.cro +0x00F484 slot 0x30
    virtual bool Unk0(); // ModuleAmiiboCamera.cro +0x012624 slot 0x3C
};
