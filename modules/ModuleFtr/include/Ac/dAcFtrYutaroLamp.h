#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x292AC in ModuleFtr.cro, offset_to_top 0, 69 entries
// vtable +0x293C8 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x293E0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x29508 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrYutaroLamp : public ::AcFtr
{
public:
    AcFtrYutaroLamp(); // ctor address unknown
    virtual ~AcFtrYutaroLamp(); // ModuleFtr.cro +0x016470 slot 0x00
};
