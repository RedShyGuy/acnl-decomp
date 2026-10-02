#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"

// RTTI 21AcInsectFieldStraight @ 0x008CCC30
// vtable 0x008F5CC0 (vptr 0x008F5CC8), offset_to_top 0, 57 entries
// vtable 0x008F5DD4 (vptr 0x008F5DDC), offset_to_top -496, 11 entries
class AcInsectFieldStraight : public ::AcInsectFieldBase
{
public:
    AcInsectFieldStraight(); // ctor address unknown
    virtual ~AcInsectFieldStraight(); // 0x00324E94 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00324E7C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0xE0(); // 0x0032496C slot 0xE0 | virtual slot, introduced by AcInsectFieldStraight
};
