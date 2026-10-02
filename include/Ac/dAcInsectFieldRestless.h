#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"

// RTTI 21AcInsectFieldRestless @ 0x008CCC24
// vtable 0x008F5B54 (vptr 0x008F5B5C), offset_to_top 0, 57 entries
// vtable 0x008F5C68 (vptr 0x008F5C70), offset_to_top -476, 11 entries
class AcInsectFieldRestless : public ::AcInsectFieldBase
{
public:
    AcInsectFieldRestless(); // ctor address unknown
    virtual ~AcInsectFieldRestless(); // 0x003246BC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x003246A4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0xC8(); // 0x00323C00 slot 0xC8 | virtual slot, introduced by AcInsectFieldBase
    virtual void vf_0xE0(); // 0x00323AB4 slot 0xE0 | virtual slot, introduced by AcInsectFieldRestless
};
