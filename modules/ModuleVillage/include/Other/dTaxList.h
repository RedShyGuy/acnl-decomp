#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// vtable +0x20FAC in ModuleVillage.cro, offset_to_top 0, 26 entries
class TaxList : public ::InstSelect<8>
{
public:
    TaxList(); // ctor address unknown
    virtual ~TaxList(); // ModuleVillage.cro +0x018F40 slot 0x00
};
