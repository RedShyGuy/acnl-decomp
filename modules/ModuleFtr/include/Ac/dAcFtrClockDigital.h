#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2A4F4 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2A608 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2A620 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2A748 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrClockDigital : public ::AcFtr
{
public:
    AcFtrClockDigital(); // ctor address unknown
    virtual ~AcFtrClockDigital(); // ModuleFtr.cro +0x0188E4 slot 0x00
};
