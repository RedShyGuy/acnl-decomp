#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2A260 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2A374 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2A38C in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2A4B4 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrRemakeOrgel : public ::AcFtr
{
public:
    AcFtrRemakeOrgel(); // ctor address unknown
    virtual ~AcFtrRemakeOrgel(); // ModuleFtr.cro +0x0183D0 slot 0x00
};
