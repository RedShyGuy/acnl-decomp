#pragma once

#include "decomp.h"
#include "Bs/dBsEventMgr.h"
#include "script/dITalkRecept.h"

// RTTI N10BsEventMgr6ReceptE @ 0x008CD808
// vtable 0x008FA854 (vptr 0x008FA85C), offset_to_top 0, 63 entries
class BsEventMgr::Recept : public ::script::ITalkRecept
{
public:
    Recept(); // ctor address unknown
    virtual ~Recept(); // 0x00316D78 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0018FA0C slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0xF8(); // 0x0070B2C0 slot 0xF8 | virtual slot, introduced by script::ITalkRecept
};
