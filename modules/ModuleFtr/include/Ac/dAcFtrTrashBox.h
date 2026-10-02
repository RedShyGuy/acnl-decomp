#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x278AC in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x279C0 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x279D8 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x27B00 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTrashBox : public ::AcFtr
{
public:
    AcFtrTrashBox(); // ctor address unknown
    virtual ~AcFtrTrashBox(); // ModuleFtr.cro +0x00A2C8 slot 0x00
};
