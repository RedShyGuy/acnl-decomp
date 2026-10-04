#pragma once

#include "decomp.h"
#include "pead/peadThread.h"

namespace pead {
// RTTI N4pead10MainThreadE @ 0x008D1144
// vtable 0x009049B0 (vptr 0x009049B8), offset_to_top 0, 17 entries
class MainThread : public ::pead::Thread
{
public:
    MainThread(); // ctor candidate(s) 0x0053DD18 (unverified)
    virtual ~MainThread(); // 0x00538014 slot 0x00 | nintendogs:bytes
    // 0x00537FEC slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x00537FE4 slot 0x08 | virtual slot, introduced by pead::Thread
    virtual void quit(bool isJam); // 0x00537FDC slot 0x18 (name is ours) | virtual slot, introduced by pead::Thread
    virtual void waitDone(); // 0x00537FE8 slot 0x1C (name is ours) | virtual slot, introduced by pead::Thread
    virtual void vf_0x20(); // 0x00537FD8 slot 0x20 | virtual slot, introduced by pead::Thread
    virtual void setPriority(int); // 0x00537FD4 slot 0x28 | slot vf_0x28 of pead::Thread
    virtual void vf_0x3C(); // 0x00537FE0 slot 0x3C | virtual slot, introduced by pead::Thread
};
} // namespace pead
