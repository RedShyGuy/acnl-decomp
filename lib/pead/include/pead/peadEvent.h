#pragma once

#include "decomp.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead5EventE @ 0x008D1274
// vtable 0x00904CC8 (vptr 0x00904CD0), offset_to_top 0, 2 entries
class Event : public ::pead::IDisposer
{
public:
    Event(); // ctor candidate(s) 0x0053B98C (unverified)
    virtual ~Event(); // 0x0053BA28 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053B9E4 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
};
} // namespace pead
