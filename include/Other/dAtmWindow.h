#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 9AtmWindow @ 0x008CD5C0
// vtable 0x008F9F8C (vptr 0x008F9F94), offset_to_top 0, 3 entries
class AtmWindow : public ::state::Mode<AtmWindow>
{
public:
    AtmWindow(); // ctor address unknown
    virtual void vf_0x00(); // 0x006CEF18 slot 0x00 | virtual slot, introduced by AtmWindow
    virtual void vf_0x04(); // 0x006CEE90 slot 0x04 | virtual slot, introduced by AtmWindow
    virtual void vf_0x08(); // 0x0082D4E8 slot 0x08 | virtual slot, introduced by AtmWindow
};
