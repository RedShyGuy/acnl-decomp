#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResourceFactoryBase.h"
#include "sead/seadNew.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DirectResourceFactory<sead::DirectResource>  typeinfo 0x008D1E90  vtable 0x00906584
//   sead::DirectResourceFactory<sead::GraphicsFileResNw4c>  typeinfo 0x008D1E9C  vtable 0x009065AC
//
// The factory of the resource type T (newResource_ is the name of the reference symbols).
template <typename T>
class DirectResourceFactory : public DirectResourceFactoryBase
{
public:
    DirectResourceFactory() : DirectResourceFactoryBase() {}
    virtual ~DirectResourceFactory() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DirectResourceFactoryBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983524 (guard 0x00983520) [<sead::DirectResource>]
        // 0x0098352C (guard 0x00983528) [<sead::GraphicsFileResNw4c>]
        static const RuntimeTypeInfo::Derive<DirectResourceFactoryBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual DirectResource* newResource_(Heap* heap, s32 alignment) { return new (heap, alignment) T; }
};
} // namespace sead
