#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadGfxMemoryMgrCtr.h"

namespace sead {
class Heap;

// RTTI N4sead22DefaultGfxMemoryMgrCtrE @ 0x008D1F14
// vtable 0x009066D8 (vptr 0x009066E0), offset_to_top 0, 6 entries
class DefaultGfxMemoryMgrCtr : public ::sead::GfxMemoryMgrCtr
{
public:
    explicit DefaultGfxMemoryMgrCtr(Heap* heap); // 0x0011EBAC
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074DD94 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return GfxMemoryMgrCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074DD48 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098354C (guard 0x00983548)
        static const RuntimeTypeInfo::Derive<GfxMemoryMgrCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    // main memory from the heap, VRAM from the start of the area up (the VRAM is never given back)
    virtual void* allocate(u32 area, u32 kind, u32 name, s32 size); // 0x0054BAC4 slot 0x08
    virtual void deallocate(u32 area, u32 kind, u32 name, void* addr); // 0x0054BAB0 slot 0x0C
    virtual ~DefaultGfxMemoryMgrCtr(); // 0x0054BC5C slot 0x10
    // 0x0054BC58 slot 0x14 (deleting dtor)

    Heap* mHeap;            // 0x08
    uptr mVramACurrent;     // 0x0C
    uptr mVramBCurrent;     // 0x10
    uptr mVramAEnd;         // 0x14
    uptr mVramBEnd;         // 0x18
    s32 mVramSizeOfKind102; // 0x1C
    s32 mVramSizeOfKind101; // 0x20
};
ASSERT_SIZE(DefaultGfxMemoryMgrCtr, 0x24);
} // namespace sead
