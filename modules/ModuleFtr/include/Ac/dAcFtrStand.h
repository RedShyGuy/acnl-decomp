#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x24EC4 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x24FD8 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x24FF0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x25118 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrStand : public ::AcFtr
{
public:
    AcFtrStand(); // ctor address unknown
    virtual ~AcFtrStand(); // ModuleFtr.cro +0x0030B8 slot 0x00
};
