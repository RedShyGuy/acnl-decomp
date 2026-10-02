#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// RTTI 21DonationBoxTalkRecept @ 0x008CCCD8
// vtable 0x008F6170 (vptr 0x008F6178), offset_to_top 0, 72 entries
// vtable 0x008F6298 (vptr 0x008F62A0), offset_to_top -124, 14 entries
class DonationBoxTalkRecept : public ::ObjTalkRecept
{
public:
    DonationBoxTalkRecept(); // ctor address unknown
    virtual ~DonationBoxTalkRecept(); // 0x00328694 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00328684 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0xF8(); // 0x007258A4 slot 0xF8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x100(); // 0x00328660 slot 0x100 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x104(); // 0x003285F0 slot 0x104 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x108(); // 0x00328640 slot 0x108 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x10C(); // 0x003283E4 slot 0x10C | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x110(); // 0x00328454 slot 0x110 | virtual slot, introduced by ObjTalkRecept
};
