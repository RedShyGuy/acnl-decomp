#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 12BsNameWindow @ 0x008CB5AC
// vtable 0x008EE280 (vptr 0x008EE288), offset_to_top 0, 16 entries
class BsNameWindow : public ::Base
{
public:
    BsNameWindow(); // ctor candidate(s) 0x001FEC4C (unverified)
    virtual ~BsNameWindow(); // 0x001FECB0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FEC80 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FE4E8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FE574 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FE544 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FE4E0 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
