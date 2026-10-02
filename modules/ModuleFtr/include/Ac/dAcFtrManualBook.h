#pragma once

#include "decomp.h"
#include "Ac/dAcFtrFlip.h"

// vtable +0x28FF0 in ModuleFtr.cro, offset_to_top 0, 77 entries
// vtable +0x2912C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x29144 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2926C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrManualBook : public ::AcFtrFlip
{
public:
    AcFtrManualBook(); // ctor address unknown
    virtual ~AcFtrManualBook(); // ModuleFtr.cro +0x00EB7C slot 0x00
};
