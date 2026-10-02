#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopTunekichi.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x68F20 in ModuleIndoor.cro, offset_to_top 0, 132 entries
// vtable +0x69138 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpShopTunekichi::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x02FF78 slot 0x00
};
