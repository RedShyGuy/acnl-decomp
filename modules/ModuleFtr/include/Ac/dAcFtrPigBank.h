#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x26B6C in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x26C80 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x26C98 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x26DC0 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrPigBank : public ::AcFtr
{
public:
    AcFtrPigBank(); // ctor address unknown
    virtual ~AcFtrPigBank(); // ModuleFtr.cro +0x00676C slot 0x00
};
