#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace script {
// The process of the scripts: it creates script::Mgr and runs its steps (names after the RTTI and libgarden).
// RTTI N6script4BaseE @ 0x008D3A58
// vtable 0x0090A524 (vptr 0x0090A52C), offset_to_top 0, 16 entries
class Base : public ::Base
{
public:
    Base(); // 0x005EA8FC (name after the RTTI)
    virtual ~Base(); // 0x005EA924 slot 0x00
    // 0x005EA914 slot 0x04 (deleting dtor)
    // the manager is created and initialized (again until its windows are ready)
    virtual oml::framework::Result Initialize(); // 0x005EA890 slot 0x0C
    // the manager is finalized and deleted
    virtual oml::framework::Result Finalize(); // 0x005EA8DC slot 0x18
    virtual oml::framework::Result Calc(); // 0x005EA8C0 slot 0x24
    virtual oml::framework::Result Draw(); // 0x005EA874 slot 0x30
};
} // namespace script
