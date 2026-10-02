#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x2738C in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopKodanuki : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopKodanuki(); // ctor address unknown
    virtual ~AcNpcSpShopKodanuki(); // ModuleShop.cro +0x01AAC0 slot 0x00
};
