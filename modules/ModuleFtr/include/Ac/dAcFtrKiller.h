#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x25684 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x25798 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x257B0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x258D8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrKiller : public ::AcFtr
{
public:
    AcFtrKiller(); // ctor address unknown
    virtual ~AcFtrKiller(); // ModuleFtr.cro +0x0045A4 slot 0x00
};
