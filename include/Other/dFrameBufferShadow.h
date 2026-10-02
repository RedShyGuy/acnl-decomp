#pragma once

#include "decomp.h"
#include "sead/seadFrameBufferCtr.h"

// RTTI 17FrameBufferShadow @ 0x008CC648
// vtable 0x008F3A70 (vptr 0x008F3A78), offset_to_top 0, 8 entries
class FrameBufferShadow : public ::sead::FrameBufferCtr
{
public:
    FrameBufferShadow(); // ctor candidate(s) 0x002CDDAC (unverified)
    virtual void vf_0x08(); // 0x002CDF34 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x0C(); // 0x002CDF18 slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void bindImpl_() const; // 0x0071FC04 slot 0x1C | slot vf_0x1C of sead::FrameBuffer
};
