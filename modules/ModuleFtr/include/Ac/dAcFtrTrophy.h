#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x260D4 in ModuleFtr.cro, offset_to_top 0, 74 entries
// vtable +0x26204 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2621C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x26344 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTrophy : public ::AcFtr
{
public:
    AcFtrTrophy(); // ctor address unknown
    virtual ~AcFtrTrophy(); // ModuleFtr.cro +0x005148 slot 0x00
};
