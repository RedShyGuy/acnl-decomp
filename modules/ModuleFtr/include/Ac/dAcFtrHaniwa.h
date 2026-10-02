#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x253EC in ModuleFtr.cro, offset_to_top 0, 68 entries
// vtable +0x25504 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2551C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x25644 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrHaniwa : public ::AcFtr
{
public:
    AcFtrHaniwa(); // ctor address unknown
    virtual ~AcFtrHaniwa(); // ModuleFtr.cro +0x004280 slot 0x00
};
