#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x977E8 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x97910 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class NpcWanderTalkRecept : public ::ObjTalkRecept
{
public:
    NpcWanderTalkRecept(); // ctor address unknown
    virtual ~NpcWanderTalkRecept(); // ModuleOutdoor.cro +0x05DB90 slot 0x00
};
