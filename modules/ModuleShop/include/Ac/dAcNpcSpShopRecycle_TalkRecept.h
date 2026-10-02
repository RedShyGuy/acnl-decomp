#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopRecycle.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x28F88 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x291A0 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopRecycle::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x012048 slot 0x00
};
