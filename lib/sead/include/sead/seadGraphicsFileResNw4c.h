#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResource.h"

namespace sead {
// RTTI N4sead19GraphicsFileResNw4cE @ 0x008D1E28
// vtable 0x009063D4 (vptr 0x009063DC), offset_to_top 0, 6 entries
//
// A loaded CGFX file (aligned to 128 bytes); the destructor cleans the file up (names are ours).
class GraphicsFileResNw4c : public ::sead::DirectResource
{
public:
    GraphicsFileResNw4c(); // 0x0054A180
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074CE88 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DirectResource::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074CE34 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834F4 (guard 0x009834F0)
        static const RuntimeTypeInfo::Derive<DirectResource> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~GraphicsFileResNw4c(); // 0x0054A1DC slot 0x08
    // 0x0054A1A8 slot 0x0C (deleting dtor)
    virtual s32 getLoadDataAlignment() const { return 128; } // 0x0074CE80 slot 0x10
    virtual void doCreate_(u8* data, u32 size, Heap* heap); // 0x0054A164 slot 0x14

    nw::gfx::res::ResGraphicsFile mResFile; // 0x14
};
ASSERT_SIZE(GraphicsFileResNw4c, 0x18);
} // namespace sead
