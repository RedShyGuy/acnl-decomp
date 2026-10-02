#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 8BsMenuBg @ 0x008CD484
// vtable 0x008F99AC (vptr 0x008F99B4), offset_to_top 0, 16 entries
// vtable 0x008F99F4 (vptr 0x008F99FC), offset_to_top -20, 3 entries
class BsMenuBg : public ::Base, public ::state::Mode<BsMenuBg>
{
public:
    BsMenuBg(); // ctor candidate(s) 0x00695EC8 (unverified)
    virtual ~BsMenuBg(); // 0x00695FF4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00695F88 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006959E4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00695CB0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00695BF4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00695988 slot 0x30 | slot vf_0x30 of oml::framework::Process
    void ChangeBg(long); // 0x006953F0 | libgarden [tier A]
};
