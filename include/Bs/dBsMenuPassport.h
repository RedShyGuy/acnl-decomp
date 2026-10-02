#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 14BsMenuPassport @ 0x008CBBD8
// vtable 0x008EFF08 (vptr 0x008EFF10), offset_to_top 0, 25 entries
// vtable 0x008EFF74 (vptr 0x008EFF7C), offset_to_top -40, 3 entries
class BsMenuPassport : public ::MenuBase, public ::state::Mode<BsMenuPassport>
{
public:
    BsMenuPassport(); // ctor address unknown
    virtual ~BsMenuPassport(); // 0x0025D1F0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0025D150 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0025CDBC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0025CF98 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0025CEFC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0025CD7C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
