#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// vtable +0x132EC in ModuleDream.cro, offset_to_top 0, 26 entries
class ResultList : public ::InstSelect<10>
{
public:
    ResultList(); // ctor address unknown
    virtual ~ResultList(); // ModuleDream.cro +0x0015C4 slot 0x00
};
