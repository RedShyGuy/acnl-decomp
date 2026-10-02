#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// vtable +0x1383C in ModuleDream.cro, offset_to_top 0, 26 entries
class AreaList : public ::InstSelect<9>
{
public:
    AreaList(); // ctor address unknown
    virtual ~AreaList(); // ModuleDream.cro +0x0098E0 slot 0x00
};
