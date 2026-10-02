#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 10BsMenuEdit @ 0x008CAFC4
// vtable 0x008EBEBC (vptr 0x008EBEC4), offset_to_top 0, 25 entries
// vtable 0x008EBF28 (vptr 0x008EBF30), offset_to_top -40, 3 entries
class BsMenuEdit : public ::MenuBase, public ::state::Mode<BsMenuEdit>
{
public:
    BsMenuEdit(); // ctor address unknown
    virtual ~BsMenuEdit(); // 0x0019931C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0019930C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00196E98 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00197A40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00197654 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00196C0C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
