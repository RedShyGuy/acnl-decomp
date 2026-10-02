#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopCamp.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0xDF70 in ModuleAutoCamp.cro, offset_to_top 0, 132 entries
// vtable +0xE188 in ModuleAutoCamp.cro, offset_to_top -124, 14 entries
class AcNpcSpShopCamp::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleAutoCamp.cro +0x00661C slot 0x00
};
