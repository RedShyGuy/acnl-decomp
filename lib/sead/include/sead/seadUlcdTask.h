#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenTask.h"

namespace sead {
// RTTI N4sead8UlcdTaskE @ 0x008D2240
// vtable 0x00906E0C (vptr 0x00906E14), offset_to_top 0, 27 entries
class UlcdTask : public ::sead::DualScreenTask
{
public:
    UlcdTask(); // ctor address unknown
    virtual ~UlcdTask(); // 0x00561968 slot 0x00 | nintendogs:bytes
    // 0x005618D4 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074FA80 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DualScreenTask::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074FA34 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835DC (guard 0x009835D8)
        static const RuntimeTypeInfo::Derive<DualScreenTask> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void pauseDraw(bool); // 0x00561860 slot 0x14 | nintendogs:bytes
    virtual void vf_0x1C(); // 0x0056172C slot 0x1C | virtual slot, introduced by sead::TaskBase
    virtual void attachDrawImpl(); // 0x005617A0 slot 0x40 | nintendogs:bytes-fuzzy
    virtual void detachDrawImpl(); // 0x00561834 slot 0x48 | nintendogs:bytes
    virtual void vf_0x4C(); // 0x0074FB84 slot 0x4C | virtual slot, introduced by sead::TaskBase
    virtual void getMethodTreeNode(int); // 0x00544BF4 slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void vf_0x5C(); // 0x0056184C slot 0x5C | virtual slot, introduced by sead::DualScreenTask
    virtual void vf_0x64(); // 0x00561858 slot 0x64 | virtual slot, introduced by sead::UlcdTask
    virtual void vf_0x68(); // 0x0056185C slot 0x68 | virtual slot, introduced by sead::UlcdTask
};
} // namespace sead
