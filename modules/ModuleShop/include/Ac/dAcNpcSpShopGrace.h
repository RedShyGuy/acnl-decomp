#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x26B30 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopGrace : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopGrace(); // ctor address unknown
    virtual ~AcNpcSpShopGrace(); // ModuleShop.cro +0x00A388 slot 0x00
};
