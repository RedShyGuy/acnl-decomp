#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopKodanuki.h"

// vtable +0x278B8 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopMamekichi : public ::AcNpcSpShopKodanuki
{
public:
    AcNpcSpShopMamekichi(); // ctor address unknown
    virtual ~AcNpcSpShopMamekichi(); // ModuleShop.cro +0x01ECEC slot 0x00
};
