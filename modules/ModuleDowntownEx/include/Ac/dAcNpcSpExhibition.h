#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x331C in ModuleDowntownEx.cro, offset_to_top 0, 100 entries
class AcNpcSpExhibition : public ::AcNpcSpShop
{
public:
    AcNpcSpExhibition(); // ctor address unknown
    virtual ~AcNpcSpExhibition(); // ModuleDowntownEx.cro +0x000CE0 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleDowntownEx.cro +0x002048 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleDowntownEx.cro +0x0020E4 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleDowntownEx.cro +0x000838 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleDowntownEx.cro +0x000780 slot 0x30
    virtual bool Unk0(); // ModuleDowntownEx.cro +0x001CBC slot 0x3C
};
