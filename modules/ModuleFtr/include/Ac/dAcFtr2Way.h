#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2B208 in ModuleFtr.cro, offset_to_top 0, 70 entries
// vtable +0x2B328 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2B340 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2B468 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtr2Way : public ::AcFtr
{
public:
    AcFtr2Way(); // ctor address unknown
    virtual ~AcFtr2Way(); // ModuleFtr.cro +0x01DDAC slot 0x00
    virtual void Initialize(); // ModuleFtr.cro +0x0223D4 slot 0x0C
    virtual void Finalize(); // ModuleFtr.cro +0x022470 slot 0x18
    virtual void Unk0(); // ModuleFtr.cro +0x01EB7C slot 0x3C
};
