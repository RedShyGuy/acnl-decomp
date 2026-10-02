#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x28834 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x28948 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x28960 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x28A88 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrFlipSwitch : public ::AcFtr
{
public:
    AcFtrFlipSwitch(); // ctor address unknown
    virtual ~AcFtrFlipSwitch(); // ModuleFtr.cro +0x00E004 slot 0x00
};
