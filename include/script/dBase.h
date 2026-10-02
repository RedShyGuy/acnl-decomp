#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace script {
// RTTI N6script4BaseE @ 0x008D3A58
// vtable 0x0090A524 (vptr 0x0090A52C), offset_to_top 0, 16 entries
class Base : public ::Base
{
public:
    Base(); // ctor candidate(s) 0x005EA8FC (unverified)
    virtual ~Base(); // 0x005EA924 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005EA914 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x005EA890 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x005EA8DC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x005EA8C0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x005EA874 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
} // namespace script
