#include "sead/seadFrameBuffer.h"
#include "sead/seadFrameBufferCtr.h"

namespace sead {
// ctor candidate(s) 0x0011E900 (unverified)
sead::FrameBufferCtr::FrameBufferCtr()
{
}

// 0x00544FF0 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBufferCtr::vf_0x08()
{
}

// 0x00544FD4 slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBufferCtr::vf_0x0C()
{
}

// 0x00726B48 slot 0x14 | virtual slot, introduced by sead::FrameBuffer
void sead::FrameBufferCtr::vf_0x14()
{
}

// 0x0074C014 slot 0x1C | nintendogs:callseq
void sead::FrameBufferCtr::bindImpl_() const
{
}

} // namespace sead
