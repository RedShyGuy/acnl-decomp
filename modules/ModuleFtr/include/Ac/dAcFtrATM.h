#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2AF60 in ModuleFtr.cro, offset_to_top 0, 72 entries
// vtable +0x2B088 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2B0A0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2B1C8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrATM : public ::AcFtr
{
public:
    AcFtrATM(); // ctor address unknown
    virtual ~AcFtrATM(); // ModuleFtr.cro +0x01D78C slot 0x00
};
