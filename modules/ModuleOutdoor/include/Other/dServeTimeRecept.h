#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x95B90 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x95CB8 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class ServeTimeRecept : public ::ObjTalkRecept
{
public:
    ServeTimeRecept(); // ctor address unknown
    virtual ~ServeTimeRecept(); // ModuleOutdoor.cro +0x03525C slot 0x00
};
