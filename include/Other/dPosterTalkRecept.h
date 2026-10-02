#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// RTTI 16PosterTalkRecept @ 0x008CC38C
// vtable 0x008F2BAC (vptr 0x008F2BB4), offset_to_top 0, 72 entries
// vtable 0x008F2CD4 (vptr 0x008F2CDC), offset_to_top -124, 14 entries
class PosterTalkRecept : public ::ObjTalkRecept
{
public:
    PosterTalkRecept(); // ctor candidate(s) 0x002BC680 (unverified)
    virtual ~PosterTalkRecept(); // 0x0023C204 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x002BC6A8 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x002BC634 slot 0x104 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x10C(); // 0x002BC2F0 slot 0x10C | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x110(); // 0x002BC440 slot 0x110 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x11C(); // 0x002BC53C slot 0x11C | virtual slot, introduced by ObjTalkRecept
};
