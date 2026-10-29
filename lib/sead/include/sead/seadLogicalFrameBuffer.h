#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadBoundBox2.h"
#include "sead/seadVector2.h"

namespace sead {
// RTTI N4sead18LogicalFrameBufferE @ 0x008D1C44
// vtable 0x00906124 (vptr 0x0090612C), offset_to_top 0, 4 entries
// A frame buffer as the viewports see it.
class LogicalFrameBuffer
{
public:
    LogicalFrameBuffer() {}
    // the size the viewports see and the area of the buffer it is drawn to
    LogicalFrameBuffer(const Vector2<f32>& virtualSize, f32 x, f32 y, f32 width, f32 height) : mVirtualSize(virtualSize)
    {
        mPhysicalArea.set(x, y, x + width, y + height);
    }
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074CC78 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074CC2C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834DC (guard 0x009834D8)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~LogicalFrameBuffer(); // 0x00549FD0 slot 0x08
    // 0x00549FCC slot 0x0C (deleting dtor)

    // (names are ours)
    Vector2<f32> mVirtualSize;      // 0x04
    BoundBox2<f32> mPhysicalArea;   // 0x0C
};
ASSERT_SIZE(LogicalFrameBuffer, 0x1C);
} // namespace sead
