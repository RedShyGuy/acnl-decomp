#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 6BsRoot @ 0x008CD2E4
// vtable 0x008F8C28 (vptr 0x008F8C30), offset_to_top 0, 16 entries
class BsRoot : public ::Base
{
public:
    BsRoot(); // ctor candidate(s) 0x007F2F40 (unverified)
    virtual ~BsRoot(); // 0x005BFFE4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005BFFD4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x005BFF1C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x005BFFBC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x005BFFA4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x005BFF14 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
