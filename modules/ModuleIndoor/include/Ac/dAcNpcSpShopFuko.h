#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x660A0 in ModuleIndoor.cro, offset_to_top 0, 100 entries
class AcNpcSpShopFuko : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopFuko(); // ctor address unknown
    virtual ~AcNpcSpShopFuko(); // ModuleIndoor.cro +0x019D64 slot 0x00
};
