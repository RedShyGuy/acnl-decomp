#pragma once

#include "decomp.h"
#include "Ac/dAcFtrTrigger.h"

// vtable +0x29548 in ModuleFtr.cro, offset_to_top 0, 74 entries
// vtable +0x29678 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x29690 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x297B8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrGossipStone : public ::AcFtrTrigger
{
public:
    AcFtrGossipStone(); // ctor address unknown
    virtual ~AcFtrGossipStone(); // ModuleFtr.cro +0x01683C slot 0x00
};
