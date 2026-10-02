#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpExhibitionIn.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x8AB0 in ModuleExhibition.cro, offset_to_top 0, 132 entries
// vtable +0x8CC8 in ModuleExhibition.cro, offset_to_top -124, 14 entries
class AcNpcSpExhibitionIn::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleExhibition.cro +0x003F10 slot 0x00
};
