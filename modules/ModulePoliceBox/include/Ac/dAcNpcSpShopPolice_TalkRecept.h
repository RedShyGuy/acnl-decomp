#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShopPolice.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x2694 in ModulePoliceBox.cro, offset_to_top 0, 133 entries
// vtable +0x28B0 in ModulePoliceBox.cro, offset_to_top -124, 14 entries
class AcNpcSpShopPolice::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePoliceBox.cro +0x00119C slot 0x00
};
