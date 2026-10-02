#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// vtable +0x13008 in ModuleDowntown.cro, offset_to_top 0, 16 entries
// vtable +0x13050 in ModuleDowntown.cro, offset_to_top -20, 3 entries
class BsMenuMapShoppingStreet : public ::Base, public ::state::Mode<BsMenuMapShoppingStreet>
{
public:
    BsMenuMapShoppingStreet(); // ctor address unknown
    virtual ~BsMenuMapShoppingStreet(); // ModuleDowntown.cro +0x0089C0 slot 0x00
    virtual void Initialize(); // ModuleDowntown.cro +0x00EEA8 slot 0x0C
    virtual void Finalize(); // ModuleDowntown.cro +0x00FA7C slot 0x18
    virtual void Calc(); // ModuleDowntown.cro +0x00F434 slot 0x24
    virtual void Draw(); // ModuleDowntown.cro +0x00EE70 slot 0x30
    virtual void Unk0(); // ModuleDowntown.cro +0x010A1C slot 0x3C
};
