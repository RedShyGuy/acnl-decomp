#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x24708 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2481C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x24834 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2495C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrChirp : public ::AcFtr
{
public:
    AcFtrChirp(); // ctor address unknown
    virtual ~AcFtrChirp(); // ModuleFtr.cro +0x002254 slot 0x00
};
