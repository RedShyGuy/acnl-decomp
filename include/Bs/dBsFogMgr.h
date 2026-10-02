#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 8BsFogMgr @ 0x008CD460
// vtable 0x008F98BC (vptr 0x008F98C4), offset_to_top 0, 16 entries
class BsFogMgr : public ::Base
{
public:
    BsFogMgr(); // ctor candidate(s) 0x00690624 (unverified)
    virtual ~BsFogMgr(); // 0x00690690 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00690654 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0069055C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006905E0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006905AC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00690528 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void Unk0(); // 0x00766F68 slot 0x3C | slot vf_0x3C of Base
};
