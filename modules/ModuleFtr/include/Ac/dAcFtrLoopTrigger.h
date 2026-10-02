#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x297F8 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2990C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x29924 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x29A4C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrLoopTrigger : public ::AcFtr
{
public:
    AcFtrLoopTrigger(); // ctor address unknown
    virtual ~AcFtrLoopTrigger(); // ModuleFtr.cro +0x016C04 slot 0x00
};
