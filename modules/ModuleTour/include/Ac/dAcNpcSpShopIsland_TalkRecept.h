#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopIsland.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x1202C in ModuleTour.cro, offset_to_top 0, 132 entries
// vtable +0x12244 in ModuleTour.cro, offset_to_top -124, 14 entries
class AcNpcSpShopIsland::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTour.cro +0x0066C0 slot 0x00
};
