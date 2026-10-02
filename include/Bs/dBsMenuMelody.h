#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 12BsMenuMelody @ 0x008CB58C
// vtable 0x008EE200 (vptr 0x008EE208), offset_to_top 0, 25 entries
// vtable 0x008EE26C (vptr 0x008EE274), offset_to_top -40, 3 entries
class BsMenuMelody : public ::MenuBase, public ::state::Mode<BsMenuMelody>
{
public:
    BsMenuMelody(); // ctor address unknown
    virtual ~BsMenuMelody(); // 0x001FE240 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001FE230 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001FD6FC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001FDE88 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001FDD90 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001FD6D0 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
