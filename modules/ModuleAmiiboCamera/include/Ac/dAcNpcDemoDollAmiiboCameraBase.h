#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0x15A10 in ModuleAmiiboCamera.cro, offset_to_top 0, 103 entries
class AcNpcDemoDollAmiiboCameraBase : public ::AcNpc
{
public:
    AcNpcDemoDollAmiiboCameraBase(); // ctor address unknown
    virtual ~AcNpcDemoDollAmiiboCameraBase(); // ModuleAmiiboCamera.cro +0x007CB4 slot 0x00
};
