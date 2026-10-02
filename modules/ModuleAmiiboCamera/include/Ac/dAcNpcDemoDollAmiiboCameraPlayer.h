#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDemoDollAmiiboCameraBase.h"

// vtable +0x15BB4 in ModuleAmiiboCamera.cro, offset_to_top 0, 103 entries
class AcNpcDemoDollAmiiboCameraPlayer : public ::AcNpcDemoDollAmiiboCameraBase
{
public:
    AcNpcDemoDollAmiiboCameraPlayer(); // ctor address unknown
    virtual ~AcNpcDemoDollAmiiboCameraPlayer(); // ModuleAmiiboCamera.cro +0x00FC54 slot 0x00
};
