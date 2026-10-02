#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopFuko.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x6876C in ModuleIndoor.cro, offset_to_top 0, 132 entries
// vtable +0x68984 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpShopFuko::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x033C64 slot 0x00
};
