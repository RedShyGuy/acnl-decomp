#pragma once

#include "decomp.h"
#include "sead/seadThread.h"

// RTTI 10ProcThread @ 0x008CB128
// vtable 0x008EC2C4 (vptr 0x008EC2CC), offset_to_top 0, 18 entries
// vtable 0x008EC314 (vptr 0x008EC31C), offset_to_top -24, 1 entries
class ProcThread : public ::sead::Thread
{
public:
    ProcThread(); // ctor address unknown
    virtual ~ProcThread(); // 0x0055D99C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x001AF800 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x001AF76C slot 0x40 | virtual slot, introduced by sead::Thread
};
