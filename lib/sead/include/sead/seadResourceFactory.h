#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadIDisposer.h"
#include "sead/seadResourceMgr.h"
#include "sead/seadTListNode.h"

namespace sead {
class Resource;

// RTTI N4sead15ResourceFactoryE @ 0x008D18B0
// vtable 0x00905B34 (vptr 0x00905B3C), offset_to_top 0, 7 entries
//
// Makes resources from files, registered under a name in the ResourceMgr. The names of the slots and
// members are ours.
class ResourceFactory : public ::sead::TListNode<sead::ResourceFactory*>, public ::sead::IDisposer
{
public:
    // (inline in the constructors of the factories)
    ResourceFactory() : TListNode<ResourceFactory*>(this), IDisposer(), mName() {}
    virtual ~ResourceFactory(); // 0x00546590 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054652C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C714 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C6C8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834AC (guard 0x009834A8)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    // loads the file of arg and makes a resource of it (NULL if it failed)
    virtual Resource* tryLoad(const ResourceMgr::LoadArg& arg) = 0; // slot 0x10
    // the same with the file decompressed by decompressor
    virtual Resource* tryLoadWithDecomp(const ResourceMgr::LoadArg& arg, Decompressor* decompressor) = 0; // slot 0x14
    // makes a resource of a file in memory
    virtual Resource* tryCreate(const ResourceMgr::CreateArg& arg) = 0; // slot 0x18

    FixedSafeString<32> mName; // 0x20
};
ASSERT_SIZE(ResourceFactory, 0x4C);
} // namespace sead
