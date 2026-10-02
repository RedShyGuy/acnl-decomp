#pragma once

#include "decomp.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"

// RTTI 13ObjTalkRecept @ 0x008CBA4C
// vtable 0x008EF550 (vptr 0x008EF558), offset_to_top 0, 72 entries
// vtable 0x008EF678 (vptr 0x008EF680), offset_to_top -124, 14 entries
class ObjTalkRecept : public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    ObjTalkRecept(); // ctor candidate(s) 0x0023C1A0 (unverified)
    virtual ~ObjTalkRecept(); // 0x0023C208 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0023C1E8 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x94(); // 0x0023C19C slot 0x94 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x98(); // 0x0023BAC4 slot 0x98 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x9C(); // 0x0023B874 slot 0x9C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA0(); // 0x0023B76C slot 0xA0 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA4(); // 0x0023B86C slot 0xA4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xAC(); // 0x0023BAC0 slot 0xAC | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xF8(); // 0x0071465C slot 0xF8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xFC(); // 0x0023B374 slot 0xFC | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x100(); // 0x0023B870 slot 0x100 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x104(); // 0x0023B7D4 slot 0x104 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x108(); // 0x0023B7D8 slot 0x108 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x10C(); // 0x0023B378 slot 0x10C | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x110(); // 0x005DA270 slot 0x110 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x114(); // 0x0023B768 slot 0x114 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x118(); // 0x0023B4D8 slot 0x118 | virtual slot, introduced by ObjTalkRecept
    virtual void vf_0x11C(); // 0x0023B7D0 slot 0x11C | virtual slot, introduced by ObjTalkRecept
};
