#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x10834 in ModuleTour.cro, offset_to_top 0, 100 entries
class AcNpcSpShopIsland : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopIsland(); // ctor address unknown
    virtual ~AcNpcSpShopIsland(); // ModuleTour.cro +0x008280 slot 0x00
};
