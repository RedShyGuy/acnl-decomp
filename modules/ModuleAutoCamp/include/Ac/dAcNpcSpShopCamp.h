#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0xDBD4 in ModuleAutoCamp.cro, offset_to_top 0, 100 entries
class AcNpcSpShopCamp : public ::AcNpcSpShop
{
public:
    class AddSaveHook;
    class DownloadHook;
    class TalkRecept;
    AcNpcSpShopCamp(); // ctor address unknown
    virtual ~AcNpcSpShopCamp(); // ModuleAutoCamp.cro +0x008074 slot 0x00
    virtual void Calc(); // ModuleAutoCamp.cro +0x007988 slot 0x24
};
