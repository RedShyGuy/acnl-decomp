#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// RTTI 23VersatileFlowTalkRecept @ 0x008CCF4C
// vtable 0x008F732C (vptr 0x008F7334), offset_to_top 0, 72 entries
// vtable 0x008F7454 (vptr 0x008F745C), offset_to_top -124, 14 entries
class VersatileFlowTalkRecept : public ::ObjTalkRecept
{
public:
    VersatileFlowTalkRecept(); // ctor address unknown
    virtual ~VersatileFlowTalkRecept(); // 0x00335E58 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00335E34 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x00335D68 slot 0x104 | virtual slot, introduced by ObjTalkRecept
};
