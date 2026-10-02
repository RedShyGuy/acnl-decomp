#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x27B40 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x27C54 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x27C6C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x27D94 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrUmbrella : public ::AcFtr
{
public:
    AcFtrUmbrella(); // ctor address unknown
    virtual ~AcFtrUmbrella(); // ModuleFtr.cro +0x00A31C slot 0x00
};
