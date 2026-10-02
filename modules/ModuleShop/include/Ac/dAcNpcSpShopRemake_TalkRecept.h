#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopRemake.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x28D30 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x28F48 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopRemake::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x0120F4 slot 0x00
};
