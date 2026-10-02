#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x26670 in ModuleShop.cro, offset_to_top 0, 89 entries
class AcNpcSpAsami : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpAsami(); // ctor address unknown
    virtual ~AcNpcSpAsami(); // ModuleShop.cro +0x0034A0 slot 0x00
    virtual void Initialize(); // ModuleShop.cro +0x0243C4 slot 0x0C
    virtual void Finalize(); // ModuleShop.cro +0x024460 slot 0x18
    virtual void Unk0(); // ModuleShop.cro +0x0230D0 slot 0x3C
};
