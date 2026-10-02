#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x97A4C in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x97B74 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class RecycleSignTalkRecept : public ::ObjTalkRecept
{
public:
    RecycleSignTalkRecept(); // ctor address unknown
    virtual ~RecycleSignTalkRecept(); // ModuleOutdoor.cro +0x05E5C0 slot 0x00
};
