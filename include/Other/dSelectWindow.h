#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 12SelectWindow @ 0x008CB800
// vtable 0x008EE808 (vptr 0x008EE810), offset_to_top 0, 3 entries
class SelectWindow : public ::state::Mode<SelectWindow>
{
public:
    SelectWindow(); // ctor address unknown
    virtual void vf_0x00(); // 0x0020A454 slot 0x00 | virtual slot, introduced by SelectWindow
    virtual void vf_0x04(); // 0x0020A3A8 slot 0x04 | virtual slot, introduced by SelectWindow
    virtual void vf_0x08(); // 0x0082AD88 slot 0x08 | virtual slot, introduced by SelectWindow
};
