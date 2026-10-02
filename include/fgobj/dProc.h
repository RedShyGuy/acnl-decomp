#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace fgobj {
// RTTI N5fgobj4ProcE @ 0x008D2A24
// vtable 0x00908DE4 (vptr 0x00908DEC), offset_to_top 0, 16 entries
class Proc : public ::Base
{
public:
    Proc(); // ctor address unknown
    virtual ~Proc(); // 0x0059F64C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0059F57C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0059E03C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0059EF40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0059EAA4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0059DFC4 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
} // namespace fgobj
