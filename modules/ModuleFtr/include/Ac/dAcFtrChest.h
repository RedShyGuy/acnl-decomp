#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x24470 in ModuleFtr.cro, offset_to_top 0, 68 entries
// vtable +0x24588 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x245A0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x246C8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrChest : public ::AcFtr
{
public:
    AcFtrChest(); // ctor address unknown
    virtual ~AcFtrChest(); // ModuleFtr.cro +0x001F18 slot 0x00
};
