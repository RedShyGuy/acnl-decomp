#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollAmiiboCameraBase.h"

// vtable +0x156C8 in ModuleAmiiboCamera.cro, offset_to_top 0, 103 entries
class AcNpcDemoDollAmiiboCamera : public ::AcNpcDemoDollAmiiboCameraBase
{
public:
    AcNpcDemoDollAmiiboCamera(); // ctor address unknown
    virtual ~AcNpcDemoDollAmiiboCamera(); // ModuleAmiiboCamera.cro +0x00E388 slot 0x00
    virtual void Initialize(); // ModuleAmiiboCamera.cro +0x012F90 slot 0x0C
    virtual void Finalize(); // ModuleAmiiboCamera.cro +0x01302C slot 0x18
    virtual void Calc(); // ModuleAmiiboCamera.cro +0x00F590 slot 0x24
    virtual void Draw(); // ModuleAmiiboCamera.cro +0x00F484 slot 0x30
    virtual void Unk0(); // ModuleAmiiboCamera.cro +0x012624 slot 0x3C
};
