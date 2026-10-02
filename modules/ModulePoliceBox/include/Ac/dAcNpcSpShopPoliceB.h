#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShopPolice.h"

// vtable +0x24FC in ModulePoliceBox.cro, offset_to_top 0, 100 entries
class AcNpcSpShopPoliceB : public ::AcNpcSpShopPolice
{
public:
    AcNpcSpShopPoliceB(); // ctor address unknown
    virtual ~AcNpcSpShopPoliceB(); // ModulePoliceBox.cro +0x0018B8 slot 0x00
};
