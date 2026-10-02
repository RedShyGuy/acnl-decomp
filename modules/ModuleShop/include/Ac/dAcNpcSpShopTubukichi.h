#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopKodanuki.h"

// vtable +0x27BE8 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopTubukichi : public ::AcNpcSpShopKodanuki
{
public:
    AcNpcSpShopTubukichi(); // ctor address unknown
    virtual ~AcNpcSpShopTubukichi(); // ModuleShop.cro +0x021B64 slot 0x00
};
