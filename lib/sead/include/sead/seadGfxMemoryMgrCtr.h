#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead15GfxMemoryMgrCtrE @ 0x008D188C
class GfxMemoryMgrCtr
{
public:
    GfxMemoryMgrCtr(); // ctor address unknown
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
};
} // namespace sead
