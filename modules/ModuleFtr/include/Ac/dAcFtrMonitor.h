#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x268C8 in ModuleFtr.cro, offset_to_top 0, 71 entries
// vtable +0x269EC in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x26A04 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x26B2C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMonitor : public ::AcFtr
{
public:
    AcFtrMonitor(); // ctor address unknown
    virtual ~AcFtrMonitor(); // ModuleFtr.cro +0x006540 slot 0x00
};
