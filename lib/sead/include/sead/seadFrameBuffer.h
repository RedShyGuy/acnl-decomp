#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadColor4f.h"
#include "sead/seadLogicalFrameBuffer.h"

namespace sead {
// RTTI N4sead11FrameBufferE @ 0x008D13F8
// vtable 0x00904F84 (vptr 0x00904F8C), offset_to_top 0, 8 entries
class FrameBuffer : public ::sead::LogicalFrameBuffer
{
public:
    FrameBuffer() {}
    FrameBuffer(const Vector2<f32>& virtualSize, f32 x, f32 y, f32 width, f32 height)
        : LogicalFrameBuffer(virtualSize, x, y, width, height)
    {
    }
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
    // forgets the frame buffer if it is the bound one
    virtual ~FrameBuffer(); // 0x0053FDF4 slot 0x08
    // 0x0053FDCC slot 0x0C (deleting dtor)
    virtual void vf_0x10() {} // 0x0074A63C slot 0x10 | virtual slot, introduced by sead::FrameBuffer
    // clears the buffers of flags (1 color, 2 depth, 4 stencil) to the values
    virtual void clearImpl_(u32 flags, const Color4f& color, f32 depth, u8 stencil) = 0; // slot 0x14
    virtual void vf_0x18() {} // 0x0074A6F4 slot 0x18 | virtual slot, introduced by sead::FrameBuffer
    virtual void bindImpl_() const = 0; // slot 0x1C

    // binds the frame buffer (name is ours)
    void bind() const; // 0x0011F334

    // the bound frame buffer (name is ours)
    static const FrameBuffer* s_pBoundFrameBuffer;
};
} // namespace sead
