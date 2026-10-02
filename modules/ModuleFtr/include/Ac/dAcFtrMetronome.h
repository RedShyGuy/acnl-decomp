#pragma once

#include "decomp.h"
#include "Ac/dAcFtrLoop.h"

// vtable +0x28304 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x28418 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x28430 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x28558 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMetronome : public ::AcFtrLoop
{
public:
    AcFtrMetronome(); // ctor address unknown
    virtual ~AcFtrMetronome(); // ModuleFtr.cro +0x00D468 slot 0x00
};
