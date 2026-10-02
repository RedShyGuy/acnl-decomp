#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 14BsMenuReaction @ 0x008CBBF8
// vtable 0x008EFF88 (vptr 0x008EFF90), offset_to_top 0, 25 entries
// vtable 0x008EFFF4 (vptr 0x008EFFFC), offset_to_top -40, 3 entries
class BsMenuReaction : public ::MenuBase, public ::state::Mode<BsMenuReaction>
{
public:
    BsMenuReaction(); // ctor address unknown
    virtual ~BsMenuReaction(); // 0x0025F94C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0025F93C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0025E6E0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0025F59C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0025F1E8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0025E674 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x0025D5A4 slot 0x44 | slot vf_0x44 of MenuBase
};
