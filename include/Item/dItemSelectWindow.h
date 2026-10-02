#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 16ItemSelectWindow @ 0x008CC338
// vtable 0x008F2A74 (vptr 0x008F2A7C), offset_to_top 0, 3 entries
class ItemSelectWindow : public ::state::Mode<ItemSelectWindow>
{
public:
    ItemSelectWindow(); // ctor candidate(s) 0x002BB0CC (unverified)
    virtual void vf_0x00(); // 0x002BB248 slot 0x00 | virtual slot, introduced by ItemSelectWindow
    virtual void vf_0x04(); // 0x002BB1D8 slot 0x04 | virtual slot, introduced by ItemSelectWindow
    virtual void vf_0x08(); // 0x0082BD00 slot 0x08 | virtual slot, introduced by ItemSelectWindow
};
