#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 10BsMenuChat @ 0x008CAFA4
// vtable 0x008EBE3C (vptr 0x008EBE44), offset_to_top 0, 25 entries
// vtable 0x008EBEA8 (vptr 0x008EBEB0), offset_to_top -40, 3 entries
class BsMenuChat : public ::MenuBase, public ::state::Mode<BsMenuChat>
{
public:
    BsMenuChat(); // ctor address unknown
    virtual ~BsMenuChat(); // 0x00193DF0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00193DCC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00193AD0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00193CCC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00193C3C slot 0x24 | slot vf_0x24 of oml::framework::Process
};
