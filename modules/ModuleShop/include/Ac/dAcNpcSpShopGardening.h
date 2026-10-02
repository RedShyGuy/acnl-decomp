#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x27720 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopGardening : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopGardening(); // ctor address unknown
    virtual ~AcNpcSpShopGardening(); // ModuleShop.cro +0x01E810 slot 0x00
};
