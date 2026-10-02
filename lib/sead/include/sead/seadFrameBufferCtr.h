#pragma once

#include "decomp.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// RTTI N4sead14FrameBufferCtrE @ 0x008D1704
// vtable 0x00905830 (vptr 0x00905838), offset_to_top 0, 8 entries
class FrameBufferCtr : public ::sead::FrameBuffer
{
public:
    FrameBufferCtr(); // ctor candidate(s) 0x0011E900 (unverified)
    virtual void vf_0x08(); // 0x00544FF0 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x0C(); // 0x00544FD4 slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x14(); // 0x00726B48 slot 0x14 | virtual slot, introduced by sead::FrameBuffer
    virtual void bindImpl_() const; // 0x0074C014 slot 0x1C | nintendogs:callseq
};
} // namespace sead
