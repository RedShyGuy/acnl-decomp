#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x97D28 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x97E50 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class NpcHousePlateTalkRecept : public ::ObjTalkRecept
{
public:
    NpcHousePlateTalkRecept(); // ctor address unknown
    virtual ~NpcHousePlateTalkRecept(); // ModuleOutdoor.cro +0x05E94C slot 0x00
};
