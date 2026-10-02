#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2AA38 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2AB4C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2AB64 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2AC8C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMyDesignObject : public ::AcFtr
{
public:
    AcFtrMyDesignObject(); // ctor address unknown
    virtual ~AcFtrMyDesignObject(); // ModuleFtr.cro +0x01A198 slot 0x00
};
