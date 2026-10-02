#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpExhibitionOut.h"
#include "Ac/dAcNpcSpShop.h"
#include "Ac/dAcNpcSpShop_ShopNpcTalkRecept.h"

// vtable +0x13064 in ModuleDowntown.cro, offset_to_top 0, 132 entries
// vtable +0x1327C in ModuleDowntown.cro, offset_to_top -124, 14 entries
class AcNpcSpExhibitionOut::TalkRecept : public ::AcNpcSpShop::ShopNpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleDowntown.cro +0x008D94 slot 0x00
};
