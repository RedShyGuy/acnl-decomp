#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadGfxMemoryMgrCtr.h"

namespace sead {
// RTTI N4sead22DefaultGfxMemoryMgrCtrE @ 0x008D1F14
// vtable 0x009066D8 (vptr 0x009066E0), offset_to_top 0, 6 entries
class DefaultGfxMemoryMgrCtr : public ::sead::GfxMemoryMgrCtr
{
public:
    DefaultGfxMemoryMgrCtr(); // ctor candidate(s) 0x0011EBAC (unverified)
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
    virtual void vf_0x08(); // 0x0054BAC4 slot 0x08 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
    virtual void vf_0x0C(); // 0x0054BAB0 slot 0x0C | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
    virtual void vf_0x10(); // 0x0054BC5C slot 0x10 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
    virtual void vf_0x14(); // 0x0054BC58 slot 0x14 | virtual slot, introduced by sead::DefaultGfxMemoryMgrCtr
};
} // namespace sead
