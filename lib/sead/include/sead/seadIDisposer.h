#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead9IDisposerE @ 0x008D2390
// vtable 0x00907110 (vptr 0x00907118), offset_to_top 0, 2 entries
class IDisposer
{
public:
    IDisposer(); // ctor candidate(s) 0x0013378C, 0x00133834 (unverified)
    virtual ~IDisposer(); // 0x0013F6E0 slot 0x00 | nintendogs:callseq-callee
    // 0x005629A8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
} // namespace sead
