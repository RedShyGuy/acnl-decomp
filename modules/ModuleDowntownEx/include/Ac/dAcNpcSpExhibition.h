#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x331C in ModuleDowntownEx.cro, offset_to_top 0, 100 entries
class AcNpcSpExhibition : public ::AcNpcSpShop
{
public:
    AcNpcSpExhibition(); // ctor address unknown
    virtual ~AcNpcSpExhibition(); // ModuleDowntownEx.cro +0x000CE0 slot 0x00
    virtual void Initialize(); // ModuleDowntownEx.cro +0x002048 slot 0x0C
    virtual void Finalize(); // ModuleDowntownEx.cro +0x0020E4 slot 0x18
    virtual void Calc(); // ModuleDowntownEx.cro +0x000838 slot 0x24
    virtual void Draw(); // ModuleDowntownEx.cro +0x000780 slot 0x30
    virtual void Unk0(); // ModuleDowntownEx.cro +0x001CBC slot 0x3C
};
