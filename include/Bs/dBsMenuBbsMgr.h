#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 12BsMenuBbsMgr @ 0x008CB54C
// vtable 0x008EE100 (vptr 0x008EE108), offset_to_top 0, 25 entries
// vtable 0x008EE16C (vptr 0x008EE174), offset_to_top -40, 3 entries
class BsMenuBbsMgr : public ::MenuBase, public ::state::Mode<BsMenuBbsMgr>
{
public:
    BsMenuBbsMgr(); // ctor address unknown
    virtual ~BsMenuBbsMgr(); // 0x001FB32C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FB308 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FB144 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FB2B8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FB228 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FB13C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
