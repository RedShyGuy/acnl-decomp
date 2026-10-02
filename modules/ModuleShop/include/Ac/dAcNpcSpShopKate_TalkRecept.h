#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopKate.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x283B0 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x285C8 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopKate::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x0128FC slot 0x00
};
