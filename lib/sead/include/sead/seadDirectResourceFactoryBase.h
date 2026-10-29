#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResourceFactory.h"

namespace sead {
class DirectResource;

// RTTI N4sead25DirectResourceFactoryBaseE @ 0x008D1F4C
//
// A factory of DirectResources: loads the file and gives it to the resource that newResource_ makes
// (no vtable of its own in the binary; the slots are shared by the DirectResourceFactory instances).
class DirectResourceFactoryBase : public ::sead::ResourceFactory
{
public:
    DirectResourceFactoryBase() : ResourceFactory() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ResourceFactory::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983574 (guard 0x00983570)
        static const RuntimeTypeInfo::Derive<ResourceFactory> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual Resource* tryLoad(const ResourceMgr::LoadArg& arg); // 0x0054D7E8 slot 0x10
    virtual Resource* tryLoadWithDecomp(const ResourceMgr::LoadArg& arg, Decompressor* decompressor); // 0x0054D640 slot 0x14
    virtual Resource* tryCreate(const ResourceMgr::CreateArg& arg); // 0x0054D728 slot 0x18
    // a new resource object (of the type of the factory)
    virtual DirectResource* newResource_(Heap* heap, s32 alignment) = 0; // slot 0x1C
};
} // namespace sead
