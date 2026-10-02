#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopKodanuki.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x291E0 in ModuleShop.cro, offset_to_top 0, 134 entries
// vtable +0x29400 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopKodanuki::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x011F2C slot 0x00
};
