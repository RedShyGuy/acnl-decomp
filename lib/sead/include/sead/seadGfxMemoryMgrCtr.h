#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead15GfxMemoryMgrCtrE @ 0x008D188C
// The allocator of the graphics memory of nngx (names are ours).
class GfxMemoryMgrCtr
{
public:
    GfxMemoryMgrCtr() : field_0x04(false) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098349C (guard 0x00983498)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    // the memory of nngx: area 0x10000 main memory, 0x20000 / 0x30000 VRAM A / B; kind 0x100.. what
    // it is for (texture, vertex buffer, ...); name of the object (names are ours)
    virtual void* allocate(u32 area, u32 kind, u32 name, s32 size) = 0; // slot 0x08
    virtual void deallocate(u32 area, u32 kind, u32 name, void* addr) = 0; // slot 0x0C
    virtual ~GfxMemoryMgrCtr() {} // slot 0x10

    bool field_0x04; // 0x04
};
} // namespace sead
