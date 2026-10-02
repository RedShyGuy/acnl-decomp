#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 20BsMenuModelHomeBoard @ 0x008CCACC
// vtable 0x008F5474 (vptr 0x008F547C), offset_to_top 0, 25 entries
// vtable 0x008F54E0 (vptr 0x008F54E8), offset_to_top -40, 3 entries
class BsMenuModelHomeBoard : public ::MenuBase, public ::state::Mode<BsMenuModelHomeBoard>
{
public:
    BsMenuModelHomeBoard(); // ctor address unknown
    virtual ~BsMenuModelHomeBoard(); // 0x00319010 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00319000 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0031749C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00318AA8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x003183BC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00317458 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
