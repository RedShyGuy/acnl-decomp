#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x93B68 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x93C90 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class ScreenRecept : public ::ObjTalkRecept
{
public:
    ScreenRecept(); // ctor address unknown
    virtual ~ScreenRecept(); // ModuleOutdoor.cro +0x018648 slot 0x00
};
