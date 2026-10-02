#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x27190 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopRecycle : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopRecycle(); // ctor address unknown
    virtual ~AcNpcSpShopRecycle(); // ModuleShop.cro +0x016A40 slot 0x00
};
