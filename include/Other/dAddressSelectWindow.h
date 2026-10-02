#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 19AddressSelectWindow @ 0x008CC934
// vtable 0x008F4D68 (vptr 0x008F4D70), offset_to_top 0, 3 entries
class AddressSelectWindow : public ::state::Mode<AddressSelectWindow>
{
public:
    AddressSelectWindow(); // ctor candidate(s) 0x002EEB74 (unverified)
    virtual void vf_0x00(); // 0x002EED78 slot 0x00 | virtual slot, introduced by AddressSelectWindow
    virtual void vf_0x04(); // 0x002EECC4 slot 0x04 | virtual slot, introduced by AddressSelectWindow
    virtual void vf_0x08(); // 0x0082C318 slot 0x08 | virtual slot, introduced by AddressSelectWindow
};
