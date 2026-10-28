#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead18LogicalFrameBufferE @ 0x008D1C44
// vtable 0x00906124 (vptr 0x0090612C), offset_to_top 0, 4 entries
class LogicalFrameBuffer
{
public:
    LogicalFrameBuffer(); // ctor address unknown
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
    virtual void vf_0x08(); // 0x00549FD0 slot 0x08 | virtual slot, introduced by sead::LogicalFrameBuffer
    virtual void vf_0x0C(); // 0x00549FCC slot 0x0C | virtual slot, introduced by sead::LogicalFrameBuffer
};
} // namespace sead
