#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x25BAC in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x25CC0 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x25CD8 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x25E00 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrSwitch : public ::AcFtr
{
public:
    AcFtrSwitch(); // ctor address unknown
    virtual ~AcFtrSwitch(); // ModuleFtr.cro +0x0048B8 slot 0x00
};
