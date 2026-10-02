#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"

// RTTI 15BsReturnSaveMgr @ 0x008CBFB4
// vtable 0x008F1600 (vptr 0x008F1608), offset_to_top 0, 16 entries
// vtable 0x008F1648 (vptr 0x008F1650), offset_to_top -20, 63 entries
class BsReturnSaveMgr : public ::Base, public ::script::ITalkRecept
{
public:
    BsReturnSaveMgr(); // ctor candidate(s) 0x0028E850 (unverified)
    virtual ~BsReturnSaveMgr(); // 0x0028E954 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028E904 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0028E6E8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0028E7DC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0028E798 slot 0x24 | slot vf_0x24 of oml::framework::Process
};
