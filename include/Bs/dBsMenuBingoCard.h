#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 15BsMenuBingoCard @ 0x008CBF28
// vtable 0x008F13B8 (vptr 0x008F13C0), offset_to_top 0, 25 entries
// vtable 0x008F1424 (vptr 0x008F142C), offset_to_top -40, 3 entries
class BsMenuBingoCard : public ::MenuBase, public ::state::Mode<BsMenuBingoCard>
{
public:
    BsMenuBingoCard(); // ctor address unknown
    virtual ~BsMenuBingoCard(); // 0x00288208 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002881F8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00287AF4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00288050 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00287F20 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00287A30 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
