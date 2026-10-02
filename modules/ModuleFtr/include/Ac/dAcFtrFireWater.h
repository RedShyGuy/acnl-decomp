#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x27DD4 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x27EE8 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x27F00 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x28028 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrFireWater : public ::AcFtr
{
public:
    AcFtrFireWater(); // ctor address unknown
    virtual ~AcFtrFireWater(); // ModuleFtr.cro +0x00A738 slot 0x00
};
