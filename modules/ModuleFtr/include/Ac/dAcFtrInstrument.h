#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x28D5C in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x28E70 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x28E88 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x28FB0 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrInstrument : public ::AcFtr
{
public:
    AcFtrInstrument(); // ctor address unknown
    virtual ~AcFtrInstrument(); // ModuleFtr.cro +0x00E664 slot 0x00
};
