#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 9BsBgSeMgr @ 0x008CD6BC
// vtable 0x008FA2AC (vptr 0x008FA2B4), offset_to_top 0, 16 entries
class BsBgSeMgr : public ::Base
{
public:
    BsBgSeMgr(); // ctor candidate(s) 0x006D186C (unverified)
    virtual ~BsBgSeMgr(); // 0x006D18FC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006D18C8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x006D13B0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x006D177C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x006D16D8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006D13A8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
