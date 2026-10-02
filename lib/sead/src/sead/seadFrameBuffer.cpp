#include "sead/seadLogicalFrameBuffer.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// ctor address unknown
sead::FrameBuffer::FrameBuffer()
{
}

// 0x0074A640 slot 0x00 | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBuffer::vf_0x00()
{
}

// 0x0074A5F0 slot 0x04 | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBuffer::vf_0x04()
{
}

// 0x0053FDF4 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBuffer::vf_0x08()
{
}

// 0x0053FDCC slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
void sead::FrameBuffer::vf_0x0C()
{
}

// 0x0074A63C slot 0x10 | virtual slot, introduced by sead::FrameBuffer
void sead::FrameBuffer::vf_0x10()
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void sead::FrameBuffer::vf_0x14()
{
}

// 0x0074A6F4 slot 0x18 | virtual slot, introduced by sead::FrameBuffer
void sead::FrameBuffer::vf_0x18()
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void sead::FrameBuffer::bindImpl_() const
{
}

} // namespace sead
