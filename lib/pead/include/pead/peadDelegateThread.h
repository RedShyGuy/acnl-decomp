#pragma once

#include "decomp.h"
#include "pead/peadThread.h"

namespace pead {
// RTTI N4pead14DelegateThreadE @ 0x008D1178
// vtable 0x00904A0C (vptr 0x00904A14), offset_to_top 0, 17 entries
class DelegateThread : public ::pead::Thread
{
public:
    DelegateThread(); // ctor candidate(s) 0x00538788 (unverified)
    virtual ~DelegateThread(); // 0x0053C570 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x005387C4 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void vf_0x3C(); // 0x0053876C slot 0x3C | virtual slot, introduced by pead::Thread
};
} // namespace pead
