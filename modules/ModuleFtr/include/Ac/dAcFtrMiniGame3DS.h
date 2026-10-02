#pragma once

#include "decomp.h"
#include "Ac/dAcFtr2Way.h"

// vtable +0x29A8C in ModuleFtr.cro, offset_to_top 0, 73 entries
// vtable +0x29BB8 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x29BD0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x29CF8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMiniGame3DS : public ::AcFtr2Way
{
public:
    AcFtrMiniGame3DS(); // ctor address unknown
    virtual ~AcFtrMiniGame3DS(); // ModuleFtr.cro +0x016FA4 slot 0x00
};
