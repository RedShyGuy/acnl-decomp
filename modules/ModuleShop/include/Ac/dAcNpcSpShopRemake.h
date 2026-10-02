#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x26FF8 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopRemake : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopRemake(); // ctor address unknown
    virtual ~AcNpcSpShopRemake(); // ModuleShop.cro +0x01108C slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x010CE0 slot 0x24
};
