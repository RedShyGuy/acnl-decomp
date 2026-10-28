#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResourceFactory.h"

namespace sead {
// RTTI N4sead25DirectResourceFactoryBaseE @ 0x008D1F4C
class DirectResourceFactoryBase : public ::sead::ResourceFactory
{
public:
    DirectResourceFactoryBase(); // ctor address unknown
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
};
} // namespace sead
