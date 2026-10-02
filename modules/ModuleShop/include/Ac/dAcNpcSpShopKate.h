#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x26918 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopKate : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopKate(); // ctor address unknown
    virtual ~AcNpcSpShopKate(); // ModuleShop.cro +0x0072E4 slot 0x00
};
