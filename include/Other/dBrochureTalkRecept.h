#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// RTTI 18BrochureTalkRecept @ 0x008CC728
// vtable 0x008F3EA8 (vptr 0x008F3EB0), offset_to_top 0, 72 entries
// vtable 0x008F3FD0 (vptr 0x008F3FD8), offset_to_top -124, 14 entries
class BrochureTalkRecept : public ::ObjTalkRecept
{
public:
    BrochureTalkRecept(); // ctor address unknown
    virtual ~BrochureTalkRecept(); // 0x002D6C1C slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x002D6C0C slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x002D6BEC slot 0x104 | virtual slot, introduced by ObjTalkRecept
};
