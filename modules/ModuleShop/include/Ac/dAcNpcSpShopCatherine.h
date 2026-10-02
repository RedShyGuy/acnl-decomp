#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x27588 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopCatherine : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopCatherine(); // ctor address unknown
    virtual ~AcNpcSpShopCatherine(); // ModuleShop.cro +0x01D618 slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x01D138 slot 0x24
};
