#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x27618 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2772C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x27744 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2786C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTheremin : public ::AcFtr
{
public:
    AcFtrTheremin(); // ctor address unknown
    virtual ~AcFtrTheremin(); // ModuleFtr.cro +0x009D9C slot 0x00
};
