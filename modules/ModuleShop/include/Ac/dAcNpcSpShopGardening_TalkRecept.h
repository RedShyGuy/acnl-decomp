#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopGardening.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x29698 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x298B0 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopGardening::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x011340 slot 0x00
};
