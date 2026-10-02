#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2499C in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x24AB0 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x24AC8 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x24BF0 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrClock : public ::AcFtr
{
public:
    AcFtrClock(); // ctor address unknown
    virtual ~AcFtrClock(); // ModuleFtr.cro +0x002604 slot 0x00
};
