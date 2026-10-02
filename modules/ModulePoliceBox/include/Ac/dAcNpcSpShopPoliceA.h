#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopPolice.h"

// vtable +0x2364 in ModulePoliceBox.cro, offset_to_top 0, 100 entries
class AcNpcSpShopPoliceA : public ::AcNpcSpShopPolice
{
public:
    AcNpcSpShopPoliceA(); // ctor address unknown
    virtual ~AcNpcSpShopPoliceA(); // ModulePoliceBox.cro +0x00166C slot 0x00
};
