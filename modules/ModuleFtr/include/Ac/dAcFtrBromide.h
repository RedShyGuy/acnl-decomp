#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x26384 in ModuleFtr.cro, offset_to_top 0, 74 entries
// vtable +0x264B4 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x264CC in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x265F4 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrBromide : public ::AcFtr
{
public:
    AcFtrBromide(); // ctor address unknown
    virtual ~AcFtrBromide(); // ModuleFtr.cro +0x005D20 slot 0x00
};
