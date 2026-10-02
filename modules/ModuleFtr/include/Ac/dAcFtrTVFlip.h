#pragma once

#include "decomp.h"
#include "Ac/dAcFtrTV.h"

// vtable +0x25E40 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x25F54 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x25F6C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x26094 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTVFlip : public ::AcFtrTV
{
public:
    AcFtrTVFlip(); // ctor address unknown
    virtual ~AcFtrTVFlip(); // ModuleFtr.cro +0x004D1C slot 0x00
};
