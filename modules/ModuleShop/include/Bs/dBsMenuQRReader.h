#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// vtable +0x26840 in ModuleShop.cro, offset_to_top 0, 25 entries
// vtable +0x268AC in ModuleShop.cro, offset_to_top -40, 3 entries
class BsMenuQRReader : public ::MenuBase, public ::state::Mode<BsMenuQRReader>
{
public:
    BsMenuQRReader(); // ctor address unknown
    virtual ~BsMenuQRReader(); // ModuleShop.cro +0x0129A8 slot 0x00
    virtual void Initialize(); // ModuleShop.cro +0x005470 slot 0x0C
    virtual void Finalize(); // ModuleShop.cro +0x005D58 slot 0x18
    virtual void Calc(); // ModuleShop.cro +0x005C14 slot 0x24
    virtual void Draw(); // ModuleShop.cro +0x0053FC slot 0x30
};
