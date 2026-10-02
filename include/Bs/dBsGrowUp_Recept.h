#pragma once

#include "decomp.h"
#include "Bs/dBsGrowUp.h"
#include "script/dITalkRecept.h"

// RTTI N8BsGrowUp6ReceptE @ 0x008D3FD0
// vtable 0x0090BF68 (vptr 0x0090BF70), offset_to_top 0, 63 entries
class BsGrowUp::Recept : public ::script::ITalkRecept
{
public:
    Recept(); // ctor address unknown
    virtual ~Recept(); // 0x00117595 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00117589 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x98(); // 0x0011753D slot 0x98 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xF8(); // 0x0011B527 slot 0xF8 | virtual slot, introduced by script::ITalkRecept
};
