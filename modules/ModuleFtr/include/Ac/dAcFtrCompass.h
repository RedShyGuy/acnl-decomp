#pragma once

#include "decomp.h"
#include "Ac/dAcFtrLoop.h"

// vtable +0x26634 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x26748 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x26760 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x26888 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrCompass : public ::AcFtrLoop
{
public:
    AcFtrCompass(); // ctor address unknown
    virtual ~AcFtrCompass(); // ModuleFtr.cro +0x00602C slot 0x00
};
