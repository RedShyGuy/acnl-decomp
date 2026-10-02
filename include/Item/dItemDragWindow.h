#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 14ItemDragWindow @ 0x008CBCF4
// vtable 0x008F03D4 (vptr 0x008F03DC), offset_to_top 0, 3 entries
class ItemDragWindow : public ::state::Mode<ItemDragWindow>
{
public:
    ItemDragWindow(); // ctor candidate(s) 0x00270208 (unverified)
    virtual ~ItemDragWindow(); // 0x002703F8 slot 0x00 | slot vf_0x00 of ItemDragWindow
    // 0x002703E8 slot 0x04 | slot vf_0x04 of ItemDragWindow (deleting dtor)
    virtual void vf_0x08(); // 0x0082B490 slot 0x08 | virtual slot, introduced by ItemDragWindow
};
