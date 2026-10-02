#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 20ItemSelectNameWindow @ 0x008CCB88
// vtable 0x008F5848 (vptr 0x008F5850), offset_to_top 0, 3 entries
class ItemSelectNameWindow : public ::state::Mode<ItemSelectNameWindow>
{
public:
    ItemSelectNameWindow(); // ctor candidate(s) 0x0031F2A8 (unverified)
    virtual void vf_0x00(); // 0x0031F37C slot 0x00 | virtual slot, introduced by ItemSelectNameWindow
    virtual void vf_0x04(); // 0x0031F33C slot 0x04 | virtual slot, introduced by ItemSelectNameWindow
    virtual void vf_0x08(); // 0x0082C750 slot 0x08 | virtual slot, introduced by ItemSelectNameWindow
};
