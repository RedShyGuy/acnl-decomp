#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x28AC8 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x28BDC in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x28BF4 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x28D1C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrHouseParts : public ::AcFtr
{
public:
    AcFtrHouseParts(); // ctor address unknown
    virtual ~AcFtrHouseParts(); // ModuleFtr.cro +0x00E40C slot 0x00
};
