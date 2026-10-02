#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x27384 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x27498 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x274B0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x275D8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrPushcart : public ::AcFtr
{
public:
    AcFtrPushcart(); // ctor address unknown
    virtual ~AcFtrPushcart(); // ModuleFtr.cro +0x009B44 slot 0x00
};
