#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace font {
// RTTI N4font4BaseE @ 0x008D10F0
// vtable 0x00904874 (vptr 0x0090487C), offset_to_top 0, 16 entries
class Base : public ::Base
{
public:
    Base(); // ctor candidate(s) 0x0052F120 (unverified)
    virtual ~Base(); // 0x0052F148 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0052F138 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0052F0C4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0052F104 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0052F0E8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0052F0A8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
} // namespace font
