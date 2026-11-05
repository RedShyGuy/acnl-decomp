#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace demo {
// the process of the orders
// RTTI N4demo4ProcE @ 0x008D10C0
// vtable 0x009047F0 (vptr 0x009047F8), offset_to_top 0, 16 entries
class Proc : public ::Base
{
public:
    Proc(); // 0x0052DBD0 (name after the RTTI)
    virtual ~Proc(); // 0x0052DBF8 slot 0x00 (name after the RTTI)
    // 0x0052DBE8 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x0052DB94 slot 0x0C
    virtual oml::framework::Result Finalize(); // 0x0052DBC8 slot 0x18
    virtual oml::framework::Result Calc(); // 0x0052DBA8 slot 0x24
    virtual oml::framework::Result Draw(); // 0x0052DB8C slot 0x30
};
} // namespace demo
