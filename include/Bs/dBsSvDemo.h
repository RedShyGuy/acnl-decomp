#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 8BsSvDemo @ 0x008CD4B0
// vtable 0x008F9A50 (vptr 0x008F9A58), offset_to_top 0, 18 entries
// vtable 0x008F9AA0 (vptr 0x008F9AA8), offset_to_top -20, 63 entries
// vtable 0x008F9BA4 (vptr 0x008F9BAC), offset_to_top -144, 3 entries
class BsSvDemo : public ::Base, public ::script::ITalkRecept, public ::state::Mode<BsSvDemo>
{
public:
    BsSvDemo(); // ctor candidate(s) 0x007F34D0 (unverified)
    virtual ~BsSvDemo(); // 0x006A36EC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006A36D0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006A358C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006A36B0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006A3620 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006A3584 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x006A2BCC slot 0x40 | virtual slot, introduced by BsSvDemo
    virtual void vf_0x44(); // 0x006A2B90 slot 0x44 | virtual slot, introduced by BsSvDemo
};
