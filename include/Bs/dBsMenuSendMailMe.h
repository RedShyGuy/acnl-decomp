#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 16BsMenuSendMailMe @ 0x008CC274
// vtable 0x008F279C (vptr 0x008F27A4), offset_to_top 0, 25 entries
// vtable 0x008F2808 (vptr 0x008F2810), offset_to_top -40, 3 entries
class BsMenuSendMailMe : public ::MenuBase, public ::state::Mode<BsMenuSendMailMe>
{
public:
    BsMenuSendMailMe(); // ctor address unknown
    virtual ~BsMenuSendMailMe(); // 0x002B4D74 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B4D64 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002B4600 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002B4AF4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002B48F4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002B45B8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
