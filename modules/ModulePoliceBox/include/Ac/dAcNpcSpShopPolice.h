#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x21CC in ModulePoliceBox.cro, offset_to_top 0, 100 entries
class AcNpcSpShopPolice : public ::AcNpcSpShop
{
public:
    class TalkRecept;
    AcNpcSpShopPolice(); // ctor address unknown
    virtual ~AcNpcSpShopPolice(); // ModulePoliceBox.cro +0x00155C slot 0x00
    virtual void Initialize(); // ModulePoliceBox.cro +0x001E18 slot 0x0C
    virtual void Finalize(); // ModulePoliceBox.cro +0x001EB4 slot 0x18
    virtual void Unk0(); // ModulePoliceBox.cro +0x001C9C slot 0x3C
};
