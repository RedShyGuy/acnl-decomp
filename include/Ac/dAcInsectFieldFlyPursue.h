#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldFly.h"

// RTTI 22AcInsectFieldFlyPursue @ 0x008CCD58
// vtable 0x008F65E4 (vptr 0x008F65EC), offset_to_top 0, 56 entries
// vtable 0x008F66F4 (vptr 0x008F66FC), offset_to_top -484, 11 entries
class AcInsectFieldFlyPursue : public ::AcInsectFieldFly
{
public:
    AcInsectFieldFlyPursue(); // ctor address unknown
    virtual ~AcInsectFieldFlyPursue(); // 0x0032DC10 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0032DBF8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0xC8(); // 0x0032D6A8 slot 0xC8 | virtual slot, introduced by AcInsectFieldBase
    virtual void vf_0xCC(); // 0x006C6A3C slot 0xCC | virtual slot, introduced by AcInsectFieldBase
};
