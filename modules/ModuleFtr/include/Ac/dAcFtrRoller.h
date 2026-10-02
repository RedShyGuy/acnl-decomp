#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x25918 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x25A2C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x25A44 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x25B6C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrRoller : public ::AcFtr
{
public:
    AcFtrRoller(); // ctor address unknown
    virtual ~AcFtrRoller(); // ModuleFtr.cro +0x0045C8 slot 0x00
};
