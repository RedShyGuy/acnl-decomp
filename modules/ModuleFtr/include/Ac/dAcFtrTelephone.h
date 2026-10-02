#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x28598 in ModuleFtr.cro, offset_to_top 0, 69 entries
// vtable +0x286B4 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x286CC in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x287F4 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTelephone : public ::AcFtr
{
public:
    AcFtrTelephone(); // ctor address unknown
    virtual ~AcFtrTelephone(); // ModuleFtr.cro +0x00DB98 slot 0x00
};
