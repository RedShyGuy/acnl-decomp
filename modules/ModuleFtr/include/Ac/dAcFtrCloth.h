#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x24C30 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x24D44 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x24D5C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x24E84 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrCloth : public ::AcFtr
{
public:
    AcFtrCloth(); // ctor address unknown
    virtual ~AcFtrCloth(); // ModuleFtr.cro +0x002BBC slot 0x00
};
