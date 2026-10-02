#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopTanukichi.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x298F0 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x29B08 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopTanukichi::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x01117C slot 0x00
};
