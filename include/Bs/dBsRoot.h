#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// The root process of the game (class name from the RTTI; the other names are ours): it makes BsDemoProc and BsMenuMgr
// as its children; BsScene makes it before the first scene.
// RTTI 6BsRoot @ 0x008CD2E4
// vtable 0x008F8C28 (vptr 0x008F8C30), offset_to_top 0, 16 entries
class BsRoot : public ::Base
{
public:
    // (inline in its factory Process::Create<BsRoot> 0x007F2F40)
    BsRoot() {}
    virtual ~BsRoot(); // 0x005BFFE4 slot 0x00
    // 0x005BFFD4 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x005BFF1C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x005BFFBC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x005BFFA4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x005BFF14 slot 0x30 | slot vf_0x30 of oml::framework::Process

    // the instance (NULL: none; name is ours)
    static BsRoot* s_pInstance;
};
