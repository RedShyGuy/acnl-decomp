#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"

// RTTI 16AcInsectFieldFly @ 0x008CC184
// vtable 0x00840518 (vptr 0x00840520), offset_to_top 0, 56 entries
// vtable 0x008F1FBC (vptr 0x008F1FC4), offset_to_top 0, 56 entries
// vtable 0x008F20CC (vptr 0x008F20D4), offset_to_top -468, 11 entries
// vtable 0x0084096C (vptr 0x00840974), offset_to_top -484, 11 entries
class AcInsectFieldFly : public ::AcInsectFieldBase
{
public:
    AcInsectFieldFly(); // ctor address unknown
    virtual ~AcInsectFieldFly(); // 0x002A8A24 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002A8A0C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x64(); // 0x002A8860 slot 0x64 | virtual slot, introduced by AcObjectBase
};
