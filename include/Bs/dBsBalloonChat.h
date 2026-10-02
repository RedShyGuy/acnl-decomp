#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 13BsBalloonChat @ 0x008CB8B4
// vtable 0x008EECB4 (vptr 0x008EECBC), offset_to_top 0, 16 entries
class BsBalloonChat : public ::Base
{
public:
    BsBalloonChat(); // ctor candidate(s) 0x0021837C (unverified)
    virtual ~BsBalloonChat(); // 0x00218578 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002184AC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00216A58 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00217C84 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002170D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00216830 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
