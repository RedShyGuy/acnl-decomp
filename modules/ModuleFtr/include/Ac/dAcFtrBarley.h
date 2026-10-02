#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x25158 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2526C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x25284 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x253AC in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrBarley : public ::AcFtr
{
public:
    AcFtrBarley(); // ctor address unknown
    virtual ~AcFtrBarley(); // ModuleFtr.cro +0x0033F8 slot 0x00
};
