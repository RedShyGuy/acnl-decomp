#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x92900 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x92A28 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class BoardRecept : public ::ObjTalkRecept
{
public:
    BoardRecept(); // ctor address unknown
    virtual ~BoardRecept(); // ModuleOutdoor.cro +0x00CD18 slot 0x00
};
