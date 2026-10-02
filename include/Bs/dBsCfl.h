#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 5BsCfl @ 0x008CD2A4
// vtable 0x008F89FC (vptr 0x008F8A04), offset_to_top 0, 16 entries
class BsCfl : public ::Base
{
public:
    BsCfl(); // ctor candidate(s) 0x0057C8D4 (unverified)
    virtual ~BsCfl(); // 0x0057C8FC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0057C8EC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0057C8BC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0057C8CC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0057C8C4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0057C8AC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
