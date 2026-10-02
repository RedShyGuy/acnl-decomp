#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x27094 in ModuleFtr.cro, offset_to_top 0, 72 entries
// vtable +0x271BC in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x271D4 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x272FC in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTrigger : public ::AcFtr
{
public:
    AcFtrTrigger(); // ctor address unknown
    virtual ~AcFtrTrigger(); // ModuleFtr.cro +0x006F88 slot 0x00
};
