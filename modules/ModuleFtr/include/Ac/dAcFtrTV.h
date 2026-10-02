#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x2ACCC in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x2ADE0 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2ADF8 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2AF20 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrTV : public ::AcFtr
{
public:
    AcFtrTV(); // ctor address unknown
    virtual ~AcFtrTV(); // ModuleFtr.cro +0x01CA94 slot 0x00
};
