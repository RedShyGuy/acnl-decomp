#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// RTTI 24VersatilePlateTalkRecept @ 0x008CD030
// vtable 0x008F7948 (vptr 0x008F7950), offset_to_top 0, 72 entries
// vtable 0x008F7A70 (vptr 0x008F7A78), offset_to_top -124, 14 entries
class VersatilePlateTalkRecept : public ::ObjTalkRecept
{
public:
    VersatilePlateTalkRecept(); // ctor address unknown
    virtual ~VersatilePlateTalkRecept(); // 0x0033EFF4 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0033EFD0 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x0033EEE8 slot 0x104 | virtual slot, introduced by ObjTalkRecept
};
