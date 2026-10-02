#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopKinuyo.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x28AB8 in ModuleShop.cro, offset_to_top 0, 132 entries
// vtable +0x28CD0 in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpShopKinuyo::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x01231C slot 0x00
};
