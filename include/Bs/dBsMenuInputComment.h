#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"

// RTTI 18BsMenuInputComment @ 0x008CC774
// vtable 0x008F4260 (vptr 0x008F4268), offset_to_top 0, 25 entries
class BsMenuInputComment : public ::MenuBase
{
public:
    BsMenuInputComment(); // ctor address unknown
    virtual ~BsMenuInputComment(); // 0x002DA588 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DA56C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DA1E8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DA4B0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DA354 slot 0x24 | slot vf_0x24 of oml::framework::Process
};
