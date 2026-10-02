#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 12BsHandCursor @ 0x008CB52C
// vtable 0x008EE0A4 (vptr 0x008EE0AC), offset_to_top 0, 16 entries
// vtable 0x008EE0EC (vptr 0x008EE0F4), offset_to_top -20, 3 entries
class BsHandCursor : public ::Base, public ::state::Mode<BsHandCursor>
{
public:
    BsHandCursor(); // ctor candidate(s) 0x007ED4BC (unverified)
    virtual ~BsHandCursor(); // 0x001FACA0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FAC3C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FA5D8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FAC1C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FA9D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FA5AC slot 0x30 | slot vf_0x30 of oml::framework::Process
    void Disable(); // 0x001FA910 | libgarden [tier A]
};
