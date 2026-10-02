#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2B75C in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2B870 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2B888 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2B9B0 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrLoop : public ::AcFtr
{
public:
    AcFtrLoop(); // ctor address unknown
    virtual ~AcFtrLoop(); // ModuleFtr.cro +0x01E828 slot 0x00
};
