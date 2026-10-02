#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopCatherine.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x29440 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x29658 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopCatherine::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x0113EC slot 0x00
};
