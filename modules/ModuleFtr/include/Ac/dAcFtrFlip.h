#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2B4A8 in ModuleFtr.cro, offset_to_top 0, 75 entries
// vtable +0x2B5DC in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2B5F4 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2B71C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrFlip : public ::AcFtr
{
public:
    AcFtrFlip(); // ctor address unknown
    virtual ~AcFtrFlip(); // ModuleFtr.cro +0x01E204 slot 0x00
};
