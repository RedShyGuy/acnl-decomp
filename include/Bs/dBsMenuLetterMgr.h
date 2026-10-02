#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 15BsMenuLetterMgr @ 0x008CBF48
// vtable 0x008F1438 (vptr 0x008F1440), offset_to_top 0, 25 entries
// vtable 0x008F14A4 (vptr 0x008F14AC), offset_to_top -40, 3 entries
class BsMenuLetterMgr : public ::MenuBase, public ::state::Mode<BsMenuLetterMgr>
{
public:
    BsMenuLetterMgr(); // ctor address unknown
    virtual ~BsMenuLetterMgr(); // 0x002888D4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002888A0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00288634 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00288838 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00288738 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0028862C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
