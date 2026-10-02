#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x26E00 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x26F14 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x26F2C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x27054 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrSwitch2 : public ::AcFtr
{
public:
    AcFtrSwitch2(); // ctor address unknown
    virtual ~AcFtrSwitch2(); // ModuleFtr.cro +0x006BCC slot 0x00
};
