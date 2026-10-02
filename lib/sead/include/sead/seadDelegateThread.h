#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace sead {
// RTTI N4sead14DelegateThreadE @ 0x008D16E0
// vtable 0x00905748 (vptr 0x00905750), offset_to_top 0, 18 entries
// vtable 0x00905798 (vptr 0x009057A0), offset_to_top -24, 1 entries
class DelegateThread : public ::sead::Thread
{
public:
    DelegateThread(); // ctor candidate(s) 0x00544838 (unverified)
    virtual ~DelegateThread(); // 0x0054488C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054487C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x0054481C slot 0x40 | virtual slot, introduced by sead::Thread
};
} // namespace sead
