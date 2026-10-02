#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x27A50 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopTanukichi : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopTanukichi(); // ctor address unknown
    virtual ~AcNpcSpShopTanukichi(); // ModuleShop.cro +0x021A54 slot 0x00
};
