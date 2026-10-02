#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 10BsHandItem @ 0x008CAF6C
// vtable 0x008EBD2C (vptr 0x008EBD34), offset_to_top 0, 16 entries
class BsHandItem : public ::Base
{
public:
    BsHandItem(); // ctor candidate(s) 0x00190198 (unverified)
    virtual ~BsHandItem(); // 0x00190204 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001901C8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001900C4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00190138 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001900D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001900BC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
