#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadLogicalFrameBuffer.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// 0x00976340
const FrameBuffer* FrameBuffer::s_pBoundFrameBuffer = nullptr;

// 0x0053FDF4 slot 0x08
// 0x0053FDCC (deleting dtor)
sead::FrameBuffer::~FrameBuffer()
{
    if (this == s_pBoundFrameBuffer) {
        s_pBoundFrameBuffer = nullptr;
    }
}

// 0x0011F334 (name is ours)
void sead::FrameBuffer::bind() const
{
    bindImpl_();
    s_pBoundFrameBuffer = this;
}

} // namespace sead

// the type information of the classes below LogicalFrameBuffer
// 0x008291B0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::LogicalFrameBuffer>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
