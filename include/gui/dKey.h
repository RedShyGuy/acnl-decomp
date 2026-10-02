#pragma once

#include "decomp.h"

namespace gui {
// RTTI N3gui3KeyE @ 0x008D0E90
// vtable 0x00904050 (vptr 0x00904058), offset_to_top 0, 6 entries
class Key
{
public:
    Key(); // ctor candidate(s) 0x004FE548 (unverified)
    virtual ~Key(); // 0x004FE65C slot 0x00 | slot vf_0x00 of gui::Key
    // 0x004FE648 slot 0x04 | slot vf_0x04 of gui::Key (deleting dtor)
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x004FE544 slot 0x14 | virtual slot, introduced by gui::Key
};
} // namespace gui
