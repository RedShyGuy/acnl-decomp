#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x66FC4 in ModuleIndoor.cro, offset_to_top 0, 100 entries
class AcNpcSpShopTunekichi : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopTunekichi(); // ctor address unknown
    virtual ~AcNpcSpShopTunekichi(); // ModuleIndoor.cro +0x03C628 slot 0x00
};
