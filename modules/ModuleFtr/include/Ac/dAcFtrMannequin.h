#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x28068 in ModuleFtr.cro, offset_to_top 0, 69 entries
// vtable +0x28184 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2819C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x282C4 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMannequin : public ::AcFtr
{
public:
    AcFtrMannequin(); // ctor address unknown
    virtual ~AcFtrMannequin(); // ModuleFtr.cro +0x0117BC slot 0x00
};
