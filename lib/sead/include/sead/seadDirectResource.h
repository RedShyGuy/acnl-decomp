#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResource.h"

namespace sead {
class Heap;

// RTTI N4sead14DirectResourceE @ 0x008D16EC
// vtable 0x009057A4 (vptr 0x009057AC), offset_to_top 0, 6 entries
//
// A resource that is the loaded file itself. The names of the members and slots are ours.
class DirectResource : public ::sead::Resource
{
public:
    DirectResource(); // 0x005448A8
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BE14 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Resource::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BDC0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098346C (guard 0x00983468)
        static const RuntimeTypeInfo::Derive<Resource> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DirectResource(); // 0x005448FC slot 0x08
    // 0x005448CC slot 0x0C (deleting dtor)
    // the alignment of the loaded file
    virtual s32 getLoadDataAlignment() const { return 4; } // 0x0074BE0C slot 0x10
    // called with the loaded file
    virtual void doCreate_(u8* data, u32 size, Heap* heap); // 0x00544890 slot 0x14

    // takes the loaded file (inline in the factories; once only)
    void create(u8* data, u32 size, u32 bufferSize, bool needDelete, Heap* heap)
    {
        if (mData != nullptr) {
            return;
        }
        mData = data;
        mSize = size;
        mBufferSize = bufferSize;
        if (needDelete) {
            mFlags |= FLAG_NEED_DELETE;
        } else {
            mFlags &= ~FLAG_NEED_DELETE;
        }
        doCreate_(data, size, heap);
    }

    enum
    {
        FLAG_NEED_DELETE = 1 << 0, // the destructor frees mData
    };

    u8* mData;        // 0x04
    u32 mSize;        // 0x08
    u32 mBufferSize;  // 0x0C
    u32 mFlags;       // 0x10
};
ASSERT_SIZE(DirectResource, 0x14);
} // namespace sead
