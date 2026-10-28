#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadLogicalFrameBuffer.h"

namespace sead {
// RTTI N4sead11FrameBufferE @ 0x008D13F8
// vtable 0x00904F84 (vptr 0x00904F8C), offset_to_top 0, 8 entries
class FrameBuffer : public ::sead::LogicalFrameBuffer
{
public:
    FrameBuffer(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A640 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return LogicalFrameBuffer::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A5F0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833D4 (guard 0x009833D0)
        static const RuntimeTypeInfo::Derive<LogicalFrameBuffer> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0053FDF4 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x0C(); // 0x0053FDCC slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x10(); // 0x0074A63C slot 0x10 | virtual slot, introduced by sead::FrameBuffer
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0074A6F4 slot 0x18 | virtual slot, introduced by sead::FrameBuffer
    virtual void bindImpl_() const; // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
