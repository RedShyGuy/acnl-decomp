#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x29FCC in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2A0E0 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2A0F8 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2A220 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrOnlyCatalog : public ::AcFtr
{
public:
    AcFtrOnlyCatalog(); // ctor address unknown
    virtual ~AcFtrOnlyCatalog(); // ModuleFtr.cro +0x0174DC slot 0x00
};
