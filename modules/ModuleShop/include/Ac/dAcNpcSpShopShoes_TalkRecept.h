#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopShoes.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x28860 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x28A78 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopShoes::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x0125DC slot 0x00
};
