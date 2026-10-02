#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x26CC8 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopShoes : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopShoes(); // ctor address unknown
    virtual ~AcNpcSpShopShoes(); // ModuleShop.cro +0x00B370 slot 0x00
};
