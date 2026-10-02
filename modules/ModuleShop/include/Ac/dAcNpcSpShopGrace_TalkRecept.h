#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopGrace.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x28608 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x28820 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopGrace::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x012688 slot 0x00
};
