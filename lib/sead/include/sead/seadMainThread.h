#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

namespace sead {
// RTTI N4sead10MainThreadE @ 0x008D13C0
// vtable 0x00904EC4 (vptr 0x00904ECC), offset_to_top 0, 18 entries
// vtable 0x00904F14 (vptr 0x00904F1C), offset_to_top -24, 1 entries
class MainThread : public ::sead::Thread
{
public:
    MainThread(); // ctor candidate(s) 0x001393C0 (unverified)
    virtual ~MainThread(); // 0x0053F224 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0053F1F8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0053F1F0 slot 0x08 | virtual slot, introduced by sead::Thread
    virtual void vf_0x1C(); // 0x0053F1E8 slot 0x1C | virtual slot, introduced by sead::Thread
    virtual void vf_0x20(); // 0x0053F1F4 slot 0x20 | virtual slot, introduced by sead::Thread
    virtual void vf_0x24(); // 0x0053F1E4 slot 0x24 | virtual slot, introduced by sead::Thread
    virtual void vf_0x2C(); // 0x0053F1E0 slot 0x2C | virtual slot, introduced by sead::Thread
    virtual void vf_0x40(); // 0x0053F1EC slot 0x40 | virtual slot, introduced by sead::Thread
};
} // namespace sead
