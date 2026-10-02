#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x923EC in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x92514 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class SignRecept : public ::ObjTalkRecept
{
public:
    SignRecept(); // ctor address unknown
    virtual ~SignRecept(); // ModuleOutdoor.cro +0x00A1B4 slot 0x00
};
