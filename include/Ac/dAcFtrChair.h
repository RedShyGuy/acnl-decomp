#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// RTTI 10AcFtrChair @ 0x008CAF28
// vtable 0x008EB8B0 (vptr 0x008EB8B8), offset_to_top 0, 67 entries
// vtable 0x008EB9C4 (vptr 0x008EB9CC), offset_to_top -104, 4 entries
// vtable 0x008EB9DC (vptr 0x008EB9E4), offset_to_top -176, 72 entries
// vtable 0x008EBB04 (vptr 0x008EBB0C), offset_to_top -300, 14 entries
class AcFtrChair : public ::AcFtr
{
public:
    AcFtrChair(); // ctor candidate(s) 0x007EBD8C (unverified)
    virtual ~AcFtrChair(); // 0x0018D160 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0018D150 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0xB0(); // 0x0018D0C0 slot 0xB0 | virtual slot, introduced by AcFtr
    virtual void vf_0xB8(); // 0x0018CF74 slot 0xB8 | virtual slot, introduced by AcFtr
    virtual void vf_0xF8(); // 0x0018D004 slot 0xF8 | virtual slot, introduced by AcFtr
};
